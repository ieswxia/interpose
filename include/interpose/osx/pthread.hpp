#include <signal.h>
#include <pthread.h>
#include <cstdio>
#include <interpose/core.hpp>

namespace interpose {
	/// Expose real pthread functions
	struct pthread_Root : public Root {
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
	
	// Redefine the Root macro to include pthread functions
	#undef Root
	#define Root pthread_Root
	
	/// Template to interpose on pthread functions
	template<class Super> struct pthread : public Super {
	private:
		static pthread<Super>*& instance() {
			static pthread<Super>* _instance = NULL;
			return _instance;
		}
	
	public:
		pthread() {
			if(instance() != NULL) {
				fprintf(stderr, "interpose error: multiple interpositions on pthread\n");
				abort();
			}
			
			instance() = this;
			
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
			return instance()->Super::pthread_self();
		}

		static int pthread_equal(pthread_t t1, pthread_t t2) {
			return instance()->Super::pthread_equal(t1, t2);
		}

		static int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*fn)(void*), void* arg) {
			return instance()->Super::pthread_create(thread, attr, fn, arg);
		}

		static int pthread_join(pthread_t thread, void** value_ptr) {
			return instance()->Super::pthread_join(thread, value_ptr);
		}

		static int pthread_detach(pthread_t thread) {
			return instance()->Super::pthread_detach(thread);
		}

		static int pthread_sigmask(int how, const sigset_t* set, sigset_t* oldset) {
			return instance()->Super::pthread_sigmask(how, set, oldset);
		}

		static int pthread_kill(pthread_t thread, int sig) {
			return instance()->Super::pthread_kill(thread, sig);
		}

		static int pthread_cancel(pthread_t thread) {
			return instance()->Super::pthread_cancel(thread);
		}

		static void pthread_exit(void* retval) {
			return instance()->Super::pthread_exit(retval);
		}

		static int pthread_setcancelstate(int state, int* oldstate) {
			return instance()->Super::pthread_setcancelstate(state, oldstate);
		}

		static int pthread_setcanceltype(int type, int* oldtype) {
			return instance()->Super::pthread_setcanceltype(type, oldtype);
		}

		static void pthread_testcancel() {
			instance()->Super::pthread_testcancel();
		}
	};
}
