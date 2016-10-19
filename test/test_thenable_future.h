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

#include <thread>

#include "../src/concurrent.h"
#include "test_common.h"

template <template <class> class ThenableFuture>
class test_any_thenable_future
{
public:
    static bool test_constructor()
    {
        ThenableFuture<int> future = std::async(std::launch::async, []() -> int { return 42; });
        int actual_value = future.get();
        test_are_equal(actual_value, 42);
        return true;
    }

    static bool test_start()
    {
        ThenableFuture<int> future = ThenableFuture<int>::start([]() -> int { return 42; });
        int actual_value = future.get();
        test_are_equal(actual_value, 42);
        return true;
    }

    static bool test_then()
    {
        ThenableFuture<long> future = 
            ThenableFuture<short>::start([]() -> short { return (short)42; })
                .then([](ThenableFuture<short>&& antecedent) -> int { return (int)(antecedent.get() + 1); })
                .then([](ThenableFuture<int>&& antecedent) -> unsigned { return (unsigned)(antecedent.get() + 1); })
                .then([](ThenableFuture<unsigned>&& antecedent) -> long { return (long)(antecedent.get() + 1); });

        long actual_value = future.get();
        test_are_equal(actual_value, 45);
        return true;
    }
};

typedef test_any_thenable_future<concurrent::thenable_future> test_thenable_future;

typedef test_any_thenable_future<concurrent::thenable_shared_future> test_thenable_shared_future;