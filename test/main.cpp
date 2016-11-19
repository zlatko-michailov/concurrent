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

#include <initializer_list>
#include <deque>
#include <iostream>
#include <functional>

#include "test_lock_free_mutex.h"
#include "test_thenable_future.h"
#include "test_ring.h"


struct test
{
    const char* name;
    const std::function<bool()> function;

    test(const char* test_name, const std::function<bool()>& test_function)
        : name(test_name), function(test_function)
    {
    }
};

struct test_group
{
    const char* name;
    const std::deque<test> tests;

    test_group(const char* test_group_name, const std::initializer_list<test>& test_group_tests)
        : name(test_group_name), tests(test_group_tests)
    {
    }
};

test_group test_groups[] =
{
    test_group("lock_free_mutex",
    {
        test("sync(complete)", test_lock_free_mutex::test_sync_complete),
        test("sync(throw)", test_lock_free_mutex::test_sync_throw),
    }),
    test_group("thenable_future",
    {
        test("constructor", test_thenable_future::test_constructor),
        test("start", test_thenable_future::test_start),
        test("then", test_thenable_future::test_then),
    }),
    test_group("ring",
    {
        test("vector", test_ring::test_ring_vector),
        test("deque", test_ring::test_ring_deque),
        test("array", test_ring::test_ring_array),
    }),
};

int main()
{
    const char* section_separator = "------------------------------------------------------------------------";

    std::cout << std::endl;
    std::cout << section_separator << std::endl;
    std::cout << "TESTING..." << std::endl;

    bool pass = true;
    for (test_group test_group : test_groups)
    {
        bool group_pass = true;

        std::cout << section_separator << std::endl;
        std::cout << test_group.name << std::endl;

        for (test test : test_group.tests)
        {
            std::cout << "\t" << test_group.name << "::" << test.name << std::endl;
            bool test_pass = test.function();
            group_pass = (group_pass && test_pass);
            pass = (pass && test_pass);
            std::cout << "\t" << (test_pass ? "Pass" : "FAIL") << std::endl;
        }

        std::cout << (group_pass ? "Pass" : "FAIL") << std::endl;
    }

    std::cout << section_separator << std::endl;
    std::cout << (pass ? "PASS" : "FAIL") << std::endl;
    std::cout << section_separator << std::endl;

    return (pass ? 0 : 1);
}