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
#include <stdexcept>
#include <vector>

template <class Container>
class ring_buffer
{
public:
    ring_buffer(size_t size, const std::function<void(size_t)>& wait)
        : container(size)
        , wait(wait)
        , virtual_write_index(0)
        , virtual_read_index(0)
        , eof_flag(false)
    {
    }

    size_t size() const
    {
        return ring_size();
    }

    bool empty() const
    {
        return virtual_read_index == virtual_write_index; 
    }

    void push_back(typename Container::const_reference value)
    {
        wait_until_available(&ring_buffer::available_to_write);
        container[write_index()] = value;
        virtual_write_index++;
    }

    void push_back(typename Container::value_type&& value)
    {
        wait_until_available(&ring_buffer::available_to_write);
        container[write_index()] = value;
        virtual_write_index++;
    }

    void push_back_eof()
    {
        eof_flag = true;
    }

    void pop_front()
    {
        wait_until_available(&ring_buffer::available_to_read);
        virtual_read_index++;
    }

    typename Container::reference front()
    {
        wait_until_available(&ring_buffer::available_to_read);
        return container[read_index()];
    }

    typename Container::const_reference front() const
    {
        wait_until_available(&ring_buffer::available_to_read);
        return container[read_index()];
    }

    bool eof() const
    {
        return eof_flag && empty();
    }

    size_t total_write_count() const
    {
        return virtual_write_index;
    }

    size_t available_to_write() const
    {
        return complement_of(available_to_read());
    }

    size_t total_read_count() const
    {
        return virtual_read_index;
    }

    size_t available_to_read() const
    {
        return virtual_write_index - virtual_read_index;
    }

private:
    void wait_until_available(const std::function<size_t(const ring_buffer<Container>&)>& available) const
    {
        for (size_t spin_count = 0; available(*this) == 0; spin_count++)
        {
            wait(spin_count);
        }
    }

    size_t write_index() const
    {
        return ring_of(virtual_write_index);
    }

    size_t read_index() const
    {
        return ring_of(virtual_read_index);
    }

    size_t ring_of(size_t value) const
    {
        return value % ring_size(); 
    }

    size_t complement_of(size_t value) const
    {
        return ring_size() - value;
    }

    size_t ring_size() const
    {
        return container.size();
    }

private:
    Container container;
    std::function<void(size_t)> wait;
    volatile size_t virtual_write_index;
    volatile size_t virtual_read_index;
    volatile bool eof_flag;
};
