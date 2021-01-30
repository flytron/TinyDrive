# TinyDrive

SBUS/PPM 1.5A dual motor controller for small robots, tanks & cars.

TinyDrive is the first all in one dual-motor controller for small robotic platforms.
The board is supporting SBUS and PPM RC receivers, 1.5A motor outputs, Headlight Output, Protected battery input and motor outputs. 

Visit the product page for more info
https://store.flytron.com/products/tiny-drive-v1-sbus-ppm-1-5a-dual-motor-controller


## Installing the Firmware

TinyDrive v1 boards are based on Atmega328P microcontroller that installed Arduino Pro Mini bootloader. 
You need only Arduino IDE(1.8 or above) & an FTDI 3.3V Serial Adapter to compile and upload the firmare.

### Arduino IDE Settings
- Select Tools>Board>Arduino Pro or Pro mini
- Processor>ATmega328P (5v, 16Mhz)
- Port> [FTDI Adapter's COM port]

### Wiring

- Basic serial TX/Rx/GND pin connection required to upload the firmware. 
- TX of FTDI to RX of TinyDrive
- RX of FTDI to TX of TinyDrive
- GND of FTDI to GND of TinyDrive


### Arduino Firmware Upload Process:

- Unplug the battery
- Press the Upload button of Arduino IDE and wait for compiling
- Plug the battery immediately(in 2 seconds) when you see "Uploading..." message on the display
- FTDI LEDs will start blinking and you will see "Done Uploading" message in a few seconds.  


## Community & Builds

Join our Tiny Trak - Micro FPV Crawler Facebook group for the builds and questions.
https://www.facebook.com/groups/181517579149618

Search TinyTrak on Thingiverse for the 3D models
https://www.thingiverse.com/search?q=tinytrak


