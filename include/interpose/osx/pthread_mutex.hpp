#include <pthread.h>
#include <cstdio>
#include <interpose/core.hpp>

namespace interpose {
	/// Expose the real pthread mutex functions
	struct pthread_mutex_Root : public Root {
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
	
	// Redefine the Root macro to include the pthread mutex functions
	#undef Root
	#define Root pthread_mutex_Root
	
	/// Template to interpose on pthread_mutex functions
	template<class Super> struct pthread_mutex : public Super {
	private:
		static pthread_mutex<Super>*& instance() {
			static pthread_mutex<Super>* _instance = NULL;
			return _instance;
		}
	
	public:
		pthread_mutex() {
			if(instance() != NULL) {
				fprintf(stderr, "interpose error: multiple interpositions on pthread_mutex\n");
				abort();
			}
			
			instance() = this;
			
			INTERPOSE(pthread_mutex_init);
			INTERPOSE(pthread_mutex_lock);
			INTERPOSE(pthread_mutex_unlock);
			INTERPOSE(pthread_mutex_trylock);
			INTERPOSE(pthread_mutex_destroy);
		}
		
		static int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr) {
			return instance()->Super::pthread_mutex_init(mutex, attr);
		}

		static int pthread_mutex_lock(pthread_mutex_t* mutex) {
			return instance()->Super::pthread_mutex_lock(mutex);
		}

		static int pthread_mutex_unlock(pthread_mutex_t* mutex) {
			return instance()->Super::pthread_mutex_unlock(mutex);
		}

		static int pthread_mutex_trylock(pthread_mutex_t* mutex) {
			return instance()->Super::pthread_mutex_trylock(mutex);
		}

		static int pthread_mutex_destroy(pthread_mutex_t* mutex) {
			return instance()->Super::pthread_mutex_destroy(mutex);
		}
	};
}
