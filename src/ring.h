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

template <class Container>
class ring
{
public:
    ring(size_t size, const std::function<void(size_t)>& wait)
        : container(size)
        , wait(wait)
        , virtual_push_index(0)
        , virtual_pop_index(0)
        , eof_flag(false)
    {
    }

    ring(const std::function<void(size_t)>& wait)
        : container()
        , wait(wait)
        , virtual_push_index(0)
        , virtual_pop_index(0)
        , eof_flag(false)
    {
    }

    size_t size() const
    {
        return ring_size();
    }

    bool empty() const
    {
        return virtual_pop_index == virtual_push_index; 
    }

    void push_back(typename Container::const_reference value)
    {
        if (eof_flag) throw std::logic_error("CONCURRENT_RING_01: Calling push_back() is not allowed after push_back_eof() has been called.");

        wait_until_available(&ring::available_to_push);
        container[push_index()] = value;
        virtual_push_index++;
    }

    void push_back(typename Container::value_type&& value)
    {
        if (eof_flag) throw std::logic_error("CONCURRENT_RING_01: Calling push_back() is not allowed after push_back_eof() has been called.");

        wait_until_available(&ring::available_to_push);
        container[push_index()] = value;
        virtual_push_index++;
    }

    void push_back_eof()
    {
        eof_flag = true;
    }

    void pop_front()
    {
        wait_until_available(&ring::available_to_pop);
        virtual_pop_index++;
    }

    typename Container::reference front()
    {
        wait_until_available(&ring::available_to_pop);
        return container[pop_index()];
    }

    typename Container::const_reference front() const
    {
        wait_until_available(&ring::available_to_pop);
        return container[pop_index()];
    }

    bool eof() const
    {
        return eof_flag && empty();
    }

    size_t total_push_count() const
    {
        return virtual_push_index;
    }

    size_t available_to_push() const
    {
        return complement_of(available_to_pop());
    }

    size_t total_pop_count() const
    {
        return virtual_pop_index;
    }

    size_t available_to_pop() const
    {
        return virtual_push_index - virtual_pop_index;
    }

private:
    void wait_until_available(const std::function<size_t(const ring<Container>&)>& available) const
    {
        for (size_t spin_count = 0; available(*this) == 0; spin_count++)
        {
            wait(spin_count);
        }
    }

    size_t push_index() const
    {
        return ring_of(virtual_push_index);
    }

    size_t pop_index() const
    {
        return ring_of(virtual_pop_index);
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
    volatile size_t virtual_push_index;
    volatile size_t virtual_pop_index;
    volatile bool eof_flag;
};

template <class T, class Allocator = std::allocator<T>>
using ring_vector = ring<std::vector<T, Allocator>>; 

template <class T, class Allocator = std::allocator<T>>
using ring_deque = ring<std::deque<T, Allocator>>; 

template <class T, size_t N>
using ring_array = ring<std::array<T, N>>; 
