#include <debug.hpp>

#include <interpose.hpp>
#include <interpose/dl.hpp>
#include <interpose/heap.hpp>
#include <interpose/proc.hpp>
#include <interpose/signal.hpp>
#include <interpose/pthread.hpp>
#include <interpose/pthread_mutex.hpp>
#include <interpose/pthread_cond.hpp>
#include <interpose/pthread_rwlock.hpp>

#include <interpose/tools/spool.hpp>

struct observer : public InterposeRoot {
	void* dlopen(const char* filename, int flag) {
		DEBUG(0, "caught dlopen. %lu threads running", Spool::threadCount());
		return InterposeRoot::dlopen(filename, flag);
	}
	
	int fork() {
		DEBUG(0, "caught fork");
		return InterposeRoot::fork();
	}
	
	int execve(const char* filename, char* const argv[], char* const envp[]) {
		DEBUG(0, "caught execve");
		return InterposeRoot::execve(filename, argv, envp);
	}
};

Interpose<observer> host;
