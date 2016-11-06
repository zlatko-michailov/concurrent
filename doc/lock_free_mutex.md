# lock_free_mutex.h


## Class `lock_free_mutex`
As the name suggests, this is a lock-free mutex.
It is intended for low-contention scenarios, like in-memory operations within the same process,
where blocking and unblocking a thread would take longer than the actual operation.

This primitive would not be suitable for scenarios where an operation could take a relatively long time, like networking or storage. 


### Constructors
#### `lock_free_mutex(std::function<void(size_t)>&& wait)` 
Constructs a `lock_free_mutex` instance with the specified wait function.

Parameter | Description
--------- | -----------
`wait` | Wait function. See [wait.h](wait.md) for built-in wait functions. A user-defined function is also acceptable.  


### Methods
#### `template <class Function, class... Args> std::result_of_t<Function(Args...)> sync(Function&& function, Args&&... args)`
"Waits" until aquires the mutex, calls `function`, and releases the mutex.

The wait function provided at construction is called everytime this method fails to aquire the mutex.  

Parameter | Description
--------- | -----------
`Function&& function` | Function to be called. 
`Args&&... args` | Set of arguments to be passed to the function. 

