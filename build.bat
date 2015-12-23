@echo off
del "Gear-Ratio.exe"

clang -std=c99 Solver.c -o Gear-Ratio.exe
echo done compiling
"Gear-Ratio"
popd
