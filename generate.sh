#!/bin/bash
echo "Generating Makefiles..."
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
echo "Generating compile_commands.json for clangd..."
ln -sf build/compile_commands.json compile_commands.json
echo "Done!"

echo "To compile run:\n cmake --build build -j":
