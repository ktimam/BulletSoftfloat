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

/*
  Long Double expansions are
  Copyright (C) 2001 Stephen L. Moshier <moshier@na-net.ornl.gov>
  and are incorporated herein by permission of the author.  The author 
  reserves the right to distribute this material elsewhere under different
  copying permissions.  These modifications are distributed here under 
  the following terms:

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1l of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA */

/* __ieee754_asin(x)
 * Method :
 *	Since  asin(x) = x + x^3/6 + x^5*3/40 + x^7*15/336 + ...
 *	we approximate asin(x) on [0,0.5l] by
 *		asin(x) = x + x*x^2*R(x^2)
 *
 *	For x in [0.5l,1]
 *		asin(x) = pi/2-2*asin(sqrt((1-x)/2))
 *	Let y = (1-x), z = y/2, s := sqrt(z), and pio2_hi+pio2_lo=pi/2;
 *	then for x>0.98l
 *		asin(x) = pi/2 - 2*(s+s*z*R(z))
 *			= pio2_hi - (2*(s+s*z*R(z)) - pio2_lo)
 *	For x<=0.98l, let pio4_hi = pio2_hi/2, then
 *		f = hi part of s;
 *		c = sqrt(z) - f = (z-f*f)/(s+f) 	...f+c=sqrt(z)
 *	and
 *		asin(x) = pi/2 - 2*(s+s*z*R(z))
 *			= pio4_hi+(pio4-2s)-(2s*z*R(z)-pio2_lo)
 *			= pio4_hi+(pio4-2f)-(2s*z*R(z)-(pio2_lo+2c))
 *
 * Special cases:
 *	if x is NaN, return x itself;
 *	if |x|>1, return NaN with invalid signal.
 *
 */


#include "math.h"
#include "math_private.h"

namespace streflop_libm {
#ifdef __STDC__
static const Extended
#else
static Extended
#endif
  one = 1.0l,
  huge = 18446744073709551615,
 pio2_hi = 1.5707963267948966192021943710788178805159986950457096099853515625l,
  pio2_lo = 2.9127320560933561582586004641843300502121E-20l,
  pio4_hi = 7.8539816339744830960109718553940894025800E-1l,

	/* coefficient for R(x^2) */

  /* asin(x) = x + x^3 pS(x^2) / qS(x^2)
     0 <= x <= 0.5l
     peak relative error 1.9e-21l  */
  pS0 =  -1.008714657938491626019651170502036851607E1l,
  pS1 =   2.331460313214179572063441834101394865259E1l,
  pS2 =  -1.863169762159016144159202387315381830227E1l,
  pS3 =   5.930399351579141771077475766877674661747E0l,
  pS4 =  -6.121291917696920296944056882932695185001E-1l,
  pS5 =   3.776934006243367487161248678019350338383E-3l,

  qS0 =  -6.052287947630949712886794360635592886517E1l,
  qS1 =   1.671229145571899593737596543114258558503E2l,
  qS2 =  -1.707840117062586426144397688315411324388E2l,
  qS3 =   7.870295154902110425886636075950077640623E1l,
  qS4 =  -1.568433562487314651121702982333303458814E1l;
    /* 1.000000000000000000000000000000000000000E0l */

#ifdef __STDC__
Extended
__ieee754_asinl (Extended x)
#else
Extended
__ieee754_asinl (Extended x)
     //Extended x;
#endif
{
  Extended t, w, p, q, c, r, s;
  int32_t ix;
  u_int32_t se, i0, i1, k;

  GET_LDOUBLE_WORDS (se, i0, i1, x);
  ix = se & 0x7fff;
  ix = (ix << 16) | (i0 >> 16);
  if (ix >= 0x3fff8000)
    {				/* |x|>= 1 */
      if (ix == 0x3fff8000 && ((i0 - 0x80000000) | i1) == 0)
	/* asin(1)=+-pi/2 with inexact */
	return x * pio2_hi + x * pio2_lo;
      return (x - x) / (x - x);	/* asin(|x|>1) is NaN */
    }
  else if (ix < 0x3ffe8000)
    {				/* |x|<0.5l */
      if (ix < 0x3fde8000)
	{			/* if |x| < 2**-33 */
	  if (huge + x > one)
	    return x;		/* return x with inexact if x!=0 */
	}
      else
	{
	  t = x * x;
	  p =
	    t * (pS0 +
		 t * (pS1 + t * (pS2 + t * (pS3 + t * (pS4 + t * pS5)))));
	  q = qS0 + t * (qS1 + t * (qS2 + t * (qS3 + t * (qS4 + t))));
	  w = p / q;
	  return x + x * w;
	}
    }
  /* 1> |x|>= 0.5l */
  w = one - fabsl (x);
  t = w * 0.5l;
  p = t * (pS0 + t * (pS1 + t * (pS2 + t * (pS3 + t * (pS4 + t * pS5)))));
  q = qS0 + t * (qS1 + t * (qS2 + t * (qS3 + t * (qS4 + t))));
  s = __ieee754_sqrtl (t);
  if (ix >= 0x3ffef999)
    {				/* if |x| > 0.975l */
      w = p / q;
      t = pio2_hi - (2.0l * (s + s * w) - pio2_lo);
    }
  else
    {
      GET_LDOUBLE_WORDS (k, i0, i1, s);
      i1 = 0;
      SET_LDOUBLE_WORDS (w,k,i0,i1);
      c = (t - w * w) / (s + w);
      r = p / q;
      p = 2.0l * s * r - (pio2_lo - 2.0l * c);
      q = pio4_hi - 2.0l * w;
      t = pio4_hi - (p - q);
    }
  if ((se & 0x8000) == 0)
    return t;
  else
    return -t;
}
}
