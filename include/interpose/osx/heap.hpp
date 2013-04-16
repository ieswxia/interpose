#include <stdlib.h>
#include <cstdio>
#include <interpose/core.hpp>

namespace interpose {
	/// Expose the real heap functions
	struct heap_Root : public Root {
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
	
	// Redefine the Root macro to include the dl functions
	#undef Root
	#define Root heap_Root
	
	/// Template to interpose on heap functions
	template<class Super> struct heap : public Super {
	private:
		static heap<Super>*& instance() {
			static heap<Super>* _instance = NULL;
			return _instance;
		}
	
	public:
		heap() {
			if(instance() != NULL) {
				fprintf(stderr, "interpose error: multiple interpositions on heap\n");
				abort();
			}
			
			instance() = this;
			
			INTERPOSE(malloc);
			INTERPOSE(free);
			INTERPOSE(calloc);
			INTERPOSE(realloc);
			INTERPOSE(reallocf);
			INTERPOSE(valloc);
			INTERPOSE(posix_memalign);
		}
		
		static void* malloc(size_t size) {
			return instance()->Super::malloc(size);
		}

		static void free(void* ptr) {
			instance()->Super::free(ptr);
		}

		static void* calloc(size_t nmemb, size_t size) {
			return instance()->Super::calloc(nmemb, size);
		}

		static void* realloc(void* ptr, size_t size) {
			return instance()->Super::realloc(ptr, size);
		}

		static void* reallocf(void* ptr, size_t size) {
			return instance()->Super::reallocf(ptr, size);
		}

		static void* valloc(size_t size) {
			return instance()->Super::valloc(size);
		}

		static int posix_memalign(void** memptr, size_t alignment, size_t size) {
			return instance()->Super::posix_memalign(memptr, alignment, size);
		}
	};
}
