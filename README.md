# concurrent
Concurrency and async patterns for C++.


## Purpose
This tiny SDK implements some useful patterns that are not currently available through the standard library.
In particular:
- [Lock-free synchronization](doc/lock_free_mutex.md)
- [Chaining of futures](doc/thenable_future.md)
- [Ring buffer](doc/ring.md)  


## How to Use this SDK
All you need is the header files from the [src](src) folder.
You can either clone the repo, or you can download them manually.
Then:
```c++
#include "<path>/concurrent.h"
```


## API Reference
- [`wait.h`](doc/wait.md) 
- [`lock_free_mutex.h`](doc/lock_free_mutex.md)
- [`thenable_future.h`](doc/thenable_future.md) 
- [`ring.h`](doc/ring.md)


## How to Build and Run the Tests
__Note:__ Unless you are modifying the SDK, you may skip this section.

To build and run the tests in Windows, run:
```cmd
<path>\util\build-win.cmd
```
It requires that Visual Studio is installed in the default location.

If you are using VS Code (and you have Visual Studio installed in the default location),
you can build and run the tests by simply pressing __Ctrl+Shift+B__.

If you have a different C++ package that includes `nmake`, you can:
```cmd
pushd <path>\test
nmake makefile-win
```

Soon, there will be a GNU make file for POSIX systems.


