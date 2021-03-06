#include "os.h"
#include <libsec.h>

/*
 * timing safe jehanne_memcmp()
 */
int
tsmemcmp(void *a1, void *a2, uint32_t n)
{
	int lt, gt, c1, c2, r, m;
	uint8_t *s1, *s2;

	r = m = 0;
	s1 = a1;
	s2 = a2;
	while(n--){
		c1 = *s1++;
		c2 = *s2++;
		lt = (c1 - c2) >> 8;
		gt = (c2 - c1) >> 8;
		r |= (lt - gt) & ~m;
		m |= lt | gt;
	}
	return r;
}
