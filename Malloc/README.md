# Simple Memory Allocator

This project implements a simple memory allocator similar to the standard ```malloc```.

To compile and run the tests, run:

```
cmake -S . -B Build
cmake --build build
( cd build ; ctest --output-on-failure)
```