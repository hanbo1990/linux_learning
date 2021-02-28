# NON/Blocking IO to read input

Build:

`arm-buildroot-linux-gnueabihf-gcc main.c -o app`

Execute:

`./app /path/to/input device [nonblocking]`, example is `./app /dev/input/event0`

## Blocking read

![diagram](./blocking-io-example.png)

## Non-blocking read

![diagram-nonblocking](./non-blocking-io-example.png)
