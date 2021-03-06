/*
 * This file is part of the UCB release of Plan 9. It is subject to the license
 * terms in the LICENSE file found in the top-level directory of this
 * distribution and at http://akaros.cs.berkeley.edu/files/Plan9License. No
 * part of the UCB release of Plan 9, including this file, may be copied,
 * modified, propagated, or distributed except according to the terms contained
 * in the LICENSE file.
 */

/*
	exp returns the exponential function of its
	floating-point argument.

	The coefficients are #1069 from Hart and Cheney. (22.35D)
*/

#include <u.h>
#include <libc.h>

#define	p0  .2080384346694663001443843411e7
#define	p1  .3028697169744036299076048876e5
#define	p2  .6061485330061080841615584556e2
#define	q0  .6002720360238832528230907598e7
#define	q1  .3277251518082914423057964422e6
#define	q2  .1749287689093076403844945335e4
#define	log2e  1.4426950408889634073599247
#define	sqrt2  1.4142135623730950488016887
#define	maxf  10000

double
jehanne_exp(double arg)
{
	double fract, temp1, temp2, xsq;
	int ent;

	if(arg == 0)
		return 1;
	if(arg < -maxf)
		return 0;
	if(arg > maxf)
		return jehanne_Inf(1);
	arg *= log2e;
	ent = jehanne_floor(arg);
	fract = (arg-ent) - 0.5;
	xsq = fract*fract;
	temp1 = ((p2*xsq+p1)*xsq+p0)*fract;
	temp2 = ((xsq+q2)*xsq+q1)*xsq + q0;
	return jehanne_ldexp(sqrt2*(temp2+temp1)/(temp2-temp1), ent);
}
