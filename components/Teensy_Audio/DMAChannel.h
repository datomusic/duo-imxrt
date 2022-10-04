/* Teensyduino Core Library
 * http://www.pjrc.com/teensy/
 * Copyright (c) 2017 PJRC.COM, LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * 2. If the Software is incorporated into a build system that allows
 * selection among a list of target devices, then similar target
 * devices manufactured by PJRC.COM must be included in the list of
 * target devices and selectable in the same manner.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef DMAChannel_h_
#define DMAChannel_h_

#include <stdint.h>
#include <MIMXRT1011.h>
#define DMA_BITER_ELINKYES_ELINK_MASK            (0x8000U)
#define DMA_TCD_CSR_INTMAJOR DMA_CSR_INTMAJOR_MASK
#define DMA_TCD_CSR_INTHALF DMA_CSR_INTHALF_MASK
#define DMA_TCD_CSR_DREQ DMA_CSR_DREQ_MASK
#define DMA_TCD_CSR_DONE DMA_CSR_DONE_MASK
#define DMA_TCD_CSR_ESG DMA_CSR_ESG_MASK
#define DMA_TCD_BITER_ELINKYES_LINKCH_MASK DMA_BITER_ELINKYES_LINKCH_MASK
#define DMA_TCD_BITER_ELINKYES_LINKCH DMA_BITER_ELINKYES_LINKCH
#define NVIC_NUM_INTERRUPTS NUMBER_OF_INT_VECTORS
#define IRQ_NUMBER_t IRQn_Type
#define __ARM_ARC_7EM__


#ifdef __cplusplus
extern "C" {
	__attribute__ ((used, aligned(1024), section(".ivt")))
	void (* _VectorsRam[NVIC_NUM_INTERRUPTS+16])(void);
}
#else
__attribute__ ((used, aligned(1024), section(".ivt")))
extern void (* _VectorsRam[NVIC_NUM_INTERRUPTS+16])(void);
#endif


// Discussion about DMAChannel is here:
// http://forum.pjrc.com/threads/25778-Could-there-be-something-like-an-ISR-template-function/page3

#define DMACHANNEL_HAS_BEGIN
#define DMACHANNEL_HAS_BOOLEAN_CTOR

// The channel allocation bitmask is accessible from "C" namespace,
// so C-only code can reserve DMA channels
#ifdef __cplusplus
extern "C" {
#endif
extern uint16_t dma_channel_allocated_mask;
#ifdef __cplusplus
}
#endif


#ifdef __cplusplus

// known libraries with DMA usage (in need of porting to this new scheme):
//
// https://github.com/PaulStoffregen/Audio
// https://github.com/PaulStoffregen/OctoWS2811
// https://github.com/pedvide/ADC
// https://github.com/duff2013/SerialEvent
// https://github.com/pixelmatix/SmartMatrix
// https://github.com/crteensy/DmaSpi <-- DmaSpi has adopted this scheme


class DMABaseClass {
public:
	typedef struct __attribute__((packed, aligned(4))) {
		volatile const void * volatile SADDR;
		int16_t SOFF;
		union { uint16_t ATTR;
			struct { uint8_t ATTR_DST; uint8_t ATTR_SRC; }; };
		union { uint32_t NBYTES; uint32_t NBYTES_MLNO;
			uint32_t NBYTES_MLOFFNO; uint32_t NBYTES_MLOFFYES; };
		int32_t SLAST;
		volatile void * volatile DADDR;
		int16_t DOFF;
		union { volatile uint16_t CITER;
			volatile uint16_t CITER_ELINKYES; volatile uint16_t CITER_ELINKNO; };
		int32_t DLASTSGA;
		volatile uint16_t CSR;
		union { volatile uint16_t BITER;
			volatile uint16_t BITER_ELINKYES; volatile uint16_t BITER_ELINKNO; };
	} TCD_t;
	TCD_t *TCD;

	/***************************************/
	/**    Data Transfer                  **/
	/***************************************/

	// Use a single variable as the data source.  Typically a register
	// for receiving data from one of the hardware peripherals is used.
	void source(volatile const signed char &p) { source(*(volatile const uint8_t *)&p); }
	void source(volatile const unsigned char &p) {
		TCD->SADDR = &p;
		TCD->SOFF = 0;
		TCD->ATTR_SRC = 0;
		if ((uint32_t)&p < 0x40000000 || TCD->NBYTES == 0) TCD->NBYTES = 1;
		TCD->SLAST = 0;
	}
	void source(volatile const signed short &p) { source(*(volatile const uint16_t *)&p); }
	void source(volatile const unsigned short &p) {
		TCD->SADDR = &p;
		TCD->SOFF = 0;
		TCD->ATTR_SRC = 1;
		if ((uint32_t)&p < 0x40000000 || TCD->NBYTES == 0) TCD->NBYTES = 2;
		TCD->SLAST = 0;
	}
	void source(volatile const signed int &p) { source(*(volatile const uint32_t *)&p); }
	void source(volatile const unsigned int &p) { source(*(volatile const uint32_t *)&p); }
	void source(volatile const signed long &p) { source(*(volatile const uint32_t *)&p); }
	void source(volatile const unsigned long &p) {
		TCD->SADDR = &p;
		TCD->SOFF = 0;
		TCD->ATTR_SRC = 2;
		if ((uint32_t)&p < 0x40000000 || TCD->NBYTES == 0) TCD->NBYTES = 4;
		TCD->SLAST = 0;
	}

	// Use a buffer (array of data) as the data source.  Typically a
	// buffer for transmitting data is used.
	void sourceBuffer(volatile const signed char p[], unsigned int len) {
		sourceBuffer((volatile const uint8_t *)p, len); }
	void sourceBuffer(volatile const unsigned char p[], unsigned int len) {
		TCD->SADDR = p;
		TCD->SOFF = 1;
		TCD->ATTR_SRC = 0;
		TCD->NBYTES = 1;
		TCD->SLAST = -len;
		TCD->BITER = len;
		TCD->CITER = len;
	}
	void sourceBuffer(volatile const signed short p[], unsigned int len) {
		sourceBuffer((volatile const uint16_t *)p, len); }
	void sourceBuffer(volatile const unsigned short p[], unsigned int len) {
		TCD->SADDR = p;
		TCD->SOFF = 2;
		TCD->ATTR_SRC = 1;
		TCD->NBYTES = 2;
		TCD->SLAST = -len;
		TCD->BITER = len / 2;
		TCD->CITER = len / 2;
	}
	void sourceBuffer(volatile const signed int p[], unsigned int len) {
		sourceBuffer((volatile const uint32_t *)p, len); }
	void sourceBuffer(volatile const unsigned int p[], unsigned int len) {
		sourceBuffer((volatile const uint32_t *)p, len); }
	void sourceBuffer(volatile const signed long p[], unsigned int len) {
		sourceBuffer((volatile const uint32_t *)p, len); }
	void sourceBuffer(volatile const unsigned long p[], unsigned int len) {
		TCD->SADDR = p;
		TCD->SOFF = 4;
		TCD->ATTR_SRC = 2;
		TCD->NBYTES = 4;
		TCD->SLAST = -len;
		TCD->BITER = len / 4;
		TCD->CITER = len / 4;
	}

	// Use a circular buffer as the data source
	void sourceCircular(volatile const signed char p[], unsigned int len) {
		sourceCircular((volatile const uint8_t *)p, len); }
	void sourceCircular(volatile const unsigned char p[], unsigned int len) {
		TCD->SADDR = p;
		TCD->SOFF = 1;
		TCD->ATTR_SRC = ((31 - __builtin_clz(len)) << 3);
		TCD->NBYTES = 1;
		TCD->SLAST = 0;
		TCD->BITER = len;
		TCD->CITER = len;
	}
	void sourceCircular(volatile const signed short p[], unsigned int len) {
		sourceCircular((volatile const uint16_t *)p, len); }
	void sourceCircular(volatile const unsigned short p[], unsigned int len) {
		TCD->SADDR = p;
		TCD->SOFF = 2;
		TCD->ATTR_SRC = ((31 - __builtin_clz(len)) << 3) | 1;
		TCD->NBYTES = 2;
		TCD->SLAST = 0;
		TCD->BITER = len / 2;
		TCD->CITER = len / 2;
	}
	void sourceCircular(volatile const signed int p[], unsigned int len) {
		sourceCircular((volatile const uint32_t *)p, len); }
	void sourceCircular(volatile const unsigned int p[], unsigned int len) {
		sourceCircular((volatile const uint32_t *)p, len); }
	void sourceCircular(volatile const signed long p[], unsigned int len) {
		sourceCircular((volatile const uint32_t *)p, len); }
	void sourceCircular(volatile const unsigned long p[], unsigned int len) {
		TCD->SADDR = p;
		TCD->SOFF = 4;
		TCD->ATTR_SRC = ((31 - __builtin_clz(len)) << 3) | 2;
		TCD->NBYTES = 4;
		TCD->SLAST = 0;
		TCD->BITER = len / 4;
		TCD->CITER = len / 4;
	}

	// Use a single variable as the data destination.  Typically a register
	// for transmitting data to one of the hardware peripherals is used.
	void destination(volatile signed char &p) { destination(*(volatile uint8_t *)&p); }
	void destination(volatile unsigned char &p) {
		TCD->DADDR = &p;
		TCD->DOFF = 0;
		TCD->ATTR_DST = 0;
		if ((uint32_t)&p < 0x40000000 || TCD->NBYTES == 0) TCD->NBYTES = 1;
		TCD->DLASTSGA = 0;
	}
	void destination(volatile signed short &p) { destination(*(volatile uint16_t *)&p); }
	void destination(volatile unsigned short &p) {
		TCD->DADDR = &p;
		TCD->DOFF = 0;
		TCD->ATTR_DST = 1;
		if ((uint32_t)&p < 0x40000000 || TCD->NBYTES == 0) TCD->NBYTES = 2;
		TCD->DLASTSGA = 0;
	}
	void destination(volatile signed int &p) { destination(*(volatile uint32_t *)&p); }
	void destination(volatile unsigned int &p) { destination(*(volatile uint32_t *)&p); }
	void destination(volatile signed long &p) { destination(*(volatile uint32_t *)&p); }
	void destination(volatile unsigned long &p) {
		TCD->DADDR = &p;
		TCD->DOFF = 0;
		TCD->ATTR_DST = 2;
		if ((uint32_t)&p < 0x40000000 || TCD->NBYTES == 0) TCD->NBYTES = 4;
		TCD->DLASTSGA = 0;
	}

	// Use a buffer (array of data) as the data destination.  Typically a
	// buffer for receiving data is used.
	void destinationBuffer(volatile signed char p[], unsigned int len) {
		destinationBuffer((volatile uint8_t *)p, len); }
	void destinationBuffer(volatile unsigned char p[], unsigned int len) {
		TCD->DADDR = p;
		TCD->DOFF = 1;
		TCD->ATTR_DST = 0;
		TCD->NBYTES = 1;
		TCD->DLASTSGA = -len;
		TCD->BITER = len;
		TCD->CITER = len;
	}
	void destinationBuffer(volatile signed short p[], unsigned int len) {
		destinationBuffer((volatile uint16_t *)p, len); }
	void destinationBuffer(volatile unsigned short p[], unsigned int len) {
		TCD->DADDR = p;
		TCD->DOFF = 2;
		TCD->ATTR_DST = 1;
		TCD->NBYTES = 2;
		TCD->DLASTSGA = -len;
		TCD->BITER = len / 2;
		TCD->CITER = len / 2;
	}
	void destinationBuffer(volatile signed int p[], unsigned int len) {
		destinationBuffer((volatile uint32_t *)p, len); }
	void destinationBuffer(volatile unsigned int p[], unsigned int len) {
		destinationBuffer((volatile uint32_t *)p, len); }
	void destinationBuffer(volatile signed long p[], unsigned int len) {
		destinationBuffer((volatile uint32_t *)p, len); }
	void destinationBuffer(volatile unsigned long p[], unsigned int len) {
		TCD->DADDR = p;
		TCD->DOFF = 4;
		TCD->ATTR_DST = 2;
		TCD->NBYTES = 4;
		TCD->DLASTSGA = -len;
		TCD->BITER = len / 4;
		TCD->CITER = len / 4;
	}

	// Use a circular buffer as the data destination
	void destinationCircular(volatile signed char p[], unsigned int len) {
		destinationCircular((volatile uint8_t *)p, len); }
	void destinationCircular(volatile unsigned char p[], unsigned int len) {
		TCD->DADDR = p;
		TCD->DOFF = 1;
		TCD->ATTR_DST = ((31 - __builtin_clz(len)) << 3);
		TCD->NBYTES = 1;
		TCD->DLASTSGA = 0;
		TCD->BITER = len;
		TCD->CITER = len;
	}
	void destinationCircular(volatile signed short p[], unsigned int len) {
		destinationCircular((volatile uint16_t *)p, len); }
	void destinationCircular(volatile unsigned short p[], unsigned int len) {
		TCD->DADDR = p;
		TCD->DOFF = 2;
		TCD->ATTR_DST = ((31 - __builtin_clz(len)) << 3) | 1;
		TCD->NBYTES = 2;
		TCD->DLASTSGA = 0;
		TCD->BITER = len / 2;
		TCD->CITER = len / 2;
	}
	void destinationCircular(volatile signed int p[], unsigned int len) {
		destinationCircular((volatile uint32_t *)p, len); }
	void destinationCircular(volatile unsigned int p[], unsigned int len) {
		destinationCircular((volatile uint32_t *)p, len); }
	void destinationCircular(volatile signed long p[], unsigned int len) {
		destinationCircular((volatile uint32_t *)p, len); }
	void destinationCircular(volatile unsigned long p[], unsigned int len) {
		TCD->DADDR = p;
		TCD->DOFF = 4;
		TCD->ATTR_DST = ((31 - __builtin_clz(len)) << 3) | 2;
		TCD->NBYTES = 4;
		TCD->DLASTSGA = 0;
		TCD->BITER = len / 4;
		TCD->CITER = len / 4;
	}

	/*************************************************/
	/**    Quantity of Data to Transfer             **/
	/*************************************************/

	// Set the data size used for each triggered transfer
	void transferSize(unsigned int len) {
		if (len == 16) {
			TCD->NBYTES = 16;
			if (TCD->SOFF != 0) TCD->SOFF = 16;
			if (TCD->DOFF != 0) TCD->DOFF = 16;
			TCD->ATTR = (TCD->ATTR & 0xF8F8) | 0x0404;
		} else if (len == 4) {
			TCD->NBYTES = 4;
			if (TCD->SOFF != 0) TCD->SOFF = 4;
			if (TCD->DOFF != 0) TCD->DOFF = 4;
			TCD->ATTR = (TCD->ATTR & 0xF8F8) | 0x0202;
		} else if (len == 2) {
			TCD->NBYTES = 2;
			if (TCD->SOFF != 0) TCD->SOFF = 2;
			if (TCD->DOFF != 0) TCD->DOFF = 2;
			TCD->ATTR = (TCD->ATTR & 0xF8F8) | 0x0101;
		} else {
			TCD->NBYTES = 1;
			if (TCD->SOFF != 0) TCD->SOFF = 1;
			if (TCD->DOFF != 0) TCD->DOFF = 1;
			TCD->ATTR = TCD->ATTR & 0xF8F8;
		}
	}

	// Set the number of transfers (number of triggers until complete)
	void transferCount(unsigned int len) {
		if (!(TCD->BITER & DMA_BITER_ELINKYES_ELINK_MASK)) {
			if (len > 32767) return;
			TCD->BITER = len;
			TCD->CITER = len;
		} else {
			if (len > 511) return;
			TCD->BITER = (TCD->BITER & 0xFE00) | len;
			TCD->CITER = (TCD->CITER & 0xFE00) | len;
		}
	}

	/*************************************************/
	/**    Special Options / Features               **/
	/*************************************************/

	void interruptAtCompletion(void) {
		TCD->CSR |= DMA_TCD_CSR_INTMAJOR;
	}

	void interruptAtHalf(void) {
		TCD->CSR |= DMA_TCD_CSR_INTHALF;
	}

	void disableOnCompletion(void) {
		TCD->CSR |= DMA_TCD_CSR_DREQ;
	}

	void replaceSettingsOnCompletion(const DMABaseClass &settings) {
		TCD->DLASTSGA = (int32_t)(settings.TCD);
		TCD->CSR &= ~DMA_TCD_CSR_DONE;
		TCD->CSR |= DMA_TCD_CSR_ESG;
	}

protected:
	// users should not be able to create instances of DMABaseClass, which
	// require the inheriting class to initialize the TCD pointer.
	DMABaseClass() {}

	static inline void copy_tcd(TCD_t *dst, const TCD_t *src) {
		dst->CSR &= ~DMA_TCD_CSR_DONE;
		const uint32_t *p = (const uint32_t *)src;
		uint32_t *q = (uint32_t *)dst;
		uint32_t t1, t2, t3, t4;
		t1 = *p++; t2 = *p++; t3 = *p++; t4 = *p++;
		*q++ = t1; *q++ = t2; *q++ = t3; *q++ = t4;
		t1 = *p++; t2 = *p++; t3 = *p++; t4 = *p++;
		*q++ = t1; *q++ = t2; *q++ = t3; *q++ = t4;
	}
};


// DMASetting represents settings stored only in memory, which can be
// applied to any DMA channel.

class DMASetting : public DMABaseClass {
public:
	DMASetting() {
		TCD = &tcddata;
	}
	DMASetting(const DMASetting &c) {
		TCD = &tcddata;
		*this = c;
	}
	DMASetting(const DMABaseClass &c) {
		TCD = &tcddata;
		*this = c;
	}
	DMASetting & operator = (const DMABaseClass &rhs) {
		copy_tcd(TCD, rhs.TCD);
		return *this;
	}
private:
	TCD_t tcddata __attribute__((aligned(32)));
};


// DMAChannel reprents an actual DMA channel and its current settings

class DMAChannel : public DMABaseClass {
public:
	/*************************************************/
	/**    Channel Allocation                       **/
	/*************************************************/

	DMAChannel() {
		begin();
	}
	DMAChannel(const DMAChannel &c) {
		TCD = c.TCD;
		channel = c.channel;
	}
	DMAChannel(const DMASetting &c) {
		begin();
		copy_tcd(TCD, c.TCD);
	}
	DMAChannel(bool allocate) {
		if (allocate) begin();
	}
	DMAChannel & operator = (const DMAChannel &rhs) {
		if (channel != rhs.channel) {
			release();
			TCD = rhs.TCD;
			channel = rhs.channel;
		}
		return *this;
	}
	DMAChannel & operator = (const DMASetting &rhs) {
		copy_tcd(TCD, rhs.TCD);
		return *this;
	}
	~DMAChannel() {
		release();
	}
	void begin(bool force_initialization = false);
private:
	void release(void);

public:
	/***************************************/
	/**    Triggering                     **/
	/***************************************/

	// Triggers cause the DMA channel to actually move data.  Each
	// trigger moves a single data unit, which is typically 8, 16 or
	// 32 bits.  If a channel is configured for 200 transfers

	// Use a hardware trigger to make the DMA channel run
	void triggerAtHardwareEvent(uint8_t source) {
		//volatile uint32_t *mux = &DMAMUX_CHCFG0 + channel;
		volatile uint32_t *mux = (volatile uint32_t *)DMAMUX_BASE + channel;
		//mux = (volatile uint32_t *)&(DMAMUX_CHCFG0) + channel;
		*mux = 0;
		*mux = (source & 0x7F) | DMAMUX_CHCFG_ENBL_MASK;
	}

	// Use another DMA channel as the trigger, causing this
	// channel to trigger after each transfer is makes, except
	// the its last transfer.  This effectively makes the 2
	// channels run in parallel until the last transfer
	void triggerAtTransfersOf(DMABaseClass &ch) {
		ch.TCD->BITER = (ch.TCD->BITER & ~DMA_TCD_BITER_ELINKYES_LINKCH_MASK)
		  | DMA_TCD_BITER_ELINKYES_LINKCH(channel) | DMA_BITER_ELINKYES_ELINK_MASK;
		ch.TCD->CITER = ch.TCD->BITER ;
	}

	// Use another DMA channel as the trigger, causing this
	// channel to trigger when the other channel completes.
	void triggerAtCompletionOf(DMABaseClass &ch) {
		ch.TCD->CSR = (ch.TCD->CSR & ~(DMA_CSR_MAJORLINKCH_MASK|DMA_TCD_CSR_DONE))
		  | DMA_CSR_MAJORLINKCH(channel) | DMA_CSR_MAJORELINK_MASK;
	}

	// Cause this DMA channel to be continuously triggered, so
	// it will move data as rapidly as possible, without waiting.
	// Normally this would be used with disableOnCompletion().
	void triggerContinuously(void) {
		volatile uint32_t *mux = (volatile uint32_t *)DMAMUX_BASE + channel;
		*mux = 0;
		*mux = DMAMUX_CHCFG_A_ON_SHIFT | DMAMUX_CHCFG_ENBL_MASK;
	}

	// Manually trigger the DMA channel.
	void triggerManual(void) {
		// DMA_SSRT = channel;
		DMA_Type * dma_channel = (DMA_Type *)((uint8_t) DMA0_BASE + channel);
		dma_channel->SSRT = 1;
	}


	/***************************************/
	/**    Interrupts                     **/
	/***************************************/

	// An interrupt routine can be run when the DMA channel completes
	// the entire transfer, and also optionally when half of the
	// transfer is completed.
	void attachInterrupt(void (*isr)(void)) {
		// _VectorsRam[channel + IRQ_DMA_CH0 + 16] = isr;
	    _VectorsRam[((uint8_t)DMA0_IRQn + channel) + 16] = isr;
		NVIC_EnableIRQ((IRQn_Type) ((uint8_t)DMA0_IRQn + channel));
	}

	void attachInterrupt(void (*isr)(void), uint8_t prio) {
		// _VectorsRam[channel + IRQ_DMA_CH0 + 16] = isr;
		_VectorsRam[((uint8_t)DMA0_IRQn + channel) + 16] = isr;
		// NVIC_ENABLE_IRQ(IRQ_DMA_CH0 + channel);
		NVIC_EnableIRQ((IRQn_Type) ((uint8_t)DMA0_IRQn + channel));
		// NVIC_SET_PRIORITY(IRQ_DMA_CH0 + channel, prio);
		NVIC_SetPriority((IRQn_Type) ((uint8_t)DMA0_IRQn + channel), prio);
	}
	
	void detachInterrupt(void) {
		// NVIC_DISABLE_IRQ(IRQ_DMA_CH0 + channel);
		NVIC_DisableIRQ((IRQn_Type) ((uint8_t)DMA0_IRQn + channel));
	}

	void clearInterrupt(void) {
		// DMA_CINT = channel;
		NVIC_ClearPendingIRQ((IRQn_Type) ((uint8_t)DMA0_IRQn + channel));
	}


	/***************************************/
	/**    Enable / Disable               **/
	/***************************************/

	void enable(void) {
		// DMA_SERQ = channel;
		DMA_Type * dma_channel = (DMA_Type *)((uint8_t) DMA0_BASE + channel);
		dma_channel->SERQ = 1;
	}
	void disable(void) {
		// DMA_CERQ = channel; // Clear enable request
		DMA_Type * dma_channel = (DMA_Type *)((uint8_t) DMA0_BASE + channel);
		dma_channel->SERQ = 1;
	}

	/***************************************/
	/**    Status                         **/
	/***************************************/

	bool complete(void) {
		if (TCD->CSR & DMA_TCD_CSR_DONE) return true;
		return false;
	}
	void clearComplete(void) {
		// DMA_CDNE = channel;
		DMA_Type * dma_channel = (DMA_Type *)((uint8_t) DMA0_BASE + channel);
		dma_channel->CDNE = 1;
	}
	bool error(void) {
		// if (DMA_ERR & (1<<channel)) return true;
		DMA_Type * dma_channel = (DMA_Type *)((uint8_t) DMA0_BASE + channel);
	    if (dma_channel->ERR) return true;
		return false;
	}
	void clearError(void) {
		// DMA_CERR = channel;
		DMA_Type * dma_channel = (DMA_Type *)((uint8_t) DMA0_BASE + channel);
		dma_channel->CERR = 1;
	}
	void * sourceAddress(void) {
		return (void *)(TCD->SADDR);
	}
	void * destinationAddress(void) {
		return (void *)(TCD->DADDR);
	}

	/***************************************/
	/**    Direct Hardware Access         **/
	/***************************************/

	// For complex and unusual configurations not possible with the above
	// functions, the Transfer Control Descriptor (TCD) and channel number
	// can be used directly.  This leads to less portable and less readable
	// code, but direct control of all parameters is possible.
	uint8_t channel;
	// TCD is accessible due to inheritance from DMABaseClass
	DMA_Type * base;
};

// arrange the relative priority of 2 or more DMA channels
void DMAPriorityOrder(DMAChannel &ch1, DMAChannel &ch2);
void DMAPriorityOrder(DMAChannel &ch1, DMAChannel &ch2, DMAChannel &ch3);
void DMAPriorityOrder(DMAChannel &ch1, DMAChannel &ch2, DMAChannel &ch3, DMAChannel &ch4);





#endif // __cplusplus

#endif // DMAChannel_h_
