
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "uh_types.h"
#include "uh_libc_mem.h"

uhos_void uhos_libc_free(uhos_void *ptr)
{
    free(ptr);
}

uhos_void *uhos_libc_malloc(uhos_size_t size)
{
    return malloc(size);
}

uhos_void *uhos_libc_zalloc(uhos_size_t size)
{
    return calloc(1, size);
}

uhos_void *uhos_libc_calloc(uhos_size_t nmemb, uhos_size_t size)
{
    return calloc(nmemb, size);
}

uhos_void *uhos_libc_realloc(uhos_void *ptr, uhos_size_t size)
{
    return realloc(ptr, size);
}

uhos_void *uhos_libc_memcpy(uhos_void *dest, const void *src, uhos_size_t n)
{
    return memcpy(dest, src, n);
}

uhos_void *uhos_libc_memmove(uhos_void *dest, const void *src, uhos_size_t n)
{
    return memmove(dest, src, n);
}

uhos_void *uhos_libc_memset(uhos_void *s, uhos_s32 c, uhos_size_t n)
{
    return memset(s, c, n);
}

uhos_s32 uhos_libc_memcmp(const uhos_void *s1, const uhos_void *s2, uhos_size_t n)
{
    return memcmp(s1, s2, n);
}
