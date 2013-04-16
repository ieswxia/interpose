# Interpose
Interpose makes it easy to intercept POSIX calls in OSX applications (Linux support coming soon).  Interpose consists entirely of C++ header files.  The list of supported functions is at the end of this document.

## Usage
The file ```sample.cpp``` shows how to interpose on several functions.  The example just prints a debug message for a few functions, then forwards the call along to the real POSIX function.

POSIX functions are broken down into subsets, each with its own header file.  For example, process creation and termination functions can be intercepted by including ```interpose/proc.hpp```.  To interpose on pthread mutex operations, include ```interpose/pthread_mutex.hpp```.  The end of this document lists all supported functions.

After including the required headers, you need to create a class or struct with the functions you want to intercept.  This struct should inherit from ```Root``` in the ```interpose``` namespace, so any functions not trapped will be forwarded on to the POSIX implementation.  The following struct can intercept calls to ```dlopen```, reports the opened library, and forwards the call on to POSIX:

    using namespace interpose;

	struct DLLogger : public Root {
		void* dlopen(const char* filename, int flag) {
			fprintf(stderr, "Opening %s\n", filename);
			
			// Pass the call on to POSIX
			return Root::dlopen(filename, flag));
		}
	};

Once the interposition struct has been written, you need to create an instance of the interposition class:

	dl<DLLogger> theDLLogger;

You can interpose on multiple subsets by nesting the interposition templates:

    dl<proc<Logger> > theLogger;	// Don't forget the space between angle brackets for compatibility

You can also interpose on different subsets with separate objects:

    dl<DLLogger> theDLLogger;
    proc<ProcLogger> theProcLogger;

*You may only define one object that interposes on a given subset*.  It is not possible for two objects to interpose on the same function.  It doesn't matter if they are of the same type.  If you try to interpose on a subset multiple types, the interpose library will abort execution.  The following code *is an example of WHAT NOT TO DO*:

    dl<DLLogger> theDLLogger;
    proc<dl<POSIXLogger> > thePOSIXLogger;

Run ```make``` to compile the included interposition code in ```sample.cpp``` to a dynamic library.  The makefile also includes several targets for testing the sample interposition code against common applications.  Run ```make test-chrome``` to preload the sample library against Google Chrome in a new profile.  This target will not work if you installed Chrome somewhere other than the Applications directory.

## Tools
TBD

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
Include ```interpose/signals.hpp``` to enable interposition on these functions:

 - ```sighandler_t signal(int signum, sighandler_t handler)```
 - ```int sigaction(int signum, const struct sigaction* act, struct sigaction* oldact)```
 - ```int raise(int sig)```
 - ```int kill(pid_t pid, int sig)```
 - ```int sigpending(sigset_t* set)```
 - ```int sigprocmask(int how, const sigset_t* set, sigset_t* oldset)```
 - ```int sigsuspend(const sigset_t* mask)```
 - ```int siginterrupt(int sig, int flag)```
 - ```int sigwait(const sigset_t* set, int* sig)```
