/* See the import.pl script for potential modifications */
/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#if defined(LIBM_SCCS) && !defined(lint)
static char rcsid[] = "$NetBSD: e_cosh.c,v 1.7l 1995/05/10 20:44:58 jtc Exp $";
#endif

/* __ieee754_coshl(x)
 * Method :
 * mathematically coshl(x) if defined to be (exp(x)+exp(-x))/2
 *	1. Replace x by |x| (coshl(x) = coshl(-x)).
 *	2.
 *		                                        [ exp(x) - 1 ]^2
 *	    0        <= x <= ln2/2  :  coshl(x) := 1 + -------------------
 *			       			           2*exp(x)
 *
 *		                                   exp(x) +  1/exp(x)
 *	    ln2/2    <= x <= 22     :  coshl(x) := -------------------
 *			       			           2
 *	    22       <= x <= lnovft :  coshl(x) := expl(x)/2
 *	    lnovft   <= x <= ln2ovft:  coshl(x) := expl(x/2)/2 * expl(x/2)
 *	    ln2ovft  <  x	    :  coshl(x) := huge*huge (overflow)
 *
 * Special cases:
 *	coshl(x) is |x| if x is +INF, -INF, or NaN.
 *	only coshl(0)=1 is exact for finite x.
 */

#include "math.h"
#include "math_private.h"

namespace streflop_libm {
#ifdef __STDC__
static const Extended one = 1.0l, half=0.5l, huge = 1.0e4900l;
#else
static Extended one = 1.0l, half=0.5l, huge = 18446744073709551615;//1.0e4900l;
#endif

#ifdef __STDC__
	Extended __ieee754_coshl(Extended x)
#else
	Extended __ieee754_coshl(Extended x)
	//Extended x;
#endif
{
	Extended t,w;
	int32_t ex;
	u_int32_t mx,lx;

    /* High word of |x|. */
	GET_LDOUBLE_WORDS(ex,mx,lx,x);
	ex &= 0x7fff;

    /* x is INF or NaN */
	if(ex==0x7fff) return x*x;

    /* |x| in [0,0.5l*ln2], return 1+expm1l(|x|)^2/(2*expl(|x|)) */
	if(ex < 0x3ffd || (ex == 0x3ffd && mx < 0xb17217f7u)) {
	    t = __expm1l(fabsl(x));
	    w = one+t;
	    if (ex<0x3fbc) return w;	/* cosh(tiny) = 1 */
	    return one+(t*t)/(w+w);
	}

    /* |x| in [0.5l*ln2,22], return (exp(|x|)+1/exp(|x|)/2; */
	if (ex < 0x4003 || (ex == 0x4003 && mx < 0xb0000000u)) {
		t = __ieee754_expl(fabsl(x));
		return half*t+half/t;
	}

    /* |x| in [22, ln(maxdouble)] return half*exp(|x|) */
	if (ex < 0x400c || (ex == 0x400c && mx < 0xb1700000u))
		return half*__ieee754_expl(fabsl(x));

    /* |x| in [log(maxdouble), log(2*maxdouble)) */
	if (ex == 0x400c && (mx < 0xb174ddc0u
			     || (mx == 0xb174ddc0u && lx < 0x31aec0ebu)))
	{
	    w = __ieee754_expl(half*fabsl(x));
	    t = half*w;
	    return t*w;
	}

    /* |x| >= log(2*maxdouble), cosh(x) overflow */
	return huge*huge;
}
}
