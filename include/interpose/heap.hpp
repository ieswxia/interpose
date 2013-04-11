#if !defined(INTERPOSE_HEAP_HPP)
#define INTERPOSE_HEAP_HPP

#include <stdlib.h>
#include "interpose.hpp"

struct interpose_heap_root : public InterposeRoot {
public:
	void* malloc(size_t size) {
		return ::malloc(size);
	}
	
	void free(void* ptr) {
		::free(ptr);
	}
	
	void* calloc(size_t nmemb, size_t size) {
		return ::calloc(nmemb, size);
	}
	
	void* realloc(void* ptr, size_t size) {
		return ::realloc(ptr, size);
	}
	
	void* reallocf(void* ptr, size_t size) {
		return ::reallocf(ptr, size);
	}
	
	void* valloc(size_t size) {
		return ::valloc(size);
	}
	
	int posix_memalign(void** memptr, size_t alignment, size_t size) {
		return ::posix_memalign(memptr, alignment, size);
	}
};

#undef InterposeRoot
#define InterposeRoot interpose_heap_root

template<class Interloper> struct interpose_heap_host : public Interpose<Interloper> {
	interpose_heap_host() {
		INTERPOSE(malloc);
		INTERPOSE(free);
		INTERPOSE(calloc);
		INTERPOSE(realloc);
		INTERPOSE(reallocf);
		INTERPOSE(valloc);
		INTERPOSE(posix_memalign);
	}
	
	static void* malloc(size_t size) {
		return Interpose<Interloper>::instance()->malloc(size);
	}
	
	static void free(void* ptr) {
		Interpose<Interloper>::instance()->free(ptr);
	}
	
	static void* calloc(size_t nmemb, size_t size) {
		return Interpose<Interloper>::instance()->calloc(nmemb, size);
	}
	
	static void* realloc(void* ptr, size_t size) {
		return Interpose<Interloper>::instance()->realloc(ptr, size);
	}
	
	static void* reallocf(void* ptr, size_t size) {
		return Interpose<Interloper>::instance()->reallocf(ptr, size);
	}
	
	static void* valloc(size_t size) {
		return Interpose<Interloper>::instance()->valloc(size);
	}
	
	static int posix_memalign(void** memptr, size_t alignment, size_t size) {
		return Interpose<Interloper>::instance()->posix_memalign(memptr, alignment, size);
	}
};

#undef Interpose
#define Interpose interpose_heap_host

#endif
