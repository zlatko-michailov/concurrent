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

#define test_fail5(m1, m2, m3, m4, m5) \
    { \
        std::cout << "FAIL: " << m1 << m2 << m3 << m4 << m5 << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")" << std::endl; \
        return false; \
    }

#define test_fail4(m1, m2, m3, m4) test_fail5((m1), (m2), (m3), (m4), "")
#define test_fail3(m1, m2, m3) test_fail4((m1), (m2), (m3), "")

#define test_are_equal(a, b) \
    { \
        if ((a) != (b)) \
        { \
            test_fail3(a, " == ", b); \
        } \
    }

#define test_is_true(a) \
    { \
        test_are_equal(a, true); \
    }

#define test_is_false(a) \
    { \
        test_are_equal(a, false); \
    }

#define test_catch(ex_t, stmt) \
    { \
        try \
        { \
            stmt; \
            test_fail3("Expected exception '", #ex_t, "' that did not come."); \
        } \
        catch (const ex_t&) \
        { \
        } \
        catch (const std::exception& _ex) \
        { \
            test_fail4("Expected exception '", #ex_t, "', but got: ", _ex.what()); \
        } \
    }
