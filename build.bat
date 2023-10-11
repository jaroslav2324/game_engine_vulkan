@echo off

cd build
cmake -S .. -B . -G "Ninja"
ninja all
