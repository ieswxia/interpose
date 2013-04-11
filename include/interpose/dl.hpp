#if !defined(INTERPOSE_DL_HPP)
#define INTERPOSE_DL_HPP

#include <dlfcn.h>
#include "interpose.hpp"

struct interpose_dl_root : public InterposeRoot {
public:
	void* dlopen(const char* filename, int flag) {
		return ::dlopen(filename, flag);
	}
	
	char* dlerror() {
		return ::dlerror();
	}
	
	void* dlsym(void* handle, const char* symbol) {
		return ::dlsym(handle, symbol);
	}
	
	int dlclose(void* handle) {
		return ::dlclose(handle);
	}
};

#undef InterposeRoot
#define InterposeRoot interpose_dl_root

template<class Interloper> struct interpose_dl_host : public Interpose<Interloper> {
	interpose_dl_host() {
		INTERPOSE(dlopen);
		INTERPOSE(dlerror);
		INTERPOSE(dlsym);
		INTERPOSE(dlclose);
	}
	
	static void* dlopen(const char* filename, int flag) {
		return Interpose<Interloper>::instance()->dlopen(filename, flag);
	}
	
	static char* dlerror() {
		return Interpose<Interloper>::instance()->dlerror();
	}
	
	static void* dlsym(void* handle, const char* symbol) {
		return Interpose<Interloper>::instance()->dlsym(handle, symbol);
	}

	static int dlclose(void* handle) {
		return Interpose<Interloper>::instance()->dlclose(handle);
	}
};

#undef Interpose
#define Interpose interpose_dl_host

#endif
