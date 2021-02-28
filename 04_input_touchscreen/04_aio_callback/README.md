# NON/Blocking IO to read input

Build:

`arm-buildroot-linux-gnueabihf-gcc main.c -lrt -o app`

Execute:

`./app /path/to/input device`, example is `./app /dev/input/event0`
