# vesc-flipsky-arduino-lights-controller
Simple Arduino code which takes Flipsky remote UART packets to: trigger STOP light / WS2812, control headlights or other relays.

# PIN DESCRIPTION

| PIN | DESCRIPTION                                                             | INVERTED |
|-----|-------------------------------------------------------------------------|----------|
| 0   | RX input (TX from Flipsky remote receiver)                              | -        |
| 3   | STOP/IDLE LIGHT - HIGH active                                           | n        |
| 4   | STOP/IDLE LIGHT - LOW active                                            | y        |
| 6   | WS2812 / WS2812b signal                                                 | -        |
| 9   | EXTRA OUTPUT - LOW active (smash quickly 5 times reverse to toggle)     | y        |
| 10  | HEADLIGHT OUTPUT - LOW active (smash quickly 3 times reverse to toggle) | y        |
| 11  | EXTRA OUTPUT - HIGH active (same as above, PIN 9)                       | n        |
| 12  | HEADLIGHT OUTPUT - HIGH active (same as above, PIN 10)                  | n        |

# USAGE
Connect Arduino pin 0 (RXD) to TXD of Flipsky remote receiver.

- Riding forvard will lit WS2812 dark red.
- Idle or breaking will light WS2812 bright red + STOP pin will be switched HIGH
- Quickly smash full reverse 3 times to toggle HEADLIGHT pin ON/OFF
- Quickly smash full reverse 4 times to toggle stealth mode ON/OFF (no lights - default states on pins)
- Quickly smash full reverse 5 times to toggle EXTRA pin ON/OFF

# External library
Adafruit_NeoPixel to drive WS2812b leds.
https://github.com/adafruit/Adafruit_NeoPixel
