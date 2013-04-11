# Interpose
Interpose is a set of C++ header files for interposing on POSIX functions from a preloaded dynamic library.  The library allows you to intercept various subsets of POSIX functionality.  Currently, only ```pthread.h``` and ```dlfcn.h``` functions are supported, but more will be added shortly.

## Usage
The file ```sample.cpp``` shows how to interpose on several functions.  The example just prints a debug message and forwards the call along to the real POSIX function.

Every library that uses Interpose must include ```interpose.hpp```.  If you need to intercept pthread functions, include ```interpose/pthread.hpp```.  To intercept the dynamic linker functions, include ```interpose/dl.hpp```.

After including the required headers, you need to create a class or struct with the functions you want to intercept.  This struct should inherit from InterposeRoot, so any functions not trapped will be forwarded on to the POSIX implementation.  The following struct intercepts calls to ```dlopen```, reports the opened library, and forward the call on to POSIX:

	struct dl_logger : public InterposeBase {
		void* dlopen(const char* filename, int flag) {
			fprintf(stderr, "Opening %s\n", filename);
			
			// Pass the call on to POSIX
			return InterposeBase::dlopen(filename, flag));
		}
	};

Once the interposition code has been written, you must declare *exactly one* instance of the interposition code:

	Interpose<dl_logger> interposer; // Only one allowed!

This creates all the necessary interposition functions, and invokes all intercepted methods on the ```interposer``` object.

If you compile this code to ```dl_logger.dylib``` and run a program with the environment variable ```DYLD_INSERT_LIBRARIES=dl_logger.dylib```, this code will print a message every time a new dynamic library is opened.  The included Makefile includes several targets for testing against common applications.  Run ```make test-chrome``` to preload the sample library against Google Chrome.
