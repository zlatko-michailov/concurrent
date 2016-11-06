# wait.h
Built-in wait functions.

A wait function takes one parameter of type `size_t` that represents the attempt to wait.
The function may wait in a different way based on how many attempts have been made so far.


## Function `wait::spin`
Does nothing, which effectively causes the calling thread to "spin". This is useful in low-contention scenarios, e.g. in-memory synchronization within the same process.

## Function `wait::progressive_sleep`
Spins a few times, and if it still gets called, takes a brief sleep. Then repats the cycle. This is useful in unpredicatable-latency scenarios. 

## Function `wait::fail`
Directly throws an exception. This is useful in cases where the program implements its own synchronization, and waiting should never happen.

