#ifndef FASTLED_STUB_H_ULX8P0SG
#define FASTLED_STUB_H_ULX8P0SG

struct CRGB {
  union {
    struct {
      union {
        uint8_t r;
        uint8_t red;
      };
      union {
        uint8_t g;
        uint8_t green;
      };
      union {
        uint8_t b;
        uint8_t blue;
      };
    };
    uint8_t raw[3];
  };

  /// Array access operator to index into the crgb object
  inline uint8_t &operator[](uint8_t x) __attribute__((always_inline)) {
    return raw[x];
  }

  /// Array access operator to index into the crgb object
  inline const uint8_t &operator[](uint8_t x) const
      __attribute__((always_inline)) {
    return raw[x];
  }

  // default values are UNINITIALIZED
  inline CRGB() __attribute__((always_inline)) = default;

  /// allow construction from R, G, B
  inline CRGB(uint8_t ir, uint8_t ig, uint8_t ib) __attribute__((always_inline))
  : r(ir), g(ig), b(ib) {}

  /// allow construction from 32-bit (really 24-bit) bit 0xRRGGBB color code
  inline CRGB(uint32_t colorcode) __attribute__((always_inline))
  : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF),
    b((colorcode >> 0) & 0xFF) {}

  /* /// allow construction from a LEDColorCorrection enum */
  /* inline CRGB( LEDColorCorrection colorcode) __attribute__((always_inline))
   */
  /* : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >>
   * 0) & 0xFF) */
  /* { */

  /* } */

  /* /// allow construction from a ColorTemperature enum */
  /* inline CRGB( ColorTemperature colorcode) __attribute__((always_inline)) */
  /* : r((colorcode >> 16) & 0xFF), g((colorcode >> 8) & 0xFF), b((colorcode >>
   * 0) & 0xFF) */
  /* { */

  /* } */

  /// allow copy construction
  inline CRGB(const CRGB &rhs) __attribute__((always_inline)) = default;
  /// allow construction from HSV color
  /* inline CRGB(const CHSV& rhs) __attribute__((always_inline)) */
  /* { */
  /* } */

  /// allow assignment from one RGB struct to another
  inline CRGB &operator=(const CRGB &rhs)
      __attribute__((always_inline)) = default;

  /// allow assignment from 32-bit (really 24-bit) 0xRRGGBB color code
  inline CRGB &operator=(const uint32_t colorcode)
      __attribute__((always_inline)) {
    r = (colorcode >> 16) & 0xFF;
    g = (colorcode >> 8) & 0xFF;
    b = (colorcode >> 0) & 0xFF;
    return *this;
  }

  /// allow assignment from R, G, and B
  inline CRGB &setRGB(uint8_t nr, uint8_t ng, uint8_t nb)
      __attribute__((always_inline)) {
    r = nr;
    g = ng;
    b = nb;
    return *this;
  }

  /// allow assignment from H, S, and V
  inline CRGB &setHSV(uint8_t hue, uint8_t sat, uint8_t val)
      __attribute__((always_inline)) {
    return *this;
  }

  /// allow assignment from just a Hue, saturation and value automatically at
  /// max.
  inline CRGB &setHue(uint8_t hue) __attribute__((always_inline)) {
    return *this;
  }

  /* /// allow assignment from HSV color */
  /* inline CRGB& operator= (const CHSV& rhs) __attribute__((always_inline)) */
  /* { */
  /*     return *this; */
  /* } */

  /// allow assignment from 32-bit (really 24-bit) 0xRRGGBB color code
  inline CRGB &setColorCode(uint32_t colorcode) __attribute__((always_inline)) {
    r = (colorcode >> 16) & 0xFF;
    g = (colorcode >> 8) & 0xFF;
    b = (colorcode >> 0) & 0xFF;
    return *this;
  }

  /// allow assignment from 32-bit (really 24-bit) 0xRRGGBB color code
  /* inline CRGB &setColorCode(uint32_t colorcode)
   * __attribute__((always_inline)) { */
  /*   r = (colorcode >> 16) & 0xFF; */
  /*   g = (colorcode >> 8) & 0xFF; */
  /*   b = (colorcode >> 0) & 0xFF; */
  /*   return *this; */
  /* } */

  /// add one RGB to another, saturating at 0xFF for each channel
  inline CRGB &operator+=(const CRGB &rhs) { return *this; }

  /// add a contstant to each channel, saturating at 0xFF
  /// this is NOT an operator+= overload because the compiler
  /// can't usefully decide when it's being passed a 32-bit
  /// constant (e.g. CRGB::Red) and an 8-bit one (CRGB::Blue)
  inline CRGB &addToRGB(uint8_t d) { return *this; }

  /// subtract one RGB from another, saturating at 0x00 for each channel
  inline CRGB &operator-=(const CRGB &rhs) { return *this; }

  /// subtract a constant from each channel, saturating at 0x00
  /// this is NOT an operator+= overload because the compiler
  /// can't usefully decide when it's being passed a 32-bit
  /// constant (e.g. CRGB::Red) and an 8-bit one (CRGB::Blue)
  inline CRGB &subtractFromRGB(uint8_t d) { return *this; }

  /// subtract a constant of '1' from each channel, saturating at 0x00
  inline CRGB &operator--() __attribute__((always_inline)) {
    subtractFromRGB(1);
    return *this;
  }

  /// subtract a constant of '1' from each channel, saturating at 0x00
  inline CRGB operator--(int) __attribute__((always_inline)) {
    CRGB retval(*this);
    --(*this);
    return retval;
  }

  /// add a constant of '1' from each channel, saturating at 0xFF
  inline CRGB &operator++() __attribute__((always_inline)) {
    addToRGB(1);
    return *this;
  }

  /// add a constant of '1' from each channel, saturating at 0xFF
  inline CRGB operator++(int) __attribute__((always_inline)) {
    CRGB retval(*this);
    ++(*this);
    return retval;
  }

  /// divide each of the channels by a constant
  inline CRGB &operator/=(uint8_t d) {
    r /= d;
    g /= d;
    b /= d;
    return *this;
  }

  /// right shift each of the channels by a constant
  inline CRGB &operator>>=(uint8_t d) {
    r >>= d;
    g >>= d;
    b >>= d;
    return *this;
  }

  /// multiply each of the channels by a constant,
  /// saturating each channel at 0xFF
  inline CRGB &operator*=(uint8_t d) { return *this; }

  /// return a CRGB object that is a scaled down version of this object
  inline CRGB scale8(const CRGB &scaledown) const { return *this; }

  /// fadeLightBy is a synonym for nscale8_video( ..., 255-fadefactor)
  inline CRGB &fadeLightBy(uint8_t fadefactor) { return *this; }

  /// "or" operator brings each channel up to the higher of the two values
  inline CRGB &operator|=(const CRGB &rhs) {
    if (rhs.r > r)
      r = rhs.r;
    if (rhs.g > g)
      g = rhs.g;
    if (rhs.b > b)
      b = rhs.b;
    return *this;
  }

  /// "or" operator brings each channel up to the higher of the two values
  inline CRGB &operator|=(uint8_t d) {
    if (d > r)
      r = d;
    if (d > g)
      g = d;
    if (d > b)
      b = d;
    return *this;
  }

  /// "and" operator brings each channel down to the lower of the two values
  inline CRGB &operator&=(const CRGB &rhs) {
    if (rhs.r < r)
      r = rhs.r;
    if (rhs.g < g)
      g = rhs.g;
    if (rhs.b < b)
      b = rhs.b;
    return *this;
  }

  /// "and" operator brings each channel down to the lower of the two values
  inline CRGB &operator&=(uint8_t d) {
    if (d < r)
      r = d;
    if (d < g)
      g = d;
    if (d < b)
      b = d;
    return *this;
  }

  /// this allows testing a CRGB for zero-ness
  inline operator bool() const __attribute__((always_inline)) {
    return r || g || b;
  }

  /// invert each channel
  inline CRGB operator-() const {
    CRGB retval;
    retval.r = 255 - r;
    retval.g = 255 - g;
    retval.b = 255 - b;
    return retval;
  }

  /// setParity adjusts the color in the smallest
  /// way possible so that the parity of the color
  /// is now the desired value.  This allows you to
  /// 'hide' one bit of information in the color.
  ///
  /// Ideally, we find one color channel which already
  /// has data in it, and modify just that channel by one.
  /// We don't want to light up a channel that's black
  /// if we can avoid it, and if the pixel is 'grayscale',
  /// (meaning that R==G==B), we modify all three channels
  /// at once, to preserve the neutral hue.
  ///
  /// There's no such thing as a free lunch; in many cases
  /// this 'hidden bit' may actually be visible, but this
  /// code makes reasonable efforts to hide it as much
  /// as is reasonably possible.
  ///
  /// Also, an effort is made to have make it such that
  /// repeatedly setting the parity to different values
  /// will not cause the color to 'drift'.  Toggling
  /// the parity twice should generally result in the
  /// original color again.
  ///
  inline void setParity(uint8_t parity) {}

  /// Predefined RGB colors
  typedef enum {
    Blue = 0x0000FF,
    Black = 0x000000,
  } HTMLColorCode;
};

struct FastLED_ {
  void show();
  void clear();
  void setBrightness(int);
  void setCorrection(int);
  template <int A, int B, int C> void addLeds(void *, int);
};

FastLED_ FastLED;

CRGB blend(const CRGB &p1, const CRGB &p2, int amountOfP2) { return p1; }

#endif /* end of include guard: FASTLED_STUB_H_ULX8P0SG */
