cd win
cmake .. -G "MinGW Makefiles"
mingw32-make
tripping-archer -i ../test/input.bmp -o test-heightmap.bmp -c test-texture.bmp -s
