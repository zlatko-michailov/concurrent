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

#include <atomic>
#include <functional>

class lock_free_mutex
{
public:
    lock_free_mutex()
    {
        // Make sure the flag is clear.
        sync_flag.clear();
    }

    template <class TFunction, class... TArgs>
    std::result_of_t<TFunction(TArgs...)> sync(TFunction&& function, TArgs&&... args)
    {
        // Spin until acquire.
        while (sync_flag.test_and_set())
        {
            std::this_thread::yield();
        }

        try
        {
            std::result_of_t<TFunction(TArgs...)> result = function(args...);

            // Release and return on completion.
            sync_flag.clear();
            return result;
        }
        catch(...)
        {
            // Release and rethrow on exception.
            sync_flag.clear();
            throw;
        }
    }

private:
    std::atomic_flag sync_flag;
};