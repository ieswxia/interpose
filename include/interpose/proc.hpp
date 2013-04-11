#if !defined(INTERPOSE_PROC_HPP)
#define INTERPOSE_PROC_HPP

#include <unistd.h>
#include <stdlib.h>
#include "interpose.hpp"

struct interpose_proc_root : public InterposeRoot {
public:
	int fork() {
		return ::fork();
	}
	
	int execve(const char* filename, char* const argv[], char* const envp[]) {
		return ::execve(filename, argv, envp);
	}
	
	void exit(int status) {
		::exit(status);
	}
	
	void _exit(int status) {
		::_exit(status);
	}
	
	void _Exit(int status) {
		::_Exit(status);
	}
	
	void abort() {
		::abort();
	}
};

#undef InterposeRoot
#define InterposeRoot interpose_proc_root

template<class Interloper> struct interpose_proc_host : public Interpose<Interloper> {
	interpose_proc_host() {
		INTERPOSE(fork);
		INTERPOSE(execve);
		INTERPOSE(exit);
		INTERPOSE(_exit);
		INTERPOSE(_Exit);
	}
	
	static int fork() {
		return Interpose<Interloper>::instance()->fork();
	}
	
	static int execve(const char* filename, char* const argv[], char* const envp[]) {
		return Interpose<Interloper>::instance()->execve(filename, argv, envp);
	}
	
	static void exit(int status) {
		Interpose<Interloper>::instance()->exit(status);
	}
	
	static void _exit(int status) {
		Interpose<Interloper>::instance()->_exit(status);
	}
	
	static void _Exit(int status) {
		Interpose<Interloper>::instance()->_Exit(status);
	}
	
	static void abort() {
		Interpose<Interloper>::instance()->abort();
	}
};

#undef Interpose
#define Interpose interpose_proc_host

#endif
