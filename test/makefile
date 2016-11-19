# MIT License
# 
# Copyright (c) 2016 Zlatko Michailov
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.


all: init_$(OSTYPE) clean build test pack

init_linux:
dir_out = $(CURDIR)/../out
#dir_obj = $(dir_out)/obj
dir_pack = $(dir_out)/include
dir_src = $(CURDIR)
dir_include = $(CURDIR)/../src
exe_test = $(dir_out)/test
file_h = $(dir_include)/*.h
file_cpp = $(dir_src)/*.cpp
cmd_rmdir = rm -fdr
cmd_mkdir = mkdir
cmd_copy = cp
cmd_cc = g++ -std=c++14 -Wpedantic -Wl,-l:libstdc++.so.6 -Wl,-l:libgcc_s.so.1 -Wl,-l:libpthread.so -o

init_: #windows
dir_out = $(MAKEDIR)\..\out
dir_obj = $(dir_out)\obj
dir_pack = $(dir_out)\include
dir_src = $(MAKEDIR)
dir_include = $(MAKEDIR)\..\src
exe_test = $(dir_out)\test.exe
file_h = $(dir_include)\*.h
file_cpp = $(dir_src)\*.cpp
cmd_rmdir = rmdir /s /q
cmd_mkdir = mkdir
cmd_copy = xcopy /y /i
cmd_cc = cl -W4 -EHs -Od -D:DEBUG -Fo:$(dir_obj) -Fe:

clean:
	$(cmd_rmdir) $(dir_out)
	$(cmd_mkdir) $(dir_out)
	$(cmd_mkdir) $(dir_obj)
	$(cmd_mkdir) $(dir_pack)

pack: test
	$(cmd_copy) $(file_h) $(dir_pack)

test: build
	$(exe_test)

build: clean $(exe_test)

$(exe_test):
	$(cmd_cc) $(exe_test) $(file_cpp)


