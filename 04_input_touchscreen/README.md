# INPUT System Linux

## IO models

## Blocking/Non-Blocking IO model

![Blocking io](.mdfiles/blocking-io.png)

![non-blocking io](.mdfiles/non-blocking-io.png)

Example:

[Example code read from input - non/blocking.](./01_blocking_io/README.md)

## IO Multipluxing model

IO multipluxing model allows user to watch multiple descriptors, once any of them are readable/writable, the application is able to read/write. The operation is sync blocking IO since the application is in charge of read/write which is blocking.

![multi-plexing](.mdfiles/io-multiplexing.png)


## Signal Driven IO model

![sig-driven-io](.mdfiles/sig-driven-io.png)

```
#include <signal.h>

int sigaction(int signum, const struct sigaction *act,
             struct sigaction *oldact); 
```

## Async IO model

![async-io](.mdfiles/async-io.png)
