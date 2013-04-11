#include <debug.hpp>

#include <interpose.hpp>
#include <interpose/dl.hpp>
#include <interpose/heap.hpp>
#include <interpose/proc.hpp>
#include <interpose/signal.hpp>
#include <interpose/pthread.hpp>

struct observer : public InterposeRoot {
	void* dlopen(const char* filename, int flag) {
		DEBUG(0, "caught dlopen");
		return InterposeRoot::dlopen(filename, flag);
	}
	
	int fork() {
		DEBUG(0, "caught fork");
		return InterposeRoot::fork();
	}
	
	int execve(const char* filename, char* const argv[], char* const envp[]) {
		DEBUG(0, "caught execve");
		return InterposeRoot::execve(filename, argv, envp);
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
