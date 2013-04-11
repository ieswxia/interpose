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
	
	int pthread_equal(pthread_t t1, pthread_t t2) {
		return ::pthread_equal(t1, t2);
	}
	
	int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*fn)(void*), void* arg) {
		return ::pthread_create(thread, attr, fn, arg);
	}
	
	int pthread_join(pthread_t thread, void** value_ptr) {
		return ::pthread_join(thread, value_ptr);
	}
	
	int pthread_detach(pthread_t thread) {
		return ::pthread_detach(thread);
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
	
	int pthread_setcancelstate(int state, int* oldstate) {
		return ::pthread_setcancelstate(state, oldstate);
	}
	
	int pthread_setcanceltype(int type, int* oldtype) {
		return ::pthread_setcanceltype(type, oldtype);
	}
	
	void pthread_testcancel() {
		::pthread_testcancel();
	}
};

#undef InterposeRoot
#define InterposeRoot interpose_pthread_root

template<class Interloper> struct interpose_pthread_host : public Interpose<Interloper> {
	interpose_pthread_host() {
		INTERPOSE(pthread_self);
		INTERPOSE(pthread_equal);
		INTERPOSE(pthread_create);
		INTERPOSE(pthread_join);
		INTERPOSE(pthread_detach);
		INTERPOSE(pthread_sigmask);
		INTERPOSE(pthread_kill);
		INTERPOSE(pthread_cancel);
		INTERPOSE(pthread_exit);
		INTERPOSE(pthread_setcancelstate);
		INTERPOSE(pthread_setcanceltype);
		INTERPOSE(pthread_testcancel);
	}
	
	static pthread_t pthread_self() {
		return Interpose<Interloper>::instance()->pthread_self();
	}
	
	static int pthread_equal(pthread_t t1, pthread_t t2) {
		return Interpose<Interloper>::instance()->pthread_equal(t1, t2);
	}
	
	static int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*fn)(void*), void* arg) {
		return Interpose<Interloper>::instance()->pthread_create(thread, attr, fn, arg);
	}
	
	static int pthread_join(pthread_t thread, void** value_ptr) {
		return Interpose<Interloper>::instance()->pthread_join(thread, value_ptr);
	}
	
	static int pthread_detach(pthread_t thread) {
		return Interpose<Interloper>::instance()->pthread_detach(thread);
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
	
	static int pthread_setcancelstate(int state, int* oldstate) {
		return Interpose<Interloper>::instance()->pthread_setcancelstate(state, oldstate);
	}
	
	static int pthread_setcanceltype(int type, int* oldtype) {
		return Interpose<Interloper>::instance()->pthread_setcanceltype(type, oldtype);
	}
	
	static void pthread_testcancel() {
		Interpose<Interloper>::instance()->pthread_testcancel();
	}
};

#undef Interpose
#define Interpose interpose_pthread_host

#endif
