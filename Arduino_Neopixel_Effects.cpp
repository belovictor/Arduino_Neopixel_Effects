#include "Arduino_Neopixel_Effects.h"

ArduinoNeopixelEffects::ArduinoNeopixelEffects(int pin, int leds) {
  _pin = pin;
  _leds = leds;
  _neopixels = Adafruit_NeoPixel(_leds, _pin, NEO_GRB + NEO_KHZ800);
  _current_effect = EFFECT_IDLE; // TODO
  _effect_step = 0;
  _effect_sub_step = 0;
  _effect_color1 = Adafruit_NeoPixel::Color(0, 0, 0);
  _effect_color2 = Adafruit_NeoPixel::Color(0, 0, 0);
  _saved_color1 = Adafruit_NeoPixel::Color(0, 0, 0);
  _saved_color2 = Adafruit_NeoPixel::Color(0, 0, 0);
  _last_effect_millis = 0;
}

ArduinoNeopixelEffects::~ArduinoNeopixelEffects() {

}

void ArduinoNeopixelEffects::begin() {
  _neopixels.begin();
  _neopixels.clear();
  _neopixels.show();
}

void ArduinoNeopixelEffects::updateLength(int leds) {
  _leds = leds;
  _neopixels.updateLength(_leds);
  _effect_step = 0;
  _effect_sub_step = 0;
}

void ArduinoNeopixelEffects::updateSettings(ArduinoNeopixelEffects::EffectsSettings settings) {
  _effects_settings = settings;
}

void ArduinoNeopixelEffects::setColor1(uint32_t color) {
  _saved_color1 = _effect_color1;
  _effect_color1 = color;
}

void ArduinoNeopixelEffects::setColor2(uint32_t color) {
  _saved_color2 = _effect_color2;
  _effect_color2 = color;
}

void ArduinoNeopixelEffects::setEffect(int effect) {
  _saved_effect = _current_effect;
  _current_effect = effect;
  _effect_step = 0;
  _effect_sub_step = 0;
}

int ArduinoNeopixelEffects::componentRed(uint32_t color) {
  return (color >> 16) & 0xFF;
}

int ArduinoNeopixelEffects::componentGreen(uint32_t color) {
  return (color >> 8) & 0xFF;
}

int ArduinoNeopixelEffects::componentBlue(uint32_t color) {
  return color & 0xFF;
}

void ArduinoNeopixelEffects::spin() {
  switch (_current_effect) {
    case EFFECT_IDLE:
      if (_effect_step == 0) {
        _neopixels.clear();
        _neopixels.show();
        _effect_step = 1;
      }
      break;
    case EFFECT_FILL:
      if (_effect_step == 0) {
        _neopixels.fill(_effect_color1, 0, _leds);
        _neopixels.show();
        _effect_step = 1;
      }
      break;
    case EFFECT_BLINK:
      if (millis() - _last_effect_millis > _effects_settings.led_blink_period) {
        _last_effect_millis = millis();
        if (_effect_step == 0) {
          _neopixels.fill(_effect_color1, 0, _leds);
          _neopixels.show();
          _effect_step = 1;
        } else {
          _neopixels.fill(_effect_color2, 0, _leds);
          _neopixels.show();
          _effect_step = 0;
        }
      }
      break;
    case EFFECT_BLINK_FAST:
      if (millis() - _last_effect_millis > _effects_settings.led_blink_fast_period) {
        _last_effect_millis = millis();
        if (_effect_step == 0) {
          _neopixels.fill(_effect_color1, 0, _leds);
          _neopixels.show();
          _effect_step = 1;
        } else {
          _neopixels.fill(_effect_color2, 0, _leds);
          _neopixels.show();
          _effect_step = 0;
        }
      }
      break;
    case EFFECT_FLASH:
      if (millis() - _last_effect_millis > _effects_settings.led_flash_period) {
        _last_effect_millis = millis();
        if (_effect_step % 2 == 0) {
          _neopixels.fill(_effect_color1, 0, _leds);
          _neopixels.show();
          _effect_step++;
        } else {
          _neopixels.fill(_effect_color2, 0, _leds);
          _neopixels.show();
          _effect_step++;
        }
        if (_effect_step == _effects_settings.led_flash_count * 2) {
          // When flash finished return to previous effect
          _current_effect = _saved_effect;
          _effect_color1 = _saved_color1;
          _effect_color2 = _saved_color2;
          _effect_step = 0;
        }
      }
      break;
    case EFFECT_DOUBLE_BLINK:
      switch (_effect_step) {
        case 0:
          if (millis() - _last_effect_millis > _effects_settings.led_double_blink_time) {
            _last_effect_millis = millis();
            _neopixels.fill(_effect_color2, 0, _leds);
            _neopixels.show();
            _effect_step = 1;
          } else {
            _neopixels.fill(_effect_color1, 0, _leds);
            _neopixels.show();
          }
          break;
        case 1:
          if (millis() - _last_effect_millis > _effects_settings.led_double_blink_time) {
            _last_effect_millis = millis();
            _neopixels.fill(_effect_color1, 0, _leds);
            _neopixels.show();
            _effect_step = 2;
          }
          break;
        case 2:
          if (millis() - _last_effect_millis > _effects_settings.led_double_blink_time) {
            _last_effect_millis = millis();
            _neopixels.fill(_effect_color2, 0, _leds);
            _neopixels.show();
            _effect_step = 3;
          }
          break;
        case 3:
          if (millis() - _last_effect_millis > _effects_settings.led_double_blink_period) {
            _last_effect_millis = millis();
            _effect_step = 0;
          }
          break;
      }
      break;
    case EFFECT_GLOW:
      if (_effect_step == 0) {
        _neopixels.fill(Adafruit_NeoPixel::Color(0, 0, 0), 0, _leds);
        _effect_step = 1;
      } else if (_effect_step == 1) {
        if (millis() - _last_effect_millis > _effects_settings.led_glow_step_period) {
            _last_effect_millis = millis();
            if (_effect_sub_step < 100) {
              _neopixels.fill(Adafruit_NeoPixel::Color(componentRed(_effect_color1)/100.0*_effect_sub_step, componentGreen(_effect_color1)/100.0*_effect_sub_step, componentBlue(_effect_color1)/100.0*_effect_sub_step), 0, _leds);
              _neopixels.show();
              _effect_sub_step++;
            } else {
              _effect_step = 2;
            }
        }
      } else {
        if (millis() - _last_effect_millis > _effects_settings.led_glow_step_period) {
            _last_effect_millis = millis();
            if (_effect_sub_step > 0) {
              _neopixels.fill(Adafruit_NeoPixel::Color(componentRed(_effect_color1)/100.0*_effect_sub_step, componentGreen(_effect_color1)/100.0*_effect_sub_step, componentBlue(_effect_color1)/100.0*_effect_sub_step), 0, _leds);
              _neopixels.show();
              _effect_sub_step--;
            } else {
              _effect_step = 0;
            }
        }
      }
      break;
    case EFFECT_RUN_FORWARD:
      if (_effect_step == 0) {
        _neopixels.fill(Adafruit_NeoPixel::Color(0, 0, 0), 0, _leds);
        _effect_sub_step = 0;
        _effect_step = 1;
      } else if (_effect_step == 1) {
        if (millis() - _last_effect_millis > _effects_settings.led_run_step_period) {
        _last_effect_millis = millis();
          _neopixels.clear();
          _neopixels.setPixelColor(_effect_sub_step, _effect_color1);
          _neopixels.show();
          _effect_sub_step++;
          if (_effect_sub_step >= _leds) {
            _effect_sub_step = 0;
          }
        }
      }
      break;
    case EFFECT_RUN_BACKWARD:
      if (_effect_step == 0) {
        _neopixels.fill(Adafruit_NeoPixel::Color(0, 0, 0), 0, _leds);
        _effect_sub_step = _leds - 1;
        _effect_step = 1;
      } else if (_effect_step == 1) {
        if (millis() - _last_effect_millis > _effects_settings.led_run_step_period) {
          _last_effect_millis = millis();
          _neopixels.clear();
          _neopixels.setPixelColor(_effect_sub_step, _effect_color1);
          _neopixels.show();
          _effect_sub_step--;
          if (_effect_sub_step < 0) {
            _effect_sub_step = _leds - 1;
          }
        }
      }
      break;
    case EFFECT_RUN_PONG:
      if (_effect_step == 0) {
        _neopixels.fill(Adafruit_NeoPixel::Color(0, 0, 0), 0, _leds);
        _effect_sub_step = 0;
        _effect_step = 1;
      } else if (_effect_step == 1) {
        if (millis() - _last_effect_millis > _effects_settings.led_run_step_period) {
          _last_effect_millis = millis();
          _neopixels.clear();
          _neopixels.setPixelColor(_effect_sub_step, _effect_color1);
          _neopixels.show();
          _effect_sub_step++;
          if (_effect_sub_step >= _leds) {
            _effect_sub_step = _leds - 1;
            _effect_step = 2;
          }
        }
      } else if (_effect_step == 2) {
        if (millis() - _last_effect_millis > _effects_settings.led_run_step_period) {
          _last_effect_millis = millis();
          _neopixels.clear();
          _neopixels.setPixelColor(_effect_sub_step, _effect_color1);
          _neopixels.show();
          _effect_sub_step--;
          if (_effect_sub_step < 0) {
            _effect_sub_step = 0;
            _effect_step = 1;
          }
        }
      }
      break;
  }
}
