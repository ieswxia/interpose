#include <pthread.h>
#include <cstdio>
#include <interpose/core.hpp>

namespace interpose {
	/// Expose the real pthread rwlock functions
	struct pthread_rwlock_Root : public Root {
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
	
	// Redefine the Root macro to include the pthread rwlock functions
	#undef Root
	#define Root pthread_rwlock_Root
	
	/// Template to interpose on the pthread rwlock functions
	template<class Super> struct pthread_rwlock : public Super {
	private:
		static pthread_rwlock<Super>*& instance() {
			static pthread_rwlock<Super>* _instance = NULL;
			return _instance;
		}
	
	public:
		pthread_rwlock() {
			if(instance() != NULL) {
				fprintf(stderr, "interpose error: multiple interpositions on pthread_rwlock\n");
				abort();
			}
			
			instance() = this;
			
			INTERPOSE(pthread_rwlock_init);
			INTERPOSE(pthread_rwlock_rdlock);
			INTERPOSE(pthread_rwlock_wrlock);
			INTERPOSE(pthread_rwlock_unlock);
			INTERPOSE(pthread_rwlock_tryrdlock);
			INTERPOSE(pthread_rwlock_trywrlock);
			INTERPOSE(pthread_rwlock_destroy);
		}
		
		static int pthread_rwlock_init(pthread_rwlock_t* rwlock, const pthread_rwlockattr_t* attr) {
			return instance()->Super::pthread_rwlock_init(rwlock, attr);
		}

		static int pthread_rwlock_rdlock(pthread_rwlock_t* rwlock) {
			return instance()->Super::pthread_rwlock_rdlock(rwlock);
		}

		static int pthread_rwlock_wrlock(pthread_rwlock_t* rwlock) {
			return instance()->Super::pthread_rwlock_wrlock(rwlock);
		}

		static int pthread_rwlock_unlock(pthread_rwlock_t* rwlock) {
			return instance()->Super::pthread_rwlock_unlock(rwlock);
		}

		static int pthread_rwlock_tryrdlock(pthread_rwlock_t* rwlock) {
			return instance()->Super::pthread_rwlock_tryrdlock(rwlock);
		}

		static int pthread_rwlock_trywrlock(pthread_rwlock_t* rwlock) {
			return instance()->Super::pthread_rwlock_trywrlock(rwlock);
		}

		static int pthread_rwlock_destroy(pthread_rwlock_t* rwlock) {
			return instance()->Super::pthread_rwlock_destroy(rwlock);
		}
	};
}
