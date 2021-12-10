This circuit will listen on the analogic current sensor port and it will count for how long the AC consumer is on.
After a while will cut off the AC consumer power since it reached the maxim treshold time for which is allowed to be on. Eventually pressing the pushbak button ("btn") will reset the timer.

Few thigs need to be taken in cosideration:
 - attiny85 on gemma won't work with the relay module unless you change the Vcc to +5V
 - a 120K resistor need to be added as an input impedance for current sensor
 - reload btn needs to be connected to +5Vcc since the pin is already pulled down by the rezistor and the led
  
