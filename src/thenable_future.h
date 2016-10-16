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


template <class Value>
class thenable_future
    : public std::shared_future<Value>
{
public:
    thenable_future(std::future<Value> other)
        : std::shared_future<Value>(std::move(other))
    {
    }

    thenable_future(const std::shared_future<Value> other)
        : std::shared_future<Value>(other)
    {
    }

    thenable_future(const thenable_future<Value>& other)
        : std::shared_future<Value>(other)
    {
    }

    thenable_future(thenable_future<Value>&& other) noexcept
        : std::shared_future<Value>(other)
    {
    }

    template <class Function, class... Args>
    static thenable_future<std::result_of_t<Function(Args...)>> start(Function&& function, Args&&... args)
    {
        return 
            std::move(
                thenable_future<std::result_of_t<Function(Args...)>>(
                     std::async(std::launch::async, function, args...)));
    }

    template <class Function, class... Args> 
    thenable_future<std::result_of_t<Function(std::shared_future<Value>&, Args...)>> then(Function&& function, Args&&... args)
    {
        return 
            std::move(
                start(
                    [this, &function, &args...]() -> std::result_of_t<Function(std::shared_future<Value>&, Args...)>
                    {
                        wait();
                        return function(*this, args...);
                    }));
    }
};

#ifdef DO_NOT_COMPILE
template<template<class> class Future, class Value>
class _base_thenable_future : public Future<Value>
{
public:
    _base_thenable_future(std::future<Value>&& other)
        : future(other)
    {
    }

    _base_thenable_future(const std::shared_future<Value>& other)
        : future(other)
    {
    }

    _base_thenable_future(_base_thenable_future<std::future, Value>&& other)
        : future(other.future)
    {
    }

    _base_thenable_future(const _base_thenable_future<std::shared_future, Value>& other)
        : future(other.future)
    {
    }

    template <class Function, class... Args>
    static _base_thenable_future<Future, std::result_of_t<Function(Args...)>> start(Function&& function, Args&&... args)
    {
        return 
            std::move(
                _base_thenable_future<Future, std::result_of_t<Function(Args...)>>(
                     std::async(std::launch::async, function, args...)));
    }

    template <class Function, class... Args> 
    _base_thenable_future<Future, std::result_of_t<Function(std::shared_future<Value>&, Args&&...)>> then(Function&& function, Args&&... args)
    {
        return 
            //std::move(
                start(
                    [=this, &function, &args...]() -> std::result_of_t<Function(std::shared_future<Value>&, Args&&...)>
                    {
                        future.wait();
                        return function(std::shared_future<Value>(this->future), args...);
                    });
    }

private:
    Future<Value> future;
};

template<class Value>
using thenable_future = _base_thenable_future<std::future, Value>; 

template<class Value>
using shared_thenable_future = _base_thenable_future<std::shared_future, Value>; 
#endif
