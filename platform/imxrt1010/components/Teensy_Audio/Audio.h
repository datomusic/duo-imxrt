#ifndef Audio_h_
#define Audio_h_

// When changing multiple audio object settings that must update at
// the same time, these functions allow the audio library interrupt
// to be disabled.  For example, you may wish to begin playing a note
// in response to reading an analog sensor.  If you have "velocity"
// information, you might start the sample playing and also adjust
// the gain of a mixer channel.  Use AudioNoInterrupts() first, then
// make both changes to the 2 separate objects.  Then allow the audio
// library to update with AudioInterrupts().  Both changes will happen
// at the same time, because AudioNoInterrupts() prevents any updates
// while you make changes.
//

#define AudioNoInterrupts() (NVIC_DisableIRQ(IRQ_SOFTWARE))
#define AudioInterrupts() (NVIC_EnableIRQ(IRQ_SOFTWARE))

#endif
