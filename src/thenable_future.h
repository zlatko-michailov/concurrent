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

template <class Value>
class thenable_future
    : public std::shared_future<Value>
{
public:
    thenable_future(std::future<Value>&& other) noexcept
        : std::shared_future<Value>(std::move(other))
    {
    }

    thenable_future(const std::shared_future<Value>& other)
        : std::shared_future<Value>(other)
    {
    }

    thenable_future(const thenable_future<Value>& other)
        : std::shared_future<Value>(other)
    {
    }

    thenable_future(thenable_future<Value>&& other) noexcept
        : std::shared_future<Value>(std::move(other))
    {
    }

    template <class Function, class... Args>
    static thenable_future<std::result_of_t<Function(Args...)>> start(Function&& function, Args&&... args)
    {
        return
            std::move(
                // Wrap the std::future returned by std::async into a thenable_future.
                thenable_future<std::result_of_t<Function(Args...)>>(
                     std::move(
                         std::async(std::launch::async, function, args...))));
    }

    template <class Function, class... Args> 
    thenable_future<std::result_of_t<Function(thenable_future<Value>&&, Args...)>> then(Function&& function, Args&&... args)
    {
        return 
            std::move(
                // Start a new thenable_future that will wait on *this before doing anything else.
                // Since *this may be destroyed by the time the function gets called, *this must be copied.
                // This is why we can only support std::shared_future as a base class.
                start(
                    [&function, &args...](thenable_future<Value> antecedent) -> std::result_of_t<Function(thenable_future<Value>&&, Args...)>
                    {
                        // Wait for the antecedent (the former *this) to complete.
                        antecedent.wait();
 
                        // Move the antecedent to the continuation.
                        return function(std::move(antecedent), args...);
                    }, *this));
    }
};
