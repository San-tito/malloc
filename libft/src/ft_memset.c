#include "libft.h"

void *
ft_memset(void *dst, int c, size_t n)
{
	if (n != 0) {
		unsigned char *d = dst;

		do
			*d++ = (unsigned char)c;
		while (--n != 0);
	}
	return (dst);
}
