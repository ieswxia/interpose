#include <unistd.h>
#include <cstdio>
#include <interpose/core.hpp>

namespace interpose {
	/// Expose the real process control functions
	struct proc_Root : public Root {
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
	
	// Redefine the Root macro to include the process control functions
	#undef Root
	#define Root proc_Root
	
	/// Template to interpose on process control functions
	template<class Super> struct proc : public Super {
	private:
		static proc<Super>*& instance() {
			static proc<Super>* _instance = NULL;
			return _instance;
		}
	
	public:
		proc() {
			if(instance() != NULL) {
				fprintf(stderr, "interpose error: multiple interpositions on proc\n");
				abort();
			}
			
			instance() = this;
			
			INTERPOSE(fork);
			INTERPOSE(execve);
			INTERPOSE(exit);
			INTERPOSE(_exit);
			INTERPOSE(_Exit);
		}
		
		static int fork() {
			return instance()->Super::fork();
		}

		static int execve(const char* filename, char* const argv[], char* const envp[]) {
			return instance()->Super::execve(filename, argv, envp);
		}

		static void exit(int status) {
			instance()->Super::exit(status);
		}

		static void _exit(int status) {
			instance()->Super::_exit(status);
		}

		static void _Exit(int status) {
			instance()->Super::_Exit(status);
		}

		static void abort() {
			instance()->Super::abort();
		}
	};
}
