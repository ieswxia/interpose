#if !defined(INTERPOSE_PTHREAD_MUTEX_HPP)
#define INTERPOSE_PTHREAD_MUTEX_HPP

#include <pthread.h>
#include "interpose.hpp"

struct interpose_pthread_mutex_root : public InterposeRoot {
public:
	int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr) {
		return ::pthread_mutex_init(mutex, attr);
	}
	
	int pthread_mutex_lock(pthread_mutex_t* mutex) {
		return ::pthread_mutex_lock(mutex);
	}
	
	int pthread_mutex_unlock(pthread_mutex_t* mutex) {
		return ::pthread_mutex_unlock(mutex);
	}
	
	int pthread_mutex_trylock(pthread_mutex_t* mutex) {
		return ::pthread_mutex_trylock(mutex);
	}
	
	int pthread_mutex_destroy(pthread_mutex_t* mutex) {
		return ::pthread_mutex_destroy(mutex);
	}
};

#undef InterposeRoot
#define InterposeRoot interpose_pthread_mutex_root

template<class Interloper> struct interpose_pthread_mutex_host : public Interpose<Interloper> {
	interpose_pthread_mutex_host() {
		INTERPOSE(pthread_mutex_init);
		INTERPOSE(pthread_mutex_lock);
		INTERPOSE(pthread_mutex_unlock);
		INTERPOSE(pthread_mutex_trylock);
		INTERPOSE(pthread_mutex_destroy);
	}
	
	static int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr) {
		return Interpose<Interloper>::instance()->pthread_mutex_init(mutex, attr);
	}
	
	static int pthread_mutex_lock(pthread_mutex_t* mutex) {
		return Interpose<Interloper>::instance()->pthread_mutex_lock(mutex);
	}
	
	static int pthread_mutex_unlock(pthread_mutex_t* mutex) {
		return Interpose<Interloper>::instance()->pthread_mutex_unlock(mutex);
	}
	
	static int pthread_mutex_trylock(pthread_mutex_t* mutex) {
		return Interpose<Interloper>::instance()->pthread_mutex_trylock(mutex);
	}
	
	static int pthread_mutex_destroy(pthread_mutex_t* mutex) {
		return Interpose<Interloper>::instance()->pthread_mutex_destroy(mutex);
	}
};

#undef Interpose
#define Interpose interpose_pthread_mutex_host

#endif
