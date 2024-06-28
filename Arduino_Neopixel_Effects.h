#ifndef Arduino_Neopixel_Effects_h
#define Arduino_Neopixel_Effects_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

/* Effects */
#define EFFECT_IDLE 0 // Do nothing
#define EFFECT_FILL 1 // Static light with one color
#define EFFECT_BLINK 2 // Blink with one color
#define EFFECT_BLINK_FAST 3 // Blink with one color
#define EFFECT_FLASH 4 // Blink FLASH_COUNT times and return to previous effect
#define EFFECT_DOUBLE_BLINK 5 // Make double blinks
#define EFFECT_GLOW 6 // Change gradually between saved color and effect color
#define EFFECT_RUN_FORWARD 7 // A light runs forward
#define EFFECT_RUN_BACKWARD 8 // A light runs backward
#define EFFECT_RUN_PONG 9 // A light runs forward and then backward

class ArduinoNeopixelEffects {
  public:
    struct EffectsSettings {
      float led_max_brightness;
      float led_blink_period;
      float led_blink_fast_period;
      float led_flash_period;
      float led_flash_count;
      float led_double_blink_period;
      float led_double_blink_time;
      float led_glow_step_period;
      float led_run_step_period;
    };

    ArduinoNeopixelEffects(int pin, int leds);
    ~ArduinoNeopixelEffects();

    void begin();
    void updateLength(int leds);
    void updateSettings(ArduinoNeopixelEffects::EffectsSettings settings);
    void setColor1(uint32_t color);
    void setColor2(uint32_t color);
    void setEffect(int effect);
    void spin();
    static int componentRed(uint32_t color);
    static int componentGreen(uint32_t color);
    static int componentBlue(uint32_t color);
  private:
    EffectsSettings _effects_settings = {
      32, // LED max brightness
      1000, // LED blink period
      300, // LED blink fast period
      200, // LED flash period
      3, // LED flash count
      500, // LED double blink period
      75, // LED double blink time
      12, // LED glow step period
      50, // LED run step period
    };

    int _pin;
    int _leds;
    int _current_effect;
    int _saved_effect;
    int _effect_step;
    int _effect_sub_step;
    uint32_t _effect_color1;
    uint32_t _effect_color2;
    uint32_t _saved_color1;
    uint32_t _saved_color2;
    unsigned long _last_effect_millis;
    Adafruit_NeoPixel _neopixels;
};

#endif