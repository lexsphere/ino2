For this project a adafruit gemma was used with teeny-tiny modifications.
This circuit will listen on the analog pin 1 the output of a current sensor and it will count for how long the AC consumer is on.
After a while will cut off the AC consumer power since it reached the maxim time interval for which is allowed to be on. Eventually pressing the pushbak button ("btn") will reset the timer.

Few things need to be taken in cosideration:
 - attiny85 on gemma won't work with the relay module unless you change the Vcc to +5V
 - a 120K resistor need to be added as an input impedance for current sensor
 - reload btn needs to be connected to +5Vcc since the pin is already pulled down by the rezistor and the led
  
