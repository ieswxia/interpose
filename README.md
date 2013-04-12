# Interpose
Interpose is a set of C++ header files for interposing on POSIX functions from a preloaded dynamic library.  The library allows you to intercept various subsets of POSIX functionality.  The list of supported functions is at the end of this document.

## Usage
The file ```sample.cpp``` shows how to interpose on several functions.  The example just prints a debug message and forwards the call along to the real POSIX function.

Every library that uses Interpose must include ```interpose.hpp```.  If you need to intercept pthread functions, include ```interpose/pthread.hpp```.  To intercept the dynamic linker functions, include ```interpose/dl.hpp```.

After including the required headers, you need to create a class or struct with the functions you want to intercept.  This struct should inherit from InterposeRoot, so any functions not trapped will be forwarded on to the POSIX implementation.  The following struct intercepts calls to ```dlopen```, reports the opened library, and forward the call on to POSIX:

	struct dl_logger : public InterposeBase {
		void* dlopen(const char* filename, int flag) {
			fprintf(stderr, "Opening %s\n", filename);
			
			// Pass the call on to POSIX
			return InterposeBase::dlopen(filename, flag));
		}
	};

Once the interposition code has been written, you must declare *exactly one* instance of the interposition code:

	Interpose<dl_logger> interposer; // Only one allowed!

This creates all the necessary interposition functions, and invokes all intercepted methods on the ```interposer``` object.

If you compile this code to ```dl_logger.dylib``` and run a program with the environment variable ```DYLD_INSERT_LIBRARIES=dl_logger.dylib```, this code will print a message every time a new dynamic library is opened.  The included Makefile includes several targets for testing against common applications.  Run ```make test-chrome``` to preload the sample library against Google Chrome.

## Tools
Interpose will include layers for implementing simple interposition-related tasks.  Right now, the only layer is ```Spool``` in ```interpose/tools/spool.hpp```.  The Spool layer intercepts thread creation and termination to keep a running list of all active threads in the application.  You can get the current count of threads with ```Spool::threadCount()```, and get a specific thread with ```Spool::getThread(size_t index)```.  The example code in ```simple.cpp``` includes the spool layer and prints a thread count on every ```dlopen``` call.

## Supported Functions
### dl
Include ```interpose/dl.hpp``` to enable interposition on these functions:

 - ```void* dlopen(const char* filename, int flag)```
 - ```char* dlerror()```
 - ```void* dlsym(void* handle, const char* symbol)```
 - ```int dlclose(void* handle)```

### heap
Include ```interpose/heap.hpp``` to enable interposition on these functions:

 - ```void* malloc(size_t size)```
 - ```void free(void* ptr)```
 - ```void* calloc(size_t nmemb, size_t size)```
 - ```void* realloc(void* ptr, size_t size)```
 - ```void* reallocf(void* ptr, size_t size)```
 - ```void* valloc(size_t size)```
 - ```int posix_memalign(void** memptr, size_t alignment, size_t size)```

### proc
Include ```interpose/proc.hpp``` to enable interposition on these functions:

 - ```int fork()```
 - ```int execve(const char* filename, char* const argv[], char* const envp[])```
 - ```void exit(int status)```
 - ```void _exit(int status)```
 - ```void _Exit(int status)```
 - ```void abort()```

### pthread
Include ```interpose/pthread.hpp``` to enable interposition on these functions:

 - ```pthread_t pthread_self()```
 - ```int pthread_create(pthread_t* thread, const pthread_attr_t* attr, void* (*fn)(void*), void* arg)```
 - ```int pthread_sigmask(int how, const sigset_t* set, sigset_t* oldset)```
 - ```int pthread_kill(pthread_t thread, int sig)```
 - ```int pthread_cancel(pthread_t thread)```
 - ```void pthread_exit(void* retval)```

### pthread_cond
Include ```interpose/pthread_cond.hpp``` to enable interposition on these functions:

 - ```int pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t* attr)```
 - ```int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex)```
 - ```int pthread_cond_signal(pthread_cond_t* cond)```
 - ```int pthread_cond_broadcast(pthread_cond_t* cond)```
 - ```int pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const struct timespec* abstime)```
 - ```int pthread_cond_destroy(pthread_cond_t* cond)```

### pthread_mutex
Include ```interpose/pthread_mutex.hpp``` to enable interposition on these functions:

 - ```int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr)```
 - ```int pthread_mutex_lock(pthread_mutex_t* mutex)```
 - ```int pthread_mutex_unlock(pthread_mutex_t* mutex)```
 - ```int pthread_mutex_trylock(pthread_mutex_t* mutex)```
 - ```int pthread_mutex_destroy(pthread_mutex_t* mutex)```

### pthread_rwlock
Include ```interpose/pthread_rwlock.hpp``` to enable interposition on these functions:

 - ```int pthread_rwlock_init(pthread_rwlock_t* rwlock, const pthread_rwlockattr_t* attr)```
 - ```int pthread_rwlock_rdlock(pthread_rwlock_t* rwlock)```
 - ```int pthread_rwlock_wrlock(pthread_rwlock_t* rwlock)```
 - ```int pthread_rwlock_unlock(pthread_rwlock_t* rwlock)```
 - ```int pthread_rwlock_tryrdlock(pthread_rwlock_t* rwlock)```
 - ```int pthread_rwlock_trywrlock(pthread_rwlock_t* rwlock)```
 - ```int pthread_rwlock_destroy(pthread_rwlock_t* rwlock)```

### signals
Include ```interpose/signal.hpp``` to enable interposition on these functions:

 - ```sighandler_t signal(int signum, sighandler_t handler)```
 - ```int sigaction(int signum, const struct sigaction* act, struct sigaction* oldact)```
 - ```int raise(int sig)```
 - ```int kill(pid_t pid, int sig)```
 - ```int sigpending(sigset_t* set)```
 - ```int sigprocmask(int how, const sigset_t* set, sigset_t* oldset)```
 - ```int sigsuspend(const sigset_t* mask)```
 - ```int siginterrupt(int sig, int flag)```
 - ```int sigwait(const sigset_t* set, int* sig)```