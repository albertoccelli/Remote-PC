# IR remote pc adapter
- Author: Alberto Occelli
- Email: albertoccelli@gmail.com


AtMega32U4-based IR receiver (windows compatible). Turn any remote controller into a pc controller.
Can emulate mouse, keyboard, alt+f4, media buttons and more.

Required components:
- any AtMega32U4 Arduino (Leonardo, Pro Micro);
- generic infrared receiver;
- led;
- 200 ohm resistor;
- breadboard and jumper wires;
- micro USB cable;
- any remote controller.

Optional components:
- welder and stain;
- perfboard;
- female pin headers.

Required libraries: 
- IRremoteInt.h and IRremote.h;
- HID-Project.h.

Steps:

1) wire components to the Arduino as described in the schematic;
2) connect the Arduino to the pc;
3) upload the provided .ino sketch to the board;
4) open the serial monitor and write down each button code;
5) update the sketch with the right values;
6) re-upload the sketch;
7)enjoy.
