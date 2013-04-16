#if !defined(INTERPOSE_HEAP_HPP)
#define INTERPOSE_HEAP_HPP

#if defined(__APPLE__)
#include <interpose/osx/heap.hpp>
#elif defined(__linux__)
#include <interpose/linux/heap.hpp>
#endif

#endif
