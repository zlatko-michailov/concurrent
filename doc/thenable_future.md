# thenable_future.h

Provides classes that derive from `std::future` and `std::shared_future` respectively that allow "chaining".

__Note:__ Since futures can take a relatively long time to complete,
`spin` may not be a good strategy to wait for a future to complete.
`progressive_sleep` or a custom wait function might be more appropriate.

## Class `thenable_future`
`template <class Value> class thenable_future : public std::future<Value>`


### Constructors
#### `thenable_future(std::future<Value> other)`
Parameter | Description
--------- | -----------
`other` | Takes over a `std::future` instance.  

#### `thenable_future(const std::shared_future<Value> other)`
Parameter | Description
--------- | -----------
`other` | Copies a `std::shared_future` instance.  

#### `thenable_future(const thenable_future<Value>& other)`
Parameter | Description
--------- | -----------
`other` | Copy constructor.  

#### `thenable_future(_thenable_future<Value>&& other) noexcept`
Parameter | Description
--------- | -----------
`other` | Move constructor.  


### Methods
#### `template <class Function, class... Args> static thenable_future<std::result_of_t<Function(Args...)>> start(Function&& function, Args&&... args)`
Wraps around the `std::future` rturned by `std::async`, and makes it thenable. 

Parameter | Description
--------- | -----------
`Function&& function` | Function to be called. 
`Args&&... args` | Set of arguments to be passed to the function. 

#### `template <class Function, class... Args> thenable_future<std::result_of_t<Function(thenable_future<Value>&&, Args...)>> then(Function&& function, Args&&... args)`
Starts a new thenable future based on a function that first waits for the current thenable future to complete,
and then calls `function`. 

Parameter | Description
--------- | -----------
`Function&& function` | Function to be called. 
`Args&&... args` | Set of arguments to be passed to the function after the 1st one. The first parameter is this thenable future. 


## Class `thenable_shared_future`
`template <class Value> class thenable_shared_future : public std::shared_future<Value>`


### Constructors
#### `thenable_shared_future(std::future<Value> other)`
Parameter | Description
--------- | -----------
`other` | Takes over a `std::future` instance.  

#### `thenable_shared_future(const std::shared_future<Value> other)`
Parameter | Description
--------- | -----------
`other` | Copies a `std::shared_future` instance.  

#### `thenable_shared_future(const thenable_shared_future<Value>& other)`
Parameter | Description
--------- | -----------
`other` | Copy constructor.  

#### `thenable_shared_future(_thenable_shared_future<Value>&& other) noexcept`
Parameter | Description
--------- | -----------
`other` | Move constructor.  


### Methods
#### `template <class Function, class... Args> static thenable_shared_future<std::result_of_t<Function(Args...)>> start(Function&& function, Args&&... args)`
Wraps around the `std::future` rturned by `std::async`, and makes it thenable and shared. 

Parameter | Description
--------- | -----------
`Function&& function` | Function to be called. 
`Args&&... args` | Set of arguments to be passed to the function. 

#### `template <class Function, class... Args> thenable_shared_future<std::result_of_t<Function(thenable_shared_future<Value>&&, Args...)>> then(Function&& function, Args&&... args)`
Starts a new thenable shared future based on a function that first waits for the current thenable shared future to complete,
and then calls `function`. 

Parameter | Description
--------- | -----------
`Function&& function` | Function to be called. 
`Args&&... args` | Set of arguments to be passed to the function after the 1st one. The first parameter is this thenable shared future. 
