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

#include <array>
#include <deque>
#include <vector>

#include "../src/concurrent.h"
#include "test_common.h"

class test_ring
{
public:
    static bool test_ring_vector()
    {
        concurrent::ring_vector<int> ring(5, concurrent::wait::fail);
        return test_ring_container(ring);
    }

    static bool test_ring_deque()
    {
        concurrent::ring_deque<int> ring(5, concurrent::wait::fail);
        return test_ring_container(ring);
    }

    static bool test_ring_array()
    {
        concurrent::ring_array<int, 5> ring(concurrent::wait::fail);
        return test_ring_container(ring);
    }

private:
    template <class Container>
    static bool test_ring_container(concurrent::ring<Container>& ring)
    {
        // Thos test assumes ring.size() == 5.
        test_are_equal(5, ring.size());

        // Empty.
        test_catch(std::logic_error, ring.front());
        test_are_equal(ring.available_to_write(), ring.size());
        test_are_equal(ring.available_to_read(), 0);
        test_are_equal(ring.total_write_count(), 0);
        test_are_equal(ring.total_read_count(), 0);
        test_is_false(ring.eof());

        // Write 2 items.
        ring.push_back(42);
        ring.push_back(99);
        test_are_equal(ring.available_to_write(), ring.size() - 2);
        test_are_equal(ring.available_to_read(), 2);
        test_are_equal(ring.total_write_count(), 2);
        test_are_equal(ring.total_read_count(), 0);
        test_is_false(ring.eof());

        // Read 2 items.
        test_are_equal(42, ring.front());
        ring.pop_front();
        test_are_equal(99, ring.front());
        ring.pop_front();
        test_are_equal(ring.available_to_write(), ring.size());
        test_are_equal(ring.available_to_read(), 0);
        test_are_equal(ring.total_write_count(), 2);
        test_are_equal(ring.total_read_count(), 2);
        test_is_false(ring.eof());

        // Write 5 items.
        ring.push_back(31);
        ring.push_back(29);
        ring.push_back(23);
        ring.push_back(17);
        ring.push_back(13);
        ring.push_back_eof();
        test_are_equal(ring.available_to_write(), 0);
        test_are_equal(ring.available_to_read(), ring.size());
        test_are_equal(ring.total_write_count(), 7);
        test_are_equal(ring.total_read_count(), 2);
        test_is_false(ring.eof());
        
        // Read 5 items.
        test_are_equal(31, ring.front());
        ring.pop_front();
        test_are_equal(29, ring.front());
        ring.pop_front();
        test_are_equal(23, ring.front());
        ring.pop_front();
        test_are_equal(17, ring.front());
        ring.pop_front();
        test_are_equal(13, ring.front());
        test_is_false(ring.eof());
        ring.pop_front();
        test_are_equal(ring.available_to_write(), ring.size());
        test_are_equal(ring.available_to_read(), 0);
        test_are_equal(ring.total_write_count(), 7);
        test_are_equal(ring.total_read_count(), 7);
        test_is_true(ring.eof());
        
        return true;
    }    
};


