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

class test_thenable_future
{
public:
    static bool test_constructor()
    {
        concurrent::thenable_future<int> tf1 = std::async(std::launch::async, []() -> int { return 42; });
        int actual_value = tf1.get();
        test_are_equal(actual_value, 42);
        return true;
    }

    static bool test_start()
    {
        concurrent::thenable_future<int> tf1 = concurrent::thenable_future<int>::start([]() -> int { return 42; });
        int actual_value = tf1.get();
        test_are_equal(actual_value, 42);
        return true;
    }

    static bool test_then()
    {
        concurrent::thenable_future<long> future = 
            concurrent::thenable_future<short>::start([]() -> short { return (short)42; })
                .then([](std::shared_future<short>& future) -> int { return (int)(future.get() + 1); })
                .then([](std::shared_future<int>& future) -> unsigned { return (unsigned)(future.get() + 1); })
                .then([](std::shared_future<unsigned>& future) -> long { return (long)(future.get() + 1); });

        long actual_value = future.get();
        test_are_equal(actual_value, 45);
        return true;
    }

    static bool test_shared_start_then()
    {
        return true;
    }

private:
};
