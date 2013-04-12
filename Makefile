CXX = clang++ -arch i386 -arch x86_64
CXXLIB = $(CXX) -shared -fPIC -compatibility_version 1 -current_version 1 -Wl,-flat_namespace,-undefined,suppress -dynamiclib

all: sample.dylib

clean:
	rm -f sample.dylib

sample.dylib: sample.cpp $(wildcard include/*.hpp) $(wildcard include/interpose/*.hpp) $(wildcard include/interpose/tools/*.hpp)
	$(CXXLIB) -Iinclude -o sample.dylib sample.cpp -lpthread

test-calc: sample.dylib
	DYLD_INSERT_LIBRARIES=sample.dylib /Applications/Calculator.app/Contents/MacOS/Calculator

test-term: sample.dylib
	DYLD_INSERT_LIBRARIES=sample.dylib /Applications/Utilities/Terminal.app/Contents/MacOS/Terminal

test-chrome: sample.dylib
	DYLD_INSERT_LIBRARIES=sample.dylib /Applications/Google\ Chrome.app/Contents/MacOS/Google\ Chrome --user-data-dir=".chrome"

test-netbeans: sample.dylib
	DYLD_INSERT_LIBRARIES=sample.dylib /Applications/NetBeans/NetBeans\ 7.3.app/Contents/MacOS/netbeans

test-itunes: sample.dylib
	DYLD_INSERT_LIBRARIES=sample.dylib /Applications/iTunes.app/Contents/MacOS/iTunes

test-bash: sample.dylib
	DYLD_INSERT_LIBRARIES=sample.dylib bash
