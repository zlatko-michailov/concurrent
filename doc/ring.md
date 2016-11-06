# ring.h

## Class `template <class Container> class ring`
Implements the ring functionality over an arbitrary _Sequence Container_.

### Thread Safety
This class is safe to be one pusher and one popper simultaneously.

If more than one pusher or more than one popper have to operate on the same instance,
access to each end should be synchronized. See [lock_free_mutex](lock_free_mutex.md). 

### Constructors
#### `ring(size_t size, const std::function<void(size_t)>& wait)`
Constructor for containters like `std::vector` and `std::deque` that need to be constructed with an initial size.

Parameter | Description
--------- | -----------
`wait` | Wait function. See [wait.h](wait.md) for built-in wait functions. A user-defined function is also acceptable.  


#### `ring(const std::function<void(size_t)>& wait)`
Constructor for containters like `std::array` that don't take initial size at construction.

Parameter | Description
--------- | -----------
`wait` | Wait function. See [wait.h](wait.md) for built-in wait functions. A user-defined function is also acceptable.  


### Methods
#### `size_t size() const`
Returns the size of the container.


#### `bool empty() const`
Returns `true` if there is nothing currently to pop.


#### `void push_back(typename Container::const_reference value)`
Pushes a value at the back end of the ring.

__Note:__ If there is no available space to push, i.e. `available_to_push() == 0`, waits until an item gets popped.


#### `void push_back(typename Container::value_type&& value)`
Pushes a value at the back end of the ring.

__Note:__ If there is no available space to push, i.e. `available_to_push() == 0`, waits until an item gets popped.


#### `void push_back_eof()`
Virtually pushes an `eof` flag at the back end of the ring to mark that no further input should be expected.

__Note:__ This method doesn't wait even if `available_to_push() == 0`.


#### `void pop_front()`
Pops the element at the front end of the ring.

__Note:__ If there is no available item to pop, i.e. `available_to_pop() == 0`, waits until an item gets pushed.


#### `typename Container::reference front()`
References the item at the front end without popping it.

__Note:__ If there is no available item to pop, i.e. `available_to_pop() == 0`, waits until an item gets pushed.


#### `typename Container::const_reference front() const`
References the item at the front end without popping it.

__Note:__ If there is no available item to pop, i.e. `available_to_pop() == 0`, waits until an item gets pushed.


#### `bool eof() const`
Returns `true` if the virtual `eof` is at the front. 

__Note:__ This method doesn't necessarily return `true` right after `push_back_eof()` has been called.
It will turn `true` after all the items currently in the conntainer have been popped.


#### `size_t total_push_count() const`
Returns the total count of pushed items (not counting the virtual `eof` mark.)


#### `size_t available_to_push() const`
Returns the available spaces to push items to.


#### `size_t total_pop_count() const`
Returns the total count of popped items.


#### `size_t available_to_pop() const`
Returns the available items to pop.
