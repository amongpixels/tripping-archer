#!/bin/bash
cd linux ; cmake .. ; make ; ./tripping-archer -i ../test/input.bmp -o test-heightmap.png -c test-texture.png -s
