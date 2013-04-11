#if !defined(INTERPOSE_HPP)
#define INTERPOSE_HPP

struct _interpose {
	void *new_func;
	void *orig_func;
};

#define INTERPOSE(fn) \
	static const _interpose _interpose_on_##fn __attribute__((used, section("__DATA, __interpose"))) = \
	{ (void*)(fn), (void*)(::fn) };

struct interpose_empty_root {};
#define InterposeRoot interpose_empty_root

template<class Interloper> struct interpose_empty_host : public Interloper {
protected:
	static interpose_empty_host*& instance() {
		static interpose_empty_host* _self = NULL;
		return _self;
	}

public:
	interpose_empty_host() {
		interpose_empty_host*& host = instance();
		if(host != NULL) {
			fprintf(stderr, "ERROR: Multiple interpose instances detected\n");
			abort();
		}
		host = this;
	}
};

#define Interpose interpose_empty_host

#endif
