#pragma once

#include <new>
#include "defines.h"

#if __has_include(<common/config_common.h>)
#include <common/config_common.h>
#endif

#if USE_JEMALLOC
#include <jemalloc/jemalloc.h>

#if JEMALLOC_VERSION_MAJOR < 4
    #undef USE_JEMALLOC
    #define USE_JEMALLOC 0
    #include <cstdlib>
#endif
#else
#include <cstdlib>
#endif


namespace Memory
{

inline ALWAYS_INLINE void * newImpl(std::size_t size)
{
    auto * ptr = malloc(size);
    if (likely(ptr != nullptr))
        return ptr;

    /// @note no std::get_new_handler logic implemented
    throw std::bad_alloc{};
}

inline ALWAYS_INLINE void * newNoExept(std::size_t size) noexcept
{
    return malloc(size);
}

inline ALWAYS_INLINE void deleteImpl(void * ptr) noexcept
{
    free(ptr);
}

#if USE_JEMALLOC

inline ALWAYS_INLINE void deleteSized(void * ptr, std::size_t size) noexcept
{
    if (unlikely(ptr == nullptr))
        return;

    sdallocx(ptr, size, 0);
}

#else

inline ALWAYS_INLINE void deleteSized(void * ptr, std::size_t size [[maybe_unused]]) noexcept
{
    free(ptr);
}

#endif

}