#if !defined(INTERPOSE_SIGNAL_HPP)
#define INTERPOSE_SIGNAL_HPP

#include <sys/types.h>
#include <signal.h>
#include "interpose.hpp"

typedef void (*_sighandler_t)(int);

struct interpose_signal_root : public InterposeRoot {
public:
	_sighandler_t signal(int signum, _sighandler_t handler) {
		return ::signal(signum, handler);
	}
	
	int sigaction(int signum, const struct sigaction* act, struct sigaction* oldact) {
		return ::sigaction(signum, act, oldact);
	}
	
	int raise(int sig) {
		return ::raise(sig);
	}
	
	int kill(pid_t pid, int sig) {
		return ::kill(pid, sig);
	}
	
	int sigpending(sigset_t* set) {
		return ::sigpending(set);
	}
	
	int sigprocmask(int how, const sigset_t* set, sigset_t* oldset) {
		return ::sigprocmask(how, set, oldset);
	}
	
	int sigsuspend(const sigset_t* mask) {
		return ::sigsuspend(mask);
	}
	
	int siginterrupt(int sig, int flag) {
		return ::siginterrupt(sig, flag);
	}
	
	int sigwait(const sigset_t* set, int* sig) {
		return ::sigwait(set, sig);
	}
};

#undef InterposeRoot
#define InterposeRoot interpose_signal_root

template<class Interloper> struct interpose_signal_host : public Interpose<Interloper> {
	interpose_signal_host() {
		INTERPOSE(signal);
		INTERPOSE(sigaction);
		INTERPOSE(raise);
		INTERPOSE(kill);
		INTERPOSE(sigpending);
		INTERPOSE(sigprocmask);
		INTERPOSE(sigsuspend);
		INTERPOSE(siginterrupt);
		INTERPOSE(sigwait);
	}
	
	static _sighandler_t signal(int signum, _sighandler_t handler) {
		return Interpose<Interloper>::instance()->signal(signum, handler);
	}
	
	static int sigaction(int signum, const struct sigaction* act, struct sigaction* oldact) {
		return Interpose<Interloper>::instance()->sigaction(signum, act, oldact);
	}
	
	static int raise(int sig) {
		return Interpose<Interloper>::instance()->raise(sig);
	}
	
	static int kill(pid_t pid, int sig) {
		return Interpose<Interloper>::instance()->kill(pid, sig);
	}
	
	static int sigpending(sigset_t* set) {
		return Interpose<Interloper>::instance()->sigpending(set);
	}
	
	static int sigprocmask(int how, const sigset_t* set, sigset_t* oldset) {
		return Interpose<Interloper>::instance()->sigprocmask(how, set, oldset);
	}
	
	static int sigsuspend(const sigset_t* mask) {
		return Interpose<Interloper>::instance()->sigsuspend(mask);
	}
	
	static int siginterrupt(int sig, int flag) {
		return Interpose<Interloper>::instance()->siginterrupt(sig, flag);
	}
	
	static int sigwait(const sigset_t* set, int* sig) {
		return Interpose<Interloper>::instance()->sigwait(set, sig);
	}
};

#undef Interpose
#define Interpose interpose_signal_host

#endif
