#include <debug.hpp>

#include <interpose.hpp>
#include <interpose/dl.hpp>
#include <interpose/pthread.hpp>

struct observer : public InterposeRoot {
	void* dlopen(const char* filename, int flag) {
		DEBUG(0, "caught dlopen");
		return InterposeRoot::dlopen(filename, flag);
	}
	
	pthread_t pthread_self() {
		return InterposeRoot::pthread_self();
	}
	
	int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*fn)(void*), void* arg) {
		DEBUG(0, "caught pthread_create");
		return InterposeRoot::pthread_create(thread, attr, fn, arg);
	}
	
	int pthread_kill(pthread_t thread, int sig) {
		DEBUG(0, "caught pthread_kill");
		return InterposeRoot::pthread_kill(thread, sig);
	}
	
	void pthread_exit(void* retval) {
		DEBUG(0, "caught pthread_exit");
		return InterposeRoot::pthread_exit(retval);
	}
};

Interpose<observer> host;
