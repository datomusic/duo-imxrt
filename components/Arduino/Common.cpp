/*
  Copyright (c) 2014 Arduino.  All right reserved.
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

extern "C" {
  #include "stdlib.h"
  #include "stdint.h"
}

#include "Common.h"

void randomSeed( uint32_t dwSeed )
{
  if ( dwSeed != 0 )
  {
    srand( dwSeed ) ;
  }
}

long random( long howbig )
{
  if ( howbig == 0 )
  {
    return 0 ;
  }

  return rand() % howbig;
}

long random( long howsmall, long howbig )
{
  if (howsmall >= howbig)
  {
    return howsmall;
  }

  long diff = howbig - howsmall;

  return random(diff) + howsmall;
}

// type_traits interferes with min() and other defines
// include it early, so we can define these later
// for Arduino compatibility
#ifdef __cplusplus
#include <type_traits>

// map() transforms input "x" from one numerical range to another.  For example, if
// you have analogInput() from 0 to 1023 and you want 5 to 25, use
// map(x, 0, 1023, 5, 25).  When "x" is an integer, the math is performed using
// integers and an integer number is returned.  When "x" is a floating point number,
// math is performed and result returned as floating point, to allow for fine grain
// mapping.
template <class T, class A, class B, class C, class D>
long map(T _x, A _in_min, B _in_max, C _out_min, D _out_max, typename std::enable_if<std::is_integral<T>::value >::type* = 0)
{
	// when the input number is an integer type, do all math as 32 bit signed long
	long x = _x, in_min = _in_min, in_max = _in_max, out_min = _out_min, out_max = _out_max;
	// Arduino's traditional algorithm
#if 0
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
#endif
#if 0
	// st42's suggestion: https://github.com/arduino/Arduino/issues/2466#issuecomment-69873889
	if ((in_max - in_min) > (out_max - out_min)) {
		return (x - in_min) * (out_max - out_min+1) / (in_max - in_min+1) + out_min;
	} else {
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
#endif
	// first compute the ranges and check if input doesn't matter
	long in_range = in_max - in_min;
	long out_range = out_max - out_min;
	if (in_range == 0) return out_min + out_range / 2;
	// compute the numerator
	long num = (x - in_min) * out_range;
	// before dividing, add extra for proper round off (towards zero)
	if (out_range >= 0) {
		num += in_range / 2;
	} else {
		num -= in_range / 2;
	}
	// divide by input range and add output offset to complete map() compute
	long result = num / in_range + out_min;
	// fix "a strange behaviour with negative numbers" (see ArduinoCore-API issue #51)
	//   this step can be deleted if you don't care about non-linear output
	//   behavior extrapolating slightly beyond the mapped input & output range
	if (out_range >= 0) {
		if (in_range * num < 0) return result - 1;
	} else {
		if (in_range * num >= 0) return result + 1;
	}
	return result;
	// more conversation:
	// https://forum.pjrc.com/threads/44503-map()-function-improvements
}
// map() transforms input "x" from one numerical range to another.  For example, if
// you have analogInput() from 0 to 1023 and you want 5 to 25, use
// map(x, 0, 1023, 5, 25).  When "x" is an integer, the math is performed using
// integers and an integer number is returned.  When "x" is a floating point number,
// math is performed and result returned as floating point, to allow for fine grain
// mapping.
template <class T, class A, class B, class C, class D>
T map(T x, A in_min, B in_max, C out_min, D out_max, typename std::enable_if<std::is_floating_point<T>::value >::type* = 0)
{
	// when the input is a float or double, do all math using the input's type
	return (x - (T)in_min) * ((T)out_max - (T)out_min) / ((T)in_max - (T)in_min) + (T)out_min;
}

uint16_t makeWord(uint16_t w) { return w; }
uint16_t makeWord(uint8_t h, uint8_t l) { return (h << 8) | l; }