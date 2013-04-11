#if !defined(INTERPOSE_PTHREAD_RWLOCK_HPP)
#define INTERPOSE_PTHREAD_RWLOCK_HPP

#include <pthread.h>
#include "interpose.hpp"

struct interpose_pthread_rwlock_root : public InterposeRoot {
public:
	int pthread_rwlock_init(pthread_rwlock_t* rwlock, const pthread_rwlockattr_t* attr) {
		return ::pthread_rwlock_init(rwlock, attr);
	}
	
	int pthread_rwlock_rdlock(pthread_rwlock_t* rwlock) {
		return ::pthread_rwlock_rdlock(rwlock);
	}
	
	int pthread_rwlock_wrlock(pthread_rwlock_t* rwlock) {
		return ::pthread_rwlock_wrlock(rwlock);
	}
	
	int pthread_rwlock_unlock(pthread_rwlock_t* rwlock) {
		return ::pthread_rwlock_unlock(rwlock);
	}
	
	int pthread_rwlock_tryrdlock(pthread_rwlock_t* rwlock) {
		return ::pthread_rwlock_tryrdlock(rwlock);
	}
	
	int pthread_rwlock_trywrlock(pthread_rwlock_t* rwlock) {
		return ::pthread_rwlock_trywrlock(rwlock);
	}
	
	int pthread_rwlock_destroy(pthread_rwlock_t* rwlock) {
		return ::pthread_rwlock_destroy(rwlock);
	}
};

#undef InterposeRoot
#define InterposeRoot interpose_pthread_rwlock_root

template<class Interloper> struct interpose_pthread_rwlock_host : public Interpose<Interloper> {
	interpose_pthread_rwlock_host() {
		INTERPOSE(pthread_rwlock_init);
		INTERPOSE(pthread_rwlock_rdlock);
		INTERPOSE(pthread_rwlock_wrlock);
		INTERPOSE(pthread_rwlock_unlock);
		INTERPOSE(pthread_rwlock_tryrdlock);
		INTERPOSE(pthread_rwlock_trywrlock);
		INTERPOSE(pthread_rwlock_destroy);
	}
	
	static int pthread_rwlock_init(pthread_rwlock_t* rwlock, const pthread_rwlockattr_t* attr) {
		return Interpose<Interloper>::instance()->pthread_rwlock_init(rwlock, attr);
	}
	
	static int pthread_rwlock_rdlock(pthread_rwlock_t* rwlock) {
		return Interpose<Interloper>::instance()->pthread_rwlock_rdlock(rwlock);
	}
	
	static int pthread_rwlock_wrlock(pthread_rwlock_t* rwlock) {
		return Interpose<Interloper>::instance()->pthread_rwlock_wrlock(rwlock);
	}
	
	static int pthread_rwlock_unlock(pthread_rwlock_t* rwlock) {
		return Interpose<Interloper>::instance()->pthread_rwlock_unlock(rwlock);
	}
	
	static int pthread_rwlock_tryrdlock(pthread_rwlock_t* rwlock) {
		return Interpose<Interloper>::instance()->pthread_rwlock_tryrdlock(rwlock);
	}
	
	static int pthread_rwlock_trywrlock(pthread_rwlock_t* rwlock) {
		return Interpose<Interloper>::instance()->pthread_rwlock_trywrlock(rwlock);
	}
	
	static int pthread_rwlock_destroy(pthread_rwlock_t* rwlock) {
		return Interpose<Interloper>::instance()->pthread_rwlock_destroy(rwlock);
	}
};

#undef Interpose
#define Interpose interpose_pthread_rwlock_host

#endif
