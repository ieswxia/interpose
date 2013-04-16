#if !defined(INTERPOSE_CORE_HPP)
#define INTERPOSE_CORE_HPP

#if defined(__APPLE__)
#include <interpose/osx/core.hpp>
#elif defined(__linux__)
#include <interpose/linux/core.hpp>
#endif

namespace interpose {
	/// Start with an empty Root type.  Specific interpositions will add to this
	/// type to provide access to the real, non-interposed POSIX functions
	struct core_Root {};
	
	/// Use a macro to expose the empty Root type
	#define Root core_Root
}

#endif
