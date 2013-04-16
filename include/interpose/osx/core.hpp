
/// Structure exposed to the linker for interposition
struct _interpose {
	void *new_func;
	void *orig_func;
};

/// Macro to instantiate an interposition structure
#define INTERPOSE(fn) \
	static const _interpose _interpose_on_##fn __attribute__((used, section("__DATA, __interpose"))) = \
	{ (void*)(fn), (void*)(::fn) };
