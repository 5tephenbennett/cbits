#!/bin/bash

# CBits Coverage Analysis Script
# Uses gcov/lcov for accurate line and function coverage reporting

set -e

log() {
  echo ">> $@" >&2
}

# Check if we're in the right directory
if [ ! -f "build.sh" ] || [ ! -d "bit_ops" ]; then
    log "Error: Please run this script from the cbits project root directory"
    exit 1
fi

BUILD_DIR="build"
COVERAGE_DIR="$BUILD_DIR/coverage"

# Function to extract API functions from header files
extract_api_functions() {
    local header_file=$1
    local bit_width=$2

    # Extract function declarations (lines with BIT{N}_ and ending with ;)
    grep -E "^[[:space:]]*[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]+BIT${bit_width}_[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]*\(" "$header_file" | \
    sed -E 's/^[[:space:]]*[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]+//' | \
    sed -E 's/[[:space:]]*\(.*$//' | \
    sort | uniq
}

# Function to extract function coverage from gcov data
extract_function_coverage() {
    local bit_width=$1
    local src_file="bit${bit_width}_ops.c"

    if [ ! -f "$COVERAGE_DIR/coverage.info" ]; then
        return
    fi

    # Extract function data from lcov info file for this source file
    awk -v srcfile="$src_file" '
        /^SF:/ {
            current_file = $0;
            gsub(/^SF:.*\//, "", current_file);
            in_target_file = (current_file == srcfile)
        }
        /^FN:/ && in_target_file {
            split($0, parts, ",");
            func_name = parts[2];
            if (func_name ~ /^BIT'${bit_width}'_/) {
                functions[func_name] = 0;
            }
        }
        /^FNDA:/ && in_target_file {
            split($0, parts, ",");
            hit_count = parts[1];
            gsub(/^FNDA:/, "", hit_count);
            func_name = parts[2];
            if (func_name ~ /^BIT'${bit_width}'_/ && hit_count > 0) {
                functions[func_name] = hit_count;
            }
        }
        END {
            for (func in functions) {
                print func ":" functions[func];
            }
        }' "$COVERAGE_DIR/coverage.info" | sort
}

# Function to extract all BIT functions from source file (including non-API functions)
extract_source_functions() {
    local src_file=$1
    local bit_width=$2

    if [ ! -f "$src_file" ]; then
        return
    fi

    # Extract function definitions from source file (including char * return types)
    # Filter out duplicates and functions already in header
    grep -E "^(char|uint|int|BOOL).*BIT${bit_width}_[a-zA-Z_][a-zA-Z0-9_]*[[:space:]]*\(" "$src_file" | \
    sed -E 's/^[^B]*BIT/BIT/' | \
    sed -E 's/[[:space:]]*\(.*$//' | \
    grep -E "^BIT${bit_width}_[a-zA-Z_][a-zA-Z0-9_]*$" | \
    sort | uniq
}

# Function to generate coverage report for a specific bit width using gcov data
analyze_bit_width_coverage() {
    local bit_width=$1
    local header_file="bit_ops/include/bit${bit_width}_ops.h"
    local src_file="bit_ops/src/bit${bit_width}_ops.c"

    log "=== Analyzing BIT${bit_width} Coverage ==="

    if [ ! -f "$header_file" ]; then
        log "Warning: Header file $header_file not found"
        return
    fi

    # Extract API functions from header
    local api_functions=$(extract_api_functions "$header_file" "$bit_width")
    local api_count=$(echo "$api_functions" | wc -l)

    # Extract additional functions from source file (like Bitstring functions)
    local source_functions=""
    if [ -f "$src_file" ]; then
        source_functions=$(extract_source_functions "$src_file" "$bit_width")
    fi

    # Combine and deduplicate
    local all_functions=$(echo -e "$api_functions\n$source_functions" | sort | uniq | grep -v '^$')
    local total_count=$(echo "$all_functions" | wc -l)

    echo "API Functions from header (${api_count} total):"
    echo "$api_functions" | sed 's/^/  /'
    echo

    if [ "$total_count" -gt "$api_count" ]; then
        local additional_functions=$(comm -13 <(echo "$api_functions" | sort) <(echo "$all_functions" | sort))
        echo "Additional functions from source ($(echo "$additional_functions" | wc -l) total):"
        echo "$additional_functions" | sed 's/^/  /'
        echo
    fi

    if [ -f "$COVERAGE_DIR/coverage.info" ]; then
        # Get function coverage from gcov data
        local coverage_data=$(extract_function_coverage "$bit_width")

        if [ -n "$coverage_data" ]; then
            echo "Function Coverage (from gcov):"
            local tested_count=0
            local api_tested=0

            while IFS= read -r func; do
                if [ -z "$func" ]; then continue; fi

                local coverage_info=$(echo "$coverage_data" | grep "^${func}:" || echo "${func}:0")
                local hit_count=$(echo "$coverage_info" | cut -d: -f2)

                # Check if this is an API function
                local is_api=$(echo "$api_functions" | grep -x "$func" || echo "")

                if [ "$hit_count" -gt 0 ]; then
                    if [ -n "$is_api" ]; then
                        echo "  ✅ $func (executed $hit_count times) [API]"
                        api_tested=$((api_tested + 1))
                    else
                        echo "  ✅ $func (executed $hit_count times) [Internal]"
                    fi
                    tested_count=$((tested_count + 1))
                else
                    if [ -n "$is_api" ]; then
                        echo "  ❌ $func (not executed) [API]"
                    else
                        echo "  ❌ $func (not executed) [Internal]"
                    fi
                fi
            done <<< "$all_functions"

            echo

            if [ "$api_tested" -eq "$api_count" ]; then
                echo "✅ All API functions covered!"
            else
                local api_untested=$((api_count - api_tested))
                echo "⚠️  $api_untested API functions not covered"
            fi

            # Calculate accurate percentage for API functions
            local api_coverage_percent=0
            if [ "$api_count" -gt 0 ]; then
                api_coverage_percent=$(( api_tested * 100 / api_count ))
            fi

            echo "API Coverage: $api_tested/$api_count functions ($api_coverage_percent%)"
            echo "Total Coverage: $tested_count/$total_count functions ($(( tested_count * 100 / total_count ))%)"
        else
            echo "No coverage data found for BIT${bit_width} functions"
            echo "Coverage: 0/$total_count functions (0%)"
        fi
    else
        echo "No coverage.info file found - please run with coverage enabled"
        echo "Coverage: Unknown"
    fi

    echo "=================================================================="
    echo
}

# Function to run gcov coverage analysis and generate reports
run_gcov_analysis() {
    if [ ! -d "$BUILD_DIR" ]; then
        log "Build directory not found. Please run build with coverage first: ./build.sh --debug --coverage"
        return
    fi

    log "Generating coverage reports..."

    # Create coverage directory
    mkdir -p "$COVERAGE_DIR"

    # Check if we have coverage data
    local gcda_files=$(find "$BUILD_DIR/bit_ops" -name "*.gcda" 2>/dev/null | wc -l)
    if [ "$gcda_files" -eq 0 ]; then
        log "No coverage data (.gcda files) found. Please run tests after building with coverage:"
        log "  ./build.sh --debug --clean --coverage"
        log "  ./build/bit_ops/test/bit_ops_gtest"
        return
    fi

    # Generate lcov coverage info
    if command -v lcov >/dev/null 2>&1; then
        log "Capturing coverage data with lcov..."
        lcov --directory "$BUILD_DIR/bit_ops" --capture --output-file "$COVERAGE_DIR/coverage.info" --quiet || {
            log "Warning: lcov capture failed"
            return
        }

        # Filter to only include our source files
        lcov --extract "$COVERAGE_DIR/coverage.info" "*/bit_ops/src/*" --output-file "$COVERAGE_DIR/coverage_filtered.info" --quiet || {
            log "Warning: lcov filter failed"
            return
        }

        # Use filtered data
        cp "$COVERAGE_DIR/coverage_filtered.info" "$COVERAGE_DIR/coverage.info"

        # Generate HTML report
        if [ -f "$COVERAGE_DIR/coverage.info" ]; then
            log "Generating HTML coverage report..."
            genhtml "$COVERAGE_DIR/coverage.info" --output-directory "$COVERAGE_DIR/html" --quiet --title "CBits Coverage Report" || {
                log "Warning: HTML generation failed"
            }

            if [ -d "$COVERAGE_DIR/html" ]; then
                log "HTML coverage report available at: $COVERAGE_DIR/html/index.html"
            fi
        fi
    else
        log "lcov not found. Install lcov for detailed coverage reports: sudo apt-get install lcov"
    fi
}

# Function to show overall summary
show_coverage_summary() {
    if [ -f "$COVERAGE_DIR/coverage.info" ]; then
        log "=== Overall Coverage Summary ==="
        lcov --summary "$COVERAGE_DIR/coverage.info" 2>/dev/null | grep -E "(lines|functions)" | sed 's/^/  /'
        echo
    fi
}

# Main execution
main() {
    log "Starting CBits Coverage Analysis (gcov/lcov based)"
    echo "=================================================================="

    # Generate coverage reports first
    run_gcov_analysis

    # Show overall summary
    show_coverage_summary

    # Analyze each bit width using actual coverage data
    for bit_width in 8 16 32 64; do
        analyze_bit_width_coverage $bit_width
    done

    log "Coverage analysis complete!"
}

# Run main function
main "$@"