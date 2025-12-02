# Tests

This library uses [valFuzz](https://github.com/San7o/valFuzz) for
testing, which is automatically downloaded and compiled by the build
system.

To build and run the tests, use the following commands:

```bash
cmake -Bbuild -DVALFUZZ_BUILD_TESTS=ON
cmake --build build -j 4
./buid/tests --no-multithread
```
