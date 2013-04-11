#if !defined(INTERPOSE_PTHREAD_COND_HPP)
#define INTERPOSE_PTHREAD_COND_HPP

#include <pthread.h>
#include "interpose.hpp"

struct interpose_pthread_cond_root : public InterposeRoot {
public:
	int pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t* attr) {
		return ::pthread_cond_init(cond, attr);
	}
	
	int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
		return ::pthread_cond_wait(cond, mutex);
	}
	
	int pthread_cond_signal(pthread_cond_t* cond) {
		return ::pthread_cond_signal(cond);
	}
	
	int pthread_cond_broadcast(pthread_cond_t* cond) {
		return ::pthread_cond_broadcast(cond);
	}
	
	int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime) {
		return ::pthread_cond_timedwait(cond, mutex, abstime);
	}
	
	int pthread_cond_destroy(pthread_cond_t* cond) {
		return ::pthread_cond_destroy(cond);
	}
};

#undef InterposeRoot
#define InterposeRoot interpose_pthread_cond_root

template<class Interloper> struct interpose_pthread_cond_host : public Interpose<Interloper> {
	interpose_pthread_cond_host() {
		INTERPOSE(pthread_cond_init);
		INTERPOSE(pthread_cond_wait);
		INTERPOSE(pthread_cond_signal);
		INTERPOSE(pthread_cond_broadcast);
		INTERPOSE(pthread_cond_timedwait);
		INTERPOSE(pthread_cond_destroy);
	}
	
	static int pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t* attr) {
		return Interpose<Interloper>::instance()->pthread_cond_init(cond, attr);
	}
	
	static int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
		return Interpose<Interloper>::instance()->pthread_cond_wait(cond, mutex);
	}
	
	static int pthread_cond_signal(pthread_cond_t* cond) {
		return Interpose<Interloper>::instance()->pthread_cond_signal(cond);
	}
	
	static int pthread_cond_broadcast(pthread_cond_t* cond) {
		return Interpose<Interloper>::instance()->pthread_cond_broadcast(cond);
	}
	
	static int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime) {
		return Interpose<Interloper>::instance()->pthread_cond_timedwait(cond, mutex, abstime);
	}
	
	static int pthread_cond_destroy(pthread_cond_t* cond) {
		return Interpose<Interloper>::instance()->pthread_cond_destroy(cond);
	}
};

#undef Interpose
#define Interpose interpose_pthread_cond_host

#endif
