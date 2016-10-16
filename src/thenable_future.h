/**
MIT License

Copyright (c) 2016 Zlatko Michailov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
**/

#pragma once

#include <functional>
#include <future>

template <template <class> class Future, class Value>
class _base_thenable_future
    : public Future<Value>
{
public:
    _base_thenable_future(std::future<Value> other)
        : Future<Value>(std::move(other))
    {
    }

    _base_thenable_future(const std::shared_future<Value> other)
        : Future<Value>(other)
    {
    }

    _base_thenable_future(const _base_thenable_future<Future, Value>& other)
        : Future<Value>(other)
    {
    }

    _base_thenable_future(_base_thenable_future<Future, Value>&& other) noexcept
        : Future<Value>(std::move(other))
    {
    }

    template <class Function, class... Args>
    static _base_thenable_future<Future, std::result_of_t<Function(Args...)>> start(Function&& function, Args&&... args)
    {
        return 
            std::move(
                // Wrap the std::future returned by std::async into a _base_thenable_future.
                _base_thenable_future<Future, std::result_of_t<Function(Args...)>>(
                     std::async(std::launch::async, function, args...)));
    }

    template <class Function, class... Args> 
    _base_thenable_future<Future, std::result_of_t<Function(_base_thenable_future<Future, Value>&&, Args...)>> then(Function&& function, Args&&... args)
    {
        return 
            std::move(
                start(
                    [this, &function, &args...]() -> std::result_of_t<Function(_base_thenable_future<Future, Value>&&, Args...)>
                    {
                        // Wait for this future to complete.
                        wait();

                        // "Move" this instance to the continuation.
                        return function(std::move(*this), args...);
                    }));
    }
};

template <class Value>
using thenable_future = _base_thenable_future<std::future, Value>; 

template <class Value>
using thenable_shared_future = _base_thenable_future<std::shared_future, Value>; 
