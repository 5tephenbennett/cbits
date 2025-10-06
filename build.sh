#!/bin/sh

# Build script for cbits project
set -e

# log function that outputs to stderr - handles both arguments and stdin
log() {
  if [ $# -gt 0 ]; then
    echo ">> $@" >&2
  else
    # No arguments - read from stdin and prefix each line
    while IFS= read -r line; do
      echo "$line" >&2
    done
  fi
}

# Print usage information
print_usage() {
  cat << EOF | log
Usage: $0 [OPTIONS]

Options:
  -d, --debug       Build in Debug mode (default: Release)
  -r, --release     Build in Release mode
  --build-dir DIR   Build directory (default: build)
  --clean           Clean build directory first
  --install         Install after building
  --install-prefix DIR  Set install prefix (default: /usr/local)
  --no-tests        Don't build tests
  --sanitizers      Force enable sanitizers for library (auto-enabled for tests)
  --sanitizers-release  Enable sanitizers for library even in release builds
  --coverage        Enable code coverage analysis
  --format          Format code with clang-format
  --format-check    Check code formatting with clang-format
  --tidy[=MODE]     Run clang-tidy analysis. MODE can be:
                      info (default) - show aggressive checks without fixing
                      fix - basic auto-fix mode
                      fix-aggressive - aggressive auto-fix with confirmation
  --docs            Generate documentation with Doxygen
  -h, --help        Show this help message
EOF
}

# Default values
BUILD_TYPE="Release"
BUILD_DIR="build"
CLEAN=false
INSTALL=false
INSTALL_PREFIX=""
TESTS=true
SANITIZERS=false
SANITIZERS_RELEASE=false
COVERAGE=false
FORMAT=false
FORMAT_CHECK=false
TIDY=""
DOCS=false

# Parse command line arguments
while [ $# -gt 0 ]; do
  case $1 in
    -d|--debug)
      BUILD_TYPE="Debug"
      shift
      ;;
    -r|--release)
      BUILD_TYPE="Release"
      shift
      ;;
    --build-dir)
      if [ -z "$2" ]; then
        log "Error: --build-dir requires a directory name"
        exit 1
      fi
      BUILD_DIR="$2"
      shift 2
      ;;
    --clean)
      CLEAN=true
      shift
      ;;
    --install)
      INSTALL=true
      shift
      ;;
    --install-prefix)
      if [ -z "$2" ]; then
        log "Error: --install-prefix requires a directory path"
        exit 1
      fi
      INSTALL_PREFIX="$2"
      shift 2
      ;;
    --no-tests)
      TESTS=false
      shift
      ;;
    --sanitizers)
      SANITIZERS=true
      shift
      ;;
    --sanitizers-release)
      SANITIZERS_RELEASE=true
      shift
      ;;
    --coverage)
      COVERAGE=true
      shift
      ;;
    --format)
      FORMAT=true
      shift
      ;;
    --format-check)
      FORMAT_CHECK=true
      shift
      ;;
    --tidy)
      TIDY="info"  # Default mode
      shift
      ;;
    --tidy=*)
      TIDY_VALUE="${1#*=}"
      case "$TIDY_VALUE" in
        "info"|"fix"|"fix-aggressive")
          TIDY="$TIDY_VALUE"
          ;;
        "")
          TIDY="info"  # Empty value defaults to info
          ;;
        *)
          log "Error: Invalid --tidy mode '$TIDY_VALUE'. Must be 'info', 'fix', or 'fix-aggressive'."
          exit 1
          ;;
      esac
      shift
      ;;
    --docs)
      DOCS=true
      shift
      ;;
    -h|--help)
      print_usage
      exit 0
      ;;
    *)
      log "Unknown option: $1"
      exit 1
      ;;
  esac
done

# Handle format-only operations (don't need build)
if [ "$FORMAT" = "true" ] || [ "$FORMAT_CHECK" = "true" ] || [ -n "$TIDY" ] || [ "$DOCS" = "true" ]; then
  # Find clang-format
  if [ "$FORMAT" = "true" ] || [ "$FORMAT_CHECK" = "true" ]; then
    CLANG_FORMAT=$(command -v clang-format || command -v clang-format-14 || command -v clang-format-15 || command -v clang-format-16)
    if [ -z "$CLANG_FORMAT" ]; then
      log "Error: clang-format not found. Please install clang-format."
      exit 1
    fi
  fi

  # Find clang-tidy
  if [ -n "$TIDY" ]; then
    CLANG_TIDY=$(command -v clang-tidy || command -v clang-tidy-14 || command -v clang-tidy-15 || command -v clang-tidy-16)
    if [ -z "$CLANG_TIDY" ]; then
      log "Error: clang-tidy not found. Please install clang-tidy."
      exit 1
    fi
  fi

  # Find doxygen
  if [ "$DOCS" = "true" ]; then
    DOXYGEN=$(command -v doxygen)
    if [ -z "$DOXYGEN" ]; then
      log "Error: doxygen not found. Please install doxygen."
      exit 1
    fi
  fi

  # Get source files
  SOURCE_FILES=$(find bit_ops/src bit_ops/include bit_ops/test -name "*.c" -o -name "*.h" -o -name "*.cpp" -o -name "*.hpp" 2>/dev/null | grep -v CMakeFiles | sort)
  if [ -z "$SOURCE_FILES" ]; then
    log "Error: No source files found"
    exit 1
  fi

  # Run format operations
  if [ "$FORMAT" = "true" ]; then
    log "Formatting code with clang-format..."
    echo "$SOURCE_FILES" | xargs $CLANG_FORMAT -i
    log "Code formatting completed!"
    exit 0
  fi

  if [ "$FORMAT_CHECK" = "true" ]; then
    log "Checking code formatting with clang-format..."
    FORMAT_ISSUES=0
    for file in $SOURCE_FILES; do
      if ! $CLANG_FORMAT "$file" >/dev/null 2>&1; then
        log "Formatting issue in: $file"
        FORMAT_ISSUES=$((FORMAT_ISSUES + 1))
      fi
    done
    if [ $FORMAT_ISSUES -eq 0 ]; then
      log "All files are properly formatted!"
      exit 0
    else
      log "Found $FORMAT_ISSUES files with formatting issues. Run with --format to fix."
      exit 1
    fi
  fi

  if [ -n "$TIDY" ]; then
    # Common setup for all tidy modes

    # Build if compile_commands.json doesn't exist
    if [ ! -f "build/compile_commands.json" ]; then
      log "Building project with clang to generate compile_commands.json for clang-tidy..."
      CC=clang CXX=clang++ ./build.sh --debug > /dev/null 2>&1
    fi

    # Get only C source and header files (exclude C++ test files)
    C_SOURCE_FILES=$(find bit_ops/src bit_ops/include bit_ops/test/interactive -name "*.c" -o -name "*.h" 2>/dev/null | sort)

    if [ -z "$C_SOURCE_FILES" ]; then
      log "Error: No C source files found"
      exit 1
    fi

    case "$TIDY" in
      "info")
        # Regular tidy analysis without fixes, but using aggressive config to show all issues
        log "Running clang-tidy static analysis with aggressive checks (info mode)..."
        CONFIG_FILE=".clang-tidy-aggressive"

        # Check for config file
        if [ ! -f "$CONFIG_FILE" ]; then
          log "Error: Configuration file $CONFIG_FILE not found."
          exit 1
        fi

        log "Files to analyze:"
        echo "$C_SOURCE_FILES" | log
        echo ""
        log "Running clang-tidy analysis..."

        # Process files with clang-tidy (no --fix flag)
        for file in $C_SOURCE_FILES; do
          log "Analyzing: $file"
          $CLANG_TIDY "$file" -p build --config-file="$CONFIG_FILE" \
              2>&1 | grep -v "unknown warning option" | grep -v "unsupported argument.*bounds-strict" | grep -v "did you mean"
        done

        log "Static analysis completed!"
        ;;

      "fix")
        # Basic auto-fix mode
        CONFIG_FILE=".clang-tidy"
        log "Running clang-tidy auto-fix (basic mode)..."

        # Check for config file
        if [ ! -f "$CONFIG_FILE" ]; then
          log "Error: Configuration file $CONFIG_FILE not found."
          exit 1
        fi

        log "Files to process:"
        echo "$C_SOURCE_FILES" | log
        echo ""
        log "Running clang-tidy with auto-fix..."

        # Process files
        fixed_count=0
        skipped_count=0

        for file in $C_SOURCE_FILES; do
          log "Processing: $file"

          # Run clang-tidy and suppress known warnings about GCC-specific flags
          if $CLANG_TIDY "$file" -p build --config-file="$CONFIG_FILE" --fix --fix-errors \
              2>&1 | grep -v "unknown warning option" | grep -v "unsupported argument.*bounds-strict" | grep -v "did you mean" > /tmp/clang_tidy_output 2>&1; then

            # Check if there were any real issues fixed
            if [ -s /tmp/clang_tidy_output ] && grep -q -E "warning|error|note.*FIX-IT" /tmp/clang_tidy_output; then
              log "  ⚡ Fixed issues"
              fixed_count=$((fixed_count + 1))
            else
              log "  ✓ No issues"
            fi
          else
            log "  ⚠ Skipped (compilation issues)"
            skipped_count=$((skipped_count + 1))
          fi
        done

        # Clean up
        rm -f /tmp/clang_tidy_output

        # Summary
        echo ""
        log "Auto-fix completed!"
        log "Fixed: $fixed_count files"
        if [ $skipped_count -gt 0 ]; then
          log "Skipped: $skipped_count files"
        fi
        echo ""
        log "Run './build.sh --tidy' to see remaining issues."
        ;;

      "fix-aggressive")
        # Aggressive auto-fix mode
        CONFIG_FILE=".clang-tidy-aggressive"
        log "Running AGGRESSIVE clang-tidy auto-fix..."
        log "WARNING: This may make significant code changes!"
        echo ""
        printf "Continue? (y/N): "
        read -r reply
        if [ "$reply" != "y" ] && [ "$reply" != "Y" ]; then
          log "Aborted."
          exit 0
        fi

        # Check for config file
        if [ ! -f "$CONFIG_FILE" ]; then
          log "Error: Configuration file $CONFIG_FILE not found."
          exit 1
        fi

        log "Files to process:"
        echo "$C_SOURCE_FILES" | log
        echo ""
        log "Running clang-tidy with aggressive auto-fix..."

        # Process files
        fixed_count=0
        skipped_count=0

        for file in $C_SOURCE_FILES; do
          log "Processing: $file"

          # Run clang-tidy and suppress known warnings about GCC-specific flags
          if $CLANG_TIDY "$file" -p build --config-file="$CONFIG_FILE" --fix --fix-errors \
              2>&1 | grep -v "unknown warning option" | grep -v "unsupported argument.*bounds-strict" | grep -v "did you mean" > /tmp/clang_tidy_output 2>&1; then

            # Check if there were any real issues fixed
            if [ -s /tmp/clang_tidy_output ] && grep -q -E "warning|error|note.*FIX-IT" /tmp/clang_tidy_output; then
              log "  ⚡ Fixed issues"
              fixed_count=$((fixed_count + 1))
            else
              log "  ✓ No issues"
            fi
          else
            log "  ⚠ Skipped (compilation issues)"
            skipped_count=$((skipped_count + 1))
          fi
        done

        # Clean up
        rm -f /tmp/clang_tidy_output

        # Summary
        echo ""
        log "Auto-fix completed!"
        log "Fixed: $fixed_count files"
        if [ $skipped_count -gt 0 ]; then
          log "Skipped: $skipped_count files"
        fi
        echo ""
        log "Note: Aggressive mode was used. Please review changes carefully."
        ;;

      *)
        log "Error: Invalid TIDY mode '$TIDY'. Must be 'info', 'fix', or 'fix-aggressive'."
        exit 1
        ;;
    esac
    exit 0
  fi

  if [ "$DOCS" = "true" ]; then
    log "Generating documentation with Doxygen..."

    # Create docs directory if it doesn't exist
    mkdir -p docs

    # Create an enhanced Doxyfile if it doesn't exist
    if [ ! -f Doxyfile ]; then
      log "Creating enhanced Doxyfile with improved navigation..."
      cat > Doxyfile << 'EOF'
# Enhanced Doxyfile for CBits project with improved navigation
PROJECT_NAME           = "CBits"
PROJECT_BRIEF          = "C99 bit manipulation library with API coverage verification"
PROJECT_VERSION        = "0.2"
PROJECT_LOGO           =
OUTPUT_DIRECTORY       = docs
INPUT                  = bit_ops/include bit_ops/src README.md
RECURSIVE              = YES
EXTRACT_ALL            = YES
EXTRACT_PRIVATE        = NO
EXTRACT_STATIC         = YES
SOURCE_BROWSER         = YES
INLINE_SOURCES         = YES

# HTML Output Configuration
GENERATE_HTML          = YES
HTML_OUTPUT            = html
HTML_FILE_EXTENSION    = .html
HTML_HEADER            =
HTML_FOOTER            =
HTML_STYLESHEET        =
HTML_EXTRA_STYLESHEET  = docs/custom.css
HTML_EXTRA_FILES       =
HTML_COLORSTYLE_HUE    = 220
HTML_COLORSTYLE_SAT    = 100
HTML_COLORSTYLE_GAMMA  = 80
HTML_TIMESTAMP         = YES
HTML_DYNAMIC_MENUS     = YES
HTML_DYNAMIC_SECTIONS  = NO
HTML_INDEX_NUM_ENTRIES = 100

# Enhanced Navigation Features
GENERATE_TREEVIEW      = YES
ENUM_VALUES_PER_LINE   = 4
TREEVIEW_WIDTH         = 250
EXT_LINKS_IN_WINDOW    = NO
FORMULA_FONTSIZE       = 10
FORMULA_TRANSPARENT    = YES
USE_MATHJAX            = NO
MATHJAX_FORMAT         = HTML-CSS
MATHJAX_RELPATH        = http://cdn.mathjax.org/mathjax/latest
MATHJAX_EXTENSIONS     =
MATHJAX_CODEFILE       =
SEARCHENGINE           = YES
SERVER_BASED_SEARCH    = NO
EXTERNAL_SEARCH        = NO
SEARCHENGINE_URL       =
SEARCHDATA_FILE        = searchdata.xml
EXTERNAL_SEARCH_ID     =
EXTRA_SEARCH_MAPPINGS  =

# Group Organization
GROUP_NESTED_COMPOUNDS = NO
SUBGROUPING            = YES
INLINE_GROUPED_CLASSES = NO
INLINE_SIMPLE_STRUCTS  = NO

# Main Page and File Organization
USE_MDFILE_AS_MAINPAGE = README.md
LAYOUT_FILE            =
CITE_BIB_FILES         =

# C-specific optimizations
OPTIMIZE_OUTPUT_FOR_C  = YES
TYPEDEF_HIDES_STRUCT   = YES
EXTENSION_MAPPING      =
MARKDOWN_SUPPORT       = YES
TOC_INCLUDE_HEADINGS   = 0
AUTOLINK_SUPPORT       = YES
BUILTIN_STL_SUPPORT    = NO
CPP_CLI_SUPPORT        = NO
SIP_SUPPORT            = NO
IDL_PROPERTY_SUPPORT   = YES
DISTRIBUTE_GROUP_DOC   = NO
GROUP_NESTED_COMPOUNDS = NO

# Input and filtering
FILE_PATTERNS          = *.c *.h *.md
RECURSIVE              = YES
EXCLUDE                =
EXCLUDE_SYMLINKS       = NO
EXCLUDE_PATTERNS       = */test/* */CMakeFiles/* */build/*
EXCLUDE_SYMBOLS        =
EXAMPLE_PATH           =
EXAMPLE_PATTERNS       = *
EXAMPLE_RECURSIVE      = NO
IMAGE_PATH             =

# Source code features
SOURCE_BROWSER         = YES
INLINE_SOURCES         = YES
STRIP_CODE_COMMENTS    = YES
REFERENCED_BY_RELATION = YES
REFERENCES_RELATION    = YES
REFERENCES_LINK_SOURCE = YES
SOURCE_TOOLTIPS        = YES
USE_HTAGS              = NO
VERBATIM_HEADERS       = YES

# Alphabetical index
ALPHABETICAL_INDEX     = YES
COLS_IN_ALPHA_INDEX    = 5
IGNORE_PREFIX          =

# LaTeX output (disabled for better performance)
GENERATE_LATEX         = NO

# RTF output (disabled)
GENERATE_RTF           = NO

# Man page output (disabled)
GENERATE_MAN           = NO

# XML output (disabled)
GENERATE_XML           = NO

# Documentation extraction
QUIET                  = NO
WARNINGS               = YES
WARN_IF_UNDOCUMENTED   = YES
WARN_IF_DOC_ERROR      = YES
WARN_NO_PARAMDOC       = NO
WARN_AS_ERROR          = NO
WARN_FORMAT            = "$file:$line: $text"
WARN_LOGFILE           =

# Documentation style
JAVADOC_AUTOBRIEF      = YES
QT_AUTOBRIEF           = YES
MULTILINE_CPP_IS_BRIEF = YES
INHERIT_DOCS           = YES
SEPARATE_MEMBER_PAGES  = NO
TAB_SIZE               = 4
ALIASES                =
TCL_SUBST              =
OPTIMIZE_OUTPUT_JAVA   = NO
OPTIMIZE_FOR_FORTRAN   = NO
OPTIMIZE_OUTPUT_VHDL   = NO

# Graphs and diagrams
HAVE_DOT               = YES
DOT_NUM_THREADS        = 0
DOT_FONTNAME           = Helvetica
DOT_FONTSIZE           = 10
DOT_FONTPATH           =
CLASS_GRAPH            = YES
COLLABORATION_GRAPH    = YES
GROUP_GRAPHS           = YES
UML_LOOK               = NO
UML_LIMIT_NUM_FIELDS   = 10
TEMPLATE_RELATIONS     = NO
INCLUDE_GRAPH          = YES
INCLUDED_BY_GRAPH      = YES
CALL_GRAPH             = NO
CALLER_GRAPH           = NO
GRAPHICAL_HIERARCHY    = YES
DIRECTORY_GRAPH        = YES
DOT_IMAGE_FORMAT       = png
INTERACTIVE_SVG        = NO
DOT_PATH               =
DOTFILE_DIRS           =
MSCFILE_DIRS           =
DIAFILE_DIRS           =
PLANTUML_JAR_PATH      =
PLANTUML_CFG_FILE      =
PLANTUML_INCLUDE_PATH  =
DOT_GRAPH_MAX_NODES    = 50
MAX_DOT_GRAPH_DEPTH    = 0
DOT_TRANSPARENT        = NO
DOT_MULTI_TARGETS      = NO
GENERATE_LEGEND        = YES
DOT_CLEANUP            = YES
EOF
    fi

    # Run doxygen
    $DOXYGEN Doxyfile

    log "Documentation generated in docs/html/"
    log "Open docs/html/index.html in your browser to view the documentation"
    exit 0
  fi
fi

# Clean if requested
if [ "$CLEAN" = "true" ]; then
  log "Cleaning build directory..."
  rm -rf "$BUILD_DIR"
fi

# Create build directory
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure
log "Configuring project..."

CMAKE_ARGS="-DCMAKE_BUILD_TYPE=$BUILD_TYPE -DBUILD_TESTS=$TESTS"

if [ "$SANITIZERS" = "true" ]; then
  CMAKE_ARGS="$CMAKE_ARGS -DENABLE_SANITIZERS_FOR_LIB=ON"
fi

if [ "$SANITIZERS_RELEASE" = "true" ]; then
  CMAKE_ARGS="$CMAKE_ARGS -DENABLE_SANITIZERS_RELEASE=ON"
fi

if [ "$COVERAGE" = "true" ]; then
  CMAKE_ARGS="$CMAKE_ARGS -DENABLE_COVERAGE=ON"
fi

if [ -n "$INSTALL_PREFIX" ]; then
  CMAKE_ARGS="$CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX"
fi

if [ -n "$TIDY" ]; then
  # Use clang for clang-tidy to avoid GCC-specific flags in compile_commands.json
  export CC=clang
  export CXX=clang++
fi

cmake $CMAKE_ARGS ..

# Build
log "Building project..."
cmake --build . --parallel

# Copy compile_commands.json to project root for IDE support
if [ -f "compile_commands.json" ]; then
  cp compile_commands.json ..
  log "Updated compile_commands.json in project root for IDE support"
fi

# Run tests if built
if [ "$TESTS" = "true" ]; then
  log "Running tests..."
  ctest --output-on-failure
fi

# Install if requested
if [ "$INSTALL" = "true" ]; then
  log "Installing..."
  cmake --install .
fi

log "Build completed successfully!"