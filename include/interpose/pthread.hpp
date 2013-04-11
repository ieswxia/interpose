#if !defined(INTERPOSE_PTHREAD_HPP)
#define INTERPOSE_PTHREAD_HPP

#include <signal.h>
#include <pthread.h>
#include "interpose.hpp"

struct interpose_pthread_root : public InterposeRoot {
public:
	pthread_t pthread_self() {
		return ::pthread_self();
	}
	
	int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*fn)(void*), void* arg) {
		return ::pthread_create(thread, attr, fn, arg);
	}
	
	int pthread_sigmask(int how, const sigset_t* set, sigset_t* oldset) {
		return ::pthread_sigmask(how, set, oldset);
	}
	
	int pthread_kill(pthread_t thread, int sig) {
		return ::pthread_kill(thread, sig);
	}
	
	int pthread_cancel(pthread_t thread) {
		return ::pthread_cancel(thread);
	}
	
	void pthread_exit(void* retval) {
		return ::pthread_exit(retval);
	}
};

#undef InterposeRoot
#define InterposeRoot interpose_pthread_root

template<class Interloper> struct interpose_pthread_host : public Interpose<Interloper> {
	interpose_pthread_host() {
		INTERPOSE(pthread_self);
		INTERPOSE(pthread_create);
		INTERPOSE(pthread_sigmask);
		INTERPOSE(pthread_kill);
		INTERPOSE(pthread_cancel);
		INTERPOSE(pthread_exit);
	}
	
	static pthread_t pthread_self() {
		return Interpose<Interloper>::instance()->pthread_self();
	}
	
	static int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*fn)(void*), void* arg) {
		return Interpose<Interloper>::instance()->pthread_create(thread, attr, fn, arg);
	}
	
	static int pthread_sigmask(int how, const sigset_t* set, sigset_t* oldset) {
		return Interpose<Interloper>::instance()->pthread_sigmask(how, set, oldset);
	}
	
	static int pthread_kill(pthread_t thread, int sig) {
		return Interpose<Interloper>::instance()->pthread_kill(thread, sig);
	}
	
	static int pthread_cancel(pthread_t thread) {
		return Interpose<Interloper>::instance()->pthread_cancel(thread);
	}
	
	static void pthread_exit(void* retval) {
		return Interpose<Interloper>::instance()->pthread_exit(retval);
	}
};

#undef Interpose
#define Interpose interpose_pthread_host

#endif
