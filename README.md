Infrared remote control experiments
=====

The aim is to have a tiny transmitter which works with the OpenPandora without compromising on its handheld quality. (For receivers, this is not a problem, because they are generally resting on a solid surface while you send them signals from somewhere else. But for a transmitter, handheld is very useful!)

Currently using very simple descriptions of the waveforms (and two different waveforms per code for the protocols with a toggle bit). Later it would be useful to handle the actual protocols, e.g. integrate with LIRC.

rx_dur (Arduino)
----
Monitors a digital signal on the specified pin, waits for this signal to match the trigger level, and outputs a list of comma-separate durations (in microseconds) on Serial. The first duration is the length of time the signal stays at the trigger level; the second is the length of time it stays at the opposite level after that; and so on. It records data until the buffer is full or until the specified maximum time after the trigger, then it transmits on Serial afterwards.

This can be connected to an IR demodulator device to read remote control signals.

tx_tone (Arduino)
----
Reads a comma-separated list of durations (in microseconds) from Serial, and outputs the 38kHz modulated version on the specified pin, using the Arduino tone function. The first duration is the length of the first burst; the second duration is the length of the first space; and so on. The bursts seem to come out about 40us shorter than specified (and the spaces about 40us longer), but the devices I have tested are not fussy about this.

From a handheld perspective, the Uno is too big, and the Nano does not seem to work on the Pandora without a hub.

circuits/led_driver_5V
----
This circuit drives the LED with about 150mA while only drawing a maximum of 23mA from the power supply. It is useful for USB-connected devices like Arduino, on the Pandora's OTG port which can only provide 100mA. The signal output pin from the Arduino is connected to the gate of the MOSFET (signal is active high).
