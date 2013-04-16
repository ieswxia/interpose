#include <pthread.h>
#include <cstdio>
#include <interpose/core.hpp>

namespace interpose {
	/// Expose the real pthread condition variable functions
	struct pthread_cond_Root : public Root {
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
	
	// Redefine the Root macro to include the pthread condition variable functions
	#undef Root
	#define Root pthread_cond_Root
	
	/// Template to interpose on pthread condition variable functions
	template<class Super> struct pthread_cond : public Super {
	private:
		static pthread_cond<Super>*& instance() {
			static pthread_cond<Super>* _instance = NULL;
			return _instance;
		}
	
	public:
		pthread_cond() {
			if(instance() != NULL) {
				fprintf(stderr, "interpose error: multiple interpositions on pthread_cond\n");
				abort();
			}
			
			instance() = this;
			
			INTERPOSE(pthread_cond_init);
			INTERPOSE(pthread_cond_wait);
			INTERPOSE(pthread_cond_signal);
			INTERPOSE(pthread_cond_broadcast);
			INTERPOSE(pthread_cond_timedwait);
			INTERPOSE(pthread_cond_destroy);
		}
		
		static int pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t* attr) {
			return instance()->Super::pthread_cond_init(cond, attr);
		}

		static int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
			return instance()->Super::pthread_cond_wait(cond, mutex);
		}

		static int pthread_cond_signal(pthread_cond_t* cond) {
			return instance()->Super::pthread_cond_signal(cond);
		}

		static int pthread_cond_broadcast(pthread_cond_t* cond) {
			return instance()->Super::pthread_cond_broadcast(cond);
		}

		static int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime) {
			return instance()->Super::pthread_cond_timedwait(cond, mutex, abstime);
		}

		static int pthread_cond_destroy(pthread_cond_t* cond) {
			return instance()->Super::pthread_cond_destroy(cond);
		}
	};
}
