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

ostype = $(OSTYPE)$(OS)

all: pack


# --------------------------------------------------------------------
# pack

pack: test pack_$(ostype)

pack_linux:
	cp $(CURDIR)/../src/*.h $(CURDIR)/../out/include

pack_Windows_NT:
	xcopy /y /i $(MAKEDIR)\..\src\*.h $(MAKEDIR)\..\out\include


# --------------------------------------------------------------------
# test

test: build test_$(ostype)

test_linux:
	$(CURDIR)/../out/test

test_windows_NT:
	$(MAKEDIR)\..\out\test.exe


# --------------------------------------------------------------------
# build

build: clean build_$(ostype)

build_linux: $(CURDIR)/../out/test

build_Windows_NT: $(MAKEDIR)\..\out\test.exe

$(CURDIR)/../out/test:
	g++ -std=c++14 -Wpedantic -Wl,-l:libstdc++.so.6 -Wl,-l:libgcc_s.so.1 -Wl,-l:libpthread.so -o $(CURDIR)/../out/test $(CURDIR)/*.cpp

$(MAKEDIR)\..\out\test.exe:
	cl -W4 -EHs -Od -D:DEBUG -Fo:$(MAKEDIR)\..\out\obj\ -Fe:$(MAKEDIR)\..\out\test.exe $(MAKEDIR)\*.cpp


# --------------------------------------------------------------------
# clean

clean: clean_$(ostype)

clean_linux:
	rm -fdr $(CURDIR)/../out
	mkdir $(CURDIR)/../out
	mkdir $(CURDIR)/../out/obj
	mkdir $(CURDIR)/../out/include

clean_Windows_NT:
	rmdir /s /q $(MAKEDIR)\..\out
	mkdir $(MAKEDIR)\..\out
	mkdir $(MAKEDIR)\..\out\obj
	mkdir $(MAKEDIR)\..\out\include

