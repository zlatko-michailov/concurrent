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

namespace wait
{
    void spin(size_t)
    {
        std::this_thread::yield();
    }

    void progressive_sleep(size_t spin_count)
    {
        if ((spin_count == 0) || (spin_count & 0x40ULL) != 0) // do 64 spins between sleeps 
        {
            std::this_thread::yield();
        }
        else
        {
            size_t sleep_milliseconds_power = ((spin_count >>= 6) & 0x03ULL); // use bts 6-8
            size_t sleep_milliseconds = (1ULL << sleep_milliseconds_power);

            std::this_thread::sleep_for(std::chrono::milliseconds(sleep_milliseconds));
        }
    }

    void fail(size_t)
    {
        throw std::logic_error("CONCURRENT_WAIT_01: Check preconditions before trying this operation.");
    }
}
