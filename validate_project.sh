#!/bin/bash

# Project validation script for Linux/WSL
# For Windows, use Git Bash or WSL

echo "====================================="
echo "LCU Simulator Project Validation"
echo "====================================="
echo ""

ERRORS=0
WARNINGS=0

# Check for required files
echo "Checking project structure..."

required_files=(
    "CMakeLists.txt"
    "README.md"
    "src/main.cpp"
    "src/mainwindow.h"
    "src/mainwindow.cpp"
    "src/lcuscene.h"
    "src/lcuscene.cpp"
    "src/datamodel.h"
    "src/datamodel.cpp"
    "src/animationcontroller.h"
    "src/animationcontroller.cpp"
)

for file in "${required_files[@]}"; do
    if [ -f "$file" ]; then
        echo "  ✓ $file"
    else
        echo "  ✗ $file (MISSING)"
        ((ERRORS++))
    fi
done

echo ""

# Check component files
echo "Checking component files..."

components=(
    "basecomponent"
    "pump"
    "valve"
    "tank"
    "heater"
    "heatexchanger"
    "condenser"
    "blower"
    "solenoidvalve"
    "pipe"
)

for comp in "${components[@]}"; do
    header="src/components/${comp}.h"
    source="src/components/${comp}.cpp"
    
    if [ -f "$header" ] && [ -f "$source" ]; then
        echo "  ✓ $comp"
    else
        echo "  ✗ $comp (MISSING)"
        [ ! -f "$header" ] && echo "    Missing: $header" && ((ERRORS++))
        [ ! -f "$source" ] && echo "    Missing: $source" && ((ERRORS++))
    fi
done

echo ""

# Check documentation
echo "Checking documentation..."

docs=(
    "README.md"
    "TESTING.md"
    "INTEGRATION_GUIDE.md"
    "test_data.json"
)

for doc in "${docs[@]}"; do
    if [ -f "$doc" ]; then
        echo "  ✓ $doc"
    else
        echo "  ⚠ $doc (MISSING)"
        ((WARNINGS++))
    fi
done

echo ""

# Check build scripts
echo "Checking build scripts..."

if [ -f "build.bat" ]; then
    echo "  ✓ build.bat (Windows)"
else
    echo "  ⚠ build.bat (MISSING)"
    ((WARNINGS++))
fi

if [ -f "run.bat" ]; then
    echo "  ✓ run.bat (Windows)"
else
    echo "  ⚠ run.bat (MISSING)"
    ((WARNINGS++))
fi

echo ""

# Count source files
echo "Project statistics..."
echo "  Header files: $(find src -name "*.h" 2>/dev/null | wc -l)"
echo "  Source files: $(find src -name "*.cpp" 2>/dev/null | wc -l)"
echo "  Total lines of code: $(find src -name "*.cpp" -o -name "*.h" 2>/dev/null | xargs wc -l 2>/dev/null | tail -1 | awk '{print $1}')"

echo ""

# Check CMakeLists.txt content
echo "Validating CMakeLists.txt..."

if [ -f "CMakeLists.txt" ]; then
    if grep -q "Qt6" CMakeLists.txt; then
        echo "  ✓ Qt6 dependency found"
    else
        echo "  ✗ Qt6 dependency missing"
        ((ERRORS++))
    fi
    
    if grep -q "LiquidCoolingUnit" CMakeLists.txt; then
        echo "  ✓ Project name set"
    else
        echo "  ⚠ Project name not found"
        ((WARNINGS++))
    fi
fi

echo ""

# Summary
echo "====================================="
echo "Validation Summary"
echo "====================================="
echo "  Errors: $ERRORS"
echo "  Warnings: $WARNINGS"

if [ $ERRORS -eq 0 ]; then
    echo ""
    echo "✓ Project structure is valid!"
    echo ""
    echo "Next steps:"
    echo "  1. Install Qt6 (https://www.qt.io/download)"
    echo "  2. Run build.bat (Windows) or cmake build"
    echo "  3. See README.md for detailed build instructions"
    exit 0
else
    echo ""
    echo "✗ Project has errors that need to be fixed."
    exit 1
fi
