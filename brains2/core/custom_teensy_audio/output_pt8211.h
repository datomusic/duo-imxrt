/* Audio Library for Teensy 3.X
 * Copyright (c) 2016, Paul Stoffregen, paul@pjrc.com
 *
 * Development of this audio library was funded by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards.  Please support PJRC's efforts to develop
 * open source software by purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

//I2S adapted to PT8211, Frank Bösing.

#ifndef output_pt8211_h_
#define output_pt8211_h_

#include <Arduino.h>
#include <AudioStream.h>

#include "fsl_dmamux.h"
#include "fsl_sai_edma.h"
#include "fsl_iomuxc.h"


#if !defined(KINETISL)

	//uncomment to enable oversampling:
// #define AUDIO_PT8211_OVERSAMPLING
	//uncomment ONE of these to define interpolation type for oversampling:
// #define AUDIO_PT8211_INTERPOLATION_LINEAR
#define AUDIO_PT8211_INTERPOLATION_CIC


class AudioOutputPT8211 : public AudioStream
{
public:
	AudioOutputPT8211(void) : AudioStream(2, inputQueueArray) { }
	virtual void update(void);
	void begin(void);
	void stop(void);
protected:
	static void config_i2s(void);
	static audio_block_t *block_left_1st;
	static audio_block_t *block_right_1st;
	static bool update_responsibility;
	// static DMAChannel dma;
	// static void isr(void)
	static void isr(I2S_Type *base, sai_edma_handle_t *handle, status_t status, void *userData);
	#if defined(AUDIO_PT8211_OVERSAMPLING)
		__attribute__((optimize("unroll-loops")))
	#endif
	;
	static audio_block_t *block_left_2nd;
	static audio_block_t *block_right_2nd;
private:
	static uint16_t block_left_offset;
	static uint16_t block_right_offset;
	audio_block_t *inputQueueArray[2];
};


#elif defined(KINETISL)
/**************************************************************************************
*       Teensy LC
***************************************************************************************/
class AudioOutputPT8211 : public AudioStream
{
public:
	AudioOutputPT8211(void) : AudioStream(2, inputQueueArray) { begin(); }
	virtual void update(void);
	void begin(void);
	
protected:	
	static audio_block_t *block_left;
	static audio_block_t *block_right;
	static DMAChannel dma1;
	static DMAChannel dma2;
	audio_block_t *inputQueueArray[2];
	static void isr1(void);
	static void isr2(void);
	static bool update_responsibility;	
};

#endif

#endif
