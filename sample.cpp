#include <debug.hpp>

#include <interpose/dl.hpp>
#include <interpose/heap.hpp>
#include <interpose/proc.hpp>
#include <interpose/signal.hpp>
#include <interpose/pthread.hpp>
#include <interpose/pthread_mutex.hpp>
#include <interpose/pthread_cond.hpp>
#include <interpose/pthread_rwlock.hpp>

//#include <interpose/tools/spool.hpp>

using namespace interpose;

struct observer : public Root {
	void* dlopen(const char* filename, int flag) {
		DEBUG(0, "Caught dlopen!");
		return Root::dlopen(filename, flag);
	}
	
	void* malloc(size_t size) {
		//DEBUG(0, "Caught malloc!");
		return Root::malloc(size);
	}
};

dl<heap<observer> > theObserver;
