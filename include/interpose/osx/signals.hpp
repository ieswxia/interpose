#include <sys/types.h>
#include <signal.h>
#include <cstdio>
#include <interpose/core.hpp>

namespace interpose {
	typedef void (*_sighandler_t)(int);
	
	/// Expose the real signal functions
	struct signals_Root : public Root {
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
	
	// Redefine the Root macro to include the signal functions
	#undef Root
	#define Root signals_Root
	
	/// Template to interpose on the signal functions
	template<class Super> struct signals : public Super {
	private:
		static signals<Super>*& instance() {
			static signals<Super>* _instance = NULL;
			return _instance;
		}
	
	public:
		signals() {
			if(instance() != NULL) {
				fprintf(stderr, "interpose error: multiple interpositions on signals\n");
				abort();
			}
			
			instance() = this;
			
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
			return instance()->Super::signal(signum, handler);
		}

		static int sigaction(int signum, const struct sigaction* act, struct sigaction* oldact) {
			return instance()->Super::sigaction(signum, act, oldact);
		}

		static int raise(int sig) {
			return instance()->Super::raise(sig);
		}

		static int kill(pid_t pid, int sig) {
			return instance()->Super::kill(pid, sig);
		}

		static int sigpending(sigset_t* set) {
			return instance()->Super::sigpending(set);
		}

		static int sigprocmask(int how, const sigset_t* set, sigset_t* oldset) {
			return instance()->Super::sigprocmask(how, set, oldset);
		}

		static int sigsuspend(const sigset_t* mask) {
			return instance()->Super::sigsuspend(mask);
		}

		static int siginterrupt(int sig, int flag) {
			return instance()->Super::siginterrupt(sig, flag);
		}

		static int sigwait(const sigset_t* set, int* sig) {
			return instance()->Super::sigwait(set, sig);
		}
	};
}
