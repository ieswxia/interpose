#include <dlfcn.h>
#include <cstdio>
#include <interpose/core.hpp>

namespace interpose {
	/// Expose the real functions from dlfcn.h
	struct dl_Root : public Root {
		static void* dlopen(const char* filename, int flag) {
			return ::dlopen(filename, flag);
		}

		static char* dlerror() {
			return ::dlerror();
		}

		static void* dlsym(void* handle, const char* symbol) {
			return ::dlsym(handle, symbol);
		}

		static int dlclose(void* handle) {
			return ::dlclose(handle);
		}
	};
	
	// Redefine the Root macro to include the dl functions
	#undef Root
	#define Root dl_Root
	
	/// Template to interpose on dl functions
	template<class Super> struct dl : public Super {
	private:
		static dl<Super>*& instance() {
			static dl<Super>* _instance = NULL;
			return _instance;
		}
	
	public:
		dl() {
			if(instance() != NULL) {
				fprintf(stderr, "interpose error: multiple interpositions on dl\n");
				abort();
			}
			
			instance() = this;
			
			INTERPOSE(dlopen);
			INTERPOSE(dlerror);
			INTERPOSE(dlsym);
			INTERPOSE(dlclose);
		}
		
		static void* dlopen(const char* filename, int flag) {
			return instance()->Super::dlopen(filename, flag);
		}

		static char* dlerror() {
			return instance()->Super::dlerror();
		}

		static void* dlsym(void* handle, const char* symbol) {
			return instance()->Super::dlsym(handle, symbol);
		}

		static int dlclose(void* handle) {
			return instance()->Super::dlclose(handle);
		}
	};
}
