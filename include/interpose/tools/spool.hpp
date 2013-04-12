#if !defined(INTERPOSE_TOOLS_SPOOL_HPP)
#define INTERPOSE_TOOLS_SPOOL_HPP

#include "../pthread.hpp"
#include "../proc.hpp"

#include <vector>

/**
 * Intercept thread creation and exit to track running threads.
 * It's called "spool" because it manages threads.  Get it?
 */
struct Spool : public InterposeRoot {
private:
	pthread_mutex_t _mutex;			//< Lock the thread list when modifying it
	std::vector<pthread_t> _threads;	//< The thread list
	
	/// The structure used to invoke thread functions inside a wrapper function
	struct _wrappedThread {
		void* (*fn)(void*);
		void* arg;
		Spool* instance;
	};
	
	/// Wrap thread invocation to catch thread returns (not the same as pthread_exit)
	static void* _wrapper(void* arg) {
		_wrappedThread* w = (_wrappedThread*)arg;
		
		// Get the thread handle
		pthread_t self = w->instance->pthread_self();
		
		// Add this thread to the thread list
		w->instance->addThread(self);
		
		// Run the real threa dfunction
		void* result = w->fn(w->arg);
		
		// Remove the thread
		w->instance->removeThread(self);
		
		// Free the argument wrapper object (allocated by the creating thread)
		delete w;
		
		return result;
	}
	
	/// Add a thread to the set
	void addThread(pthread_t thread) {
		lock();
		_threads.push_back(thread);
		unlock();
	}
	
	/// Remove a thread from the set
	void removeThread(pthread_t thread) {
		lock();
		for(std::vector<pthread_t>::iterator iter = _threads.begin(); iter != _threads.end(); iter++) {
			if(*iter == thread) {
				_threads.erase(iter);
				unlock();
				return;
			}
		}
		unlock();
	}
	
public:
	/// Initialize the thread tracker
	Spool() {
		// Set up a recursive lock
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
		
		// Initialize the lock
		pthread_mutex_init(&_mutex, &attr);
		
		// Destroy the mutex attr
		pthread_mutexattr_destroy(&attr);
		
		// Add the main thread
		addThread(InterposeRoot::pthread_self());
	}
	
	~Spool() {
		// Destroy the lock
		pthread_mutex_destroy(&_mutex);
	}
	
	void lock() {
		pthread_mutex_lock(&_mutex);
	}
	
	void unlock() {
		pthread_mutex_unlock(&_mutex);
	}
	
	/// Get the number of live threads
	size_t threadCount() {
		lock();
		size_t count = _threads.size();
		unlock();
		return count;
	}
	
	/// Get a thread at a specific index
	pthread_t getThread(size_t i) {
		lock();
		pthread_t t = _threads[i];
		unlock();
		return t;
	}
	
	/// Intercept calls to fork
	int fork() {
		// Do the fork
		int pid = InterposeRoot::fork();
		
		// The child process only has a main thread
		if(pid == 0) {
			// No need to lock.  There's only one thread in the child process
			_threads.clear();
			
			addThread(InterposeRoot::pthread_self());
		}
		
		return pid;
	}
	
	/// Intercept calls to pthread_create
	int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*fn)(void*), void* arg) {
		// Build an argument structure to pass the real thread function and arg to the wrapper
		_wrappedThread* w = new _wrappedThread();
		w->fn = fn;
		w->arg = arg;
		w->instance = this;
		
		// Create a thread with the wrapper function
		return InterposeRoot::pthread_create(thread, attr, _wrapper, w);
	}
	
	/// Intercept calls to pthread_exit
	void pthread_exit(void* retval) {
		removeThread(InterposeRoot::pthread_self());
		InterposeRoot::pthread_exit(retval);
	}
	
	/// Remove killed threads
	int pthread_kill(pthread_t thread, int signum) {
		if(signum == SIGKILL) {
			removeThread(thread);
		}
		
		return InterposeRoot::pthread_kill(thread, signum);
	}
	
	/// Remove cancelled threads
	int pthread_cancel(pthread_t thread) {
		removeThread(thread);
		return InterposeRoot::pthread_cancel(thread);
	}
};

#undef InterposeRoot
#define InterposeRoot Spool

#endif
