 BOARD_TAG    = uno # Here goes your board type, e.g. : uno
ARDUINO_PORT = /dev/ttyACM0 # Change to your own tty interface

ARDUINO_LIBS = # The libs needed by your sketchbook, examples are : Wire Wire/utility Ethernet...

include /usr/share/arduino/Arduino.mk # This is where arduino-mk installed
