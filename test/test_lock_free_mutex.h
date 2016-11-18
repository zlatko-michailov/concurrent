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

#include "../src/concurrent.h"
#include "test_common.h"

class test_lock_free_mutex
{
public:
    static bool test_sync_complete()
    {
        return test_sync(false);
    }

    static bool test_sync_throw()
    {
        return test_sync(true);
    }

private:
    static bool test_sync(bool with_throw)
    {
        shared_field = 0;
        std::vector<std::future<bool>> futures(agent_count);

        for (size_t agent_index = 0; agent_index < agent_count; agent_index++)
        {
            futures[agent_index] = std::async(std::launch::async, async_proc, agent_index, with_throw);
        }

        for (size_t agent_index = 0; agent_index < agent_count; agent_index++)
        {
            try
            {
                bool result = futures[agent_index].get();
                test_is_true(result && !should_throw(with_throw, agent_index));
            }
            catch(...)
            {
                test_is_true(should_throw(with_throw, agent_index));
            }
        }

        test_are_equal(shared_field, agent_count);
        return true;
    }

    static bool async_proc(size_t agent_index, bool with_throw)
    {
        return lock_free_mutex.sync(sync_proc, agent_index, with_throw);
    }

    static bool sync_proc(size_t agent_index, bool with_throw)
    {
        size_t current_value = shared_field;

        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_milliseconds));
        test_are_equal(shared_field, current_value);
        shared_field = current_value + 1;

        if (should_throw(with_throw, agent_index))
        {
            throw L"Surprise!";
        }

        return true;
    }

    static bool should_throw(bool with_throw, size_t agent_index)
    {
        return with_throw && ((agent_index & 1) == 1);
    }

private:
    static concurrent::lock_free_mutex lock_free_mutex;
    static volatile size_t shared_field;

    static const size_t agent_count;
    static const size_t sleep_milliseconds;
};

concurrent::lock_free_mutex test_lock_free_mutex::lock_free_mutex(&concurrent::wait::spin);
volatile size_t test_lock_free_mutex::shared_field;
const size_t test_lock_free_mutex::agent_count = 100;
const size_t test_lock_free_mutex::sleep_milliseconds = 10;

