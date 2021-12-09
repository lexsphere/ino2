int analog_sensor_pin = 1; // 1 - ok correct
int output_pin = 0;
int d0 = 1;
int c = 0;
int sensor_val = 0;
boolean alarm = false;
boolean load_on = false;
boolean last_load_on = false;
int btn_state = HIGH;
int last_btn_state = 0;
int btn_state_counter = 0;
int on_counter = 0;
int main_counter = 0;
int i = 0;			/* main loop counter */
int j = 0;			/* main loop counter */
int ssum = 0;		/* samples sum */
#include <avr/wdt.h>

#define TRESHOLD 10 /* ~mV */
int max_onc = 1800; //1800 /* consecutive cycles; max on counter */ 
int sample_interval = 10; //10 /* sample interval - sample at every x cycles */
int reset_interval = 1;	//360*24 /* if we take samples at every 10 seconds */

int reset_counter = 0;
#define SAR_LEN 3600 /* number of samples */
#define S_PROCENTAGE 90	/* samples_on procentage */
int sarr[SAR_LEN] = {0}; /* samples array */
int sproc = SAR_LEN * S_PROCENTAGE/100; /* calculate samples procentage */ 


void setup() {
	pinMode(output_pin, OUTPUT);
	pinMode(d0, OUTPUT);
	delay(2000);
	wdt_enable(WDTO_8S); /*16000000L = 16s*/
}

void check_consecutive_current_withdraw(int sensor_val) {
	int k = 0;
	int sum=0;

	for(k=0; k<34; k++) {
		sensor_val = analogRead(analog_sensor_pin);
		if (sensor_val > TRESHOLD) {
			sum++;	
		} else {
			// do nothing	
		}
		delay(1);
	}
	if (sum > 1){
		load_on = true;
		steady_on();
	} else {
		load_on = false;
		flash();
	}
	if (last_load_on == true && load_on == true) {
		on_counter++;
	} else {
		on_counter = 0;
	}

}
void check_load_on_during_and_interval() {
	//// check load_on during and interval
	//if  ( i%sample_interval == 0 ) {
	//	if (load_on) {
	//		sarr[j] = 1;	
	//	} else {
	//		sarr[j] = 0;	
	//	}
	//	j++;
	//	if (j > SAR_LEN){
	//		j = 0;
	//	}
	//}

	//ssum = 0;
	//for (int m = 0; m < SAR_LEN; m++) {
	//	ssum = ssum + sarr[m]; 
	//}

	//if (ssum > sproc) {
	//	alarm = true;
	//	memset(sarr, 0, sizeof(sarr));
	//}

}
void reset_via_watchdog_timer() {
	while(true) {}
}
void flash() {
	digitalWrite(d0, HIGH); 
	delay(100);
	digitalWrite(d0, LOW); 
}
void long_flash() {
	digitalWrite(d0, HIGH); 
	delay(1000);
	digitalWrite(d0, LOW); 
}
void steady_on() {
	digitalWrite(d0, HIGH); 
}
void turn_led_off() {
	digitalWrite(d0, LOW); 
}
void loop() {
	delay(1000);
	wdt_reset();

	if (alarm) {
		digitalWrite(output_pin, LOW); 
		steady_on();
		delay(3000);

		pinMode(d0, INPUT);
		c = 0;
		while(true) {
			wdt_reset();
			delay(1000);
			btn_state = digitalRead(d0);
			if (btn_state == LOW && c > 0) {
				break;
			}
			if (btn_state == HIGH) {
				alarm = false;
				on_counter = 0;
				c = 1;
			} 
		}
		pinMode(d0, OUTPUT);

	} else {  /* happy days */
		digitalWrite(output_pin, HIGH); 
		pinMode(d0, INPUT);
		c = 0;
		btn_state = digitalRead(d0);
		if (btn_state == HIGH) {
			while (true) {
				wdt_reset();
				delay(1000);
				c++;
				btn_state = digitalRead(d0);
				if (btn_state == LOW) {
					digitalWrite(output_pin, LOW);
					delay(3000);
					digitalWrite(output_pin, HIGH);
					break;
				}
			}				
		}
		pinMode(d0, OUTPUT);

		if (on_counter > max_onc) {
			alarm = true;
		}	

		last_load_on = load_on; //save last_load_on value
		check_consecutive_current_withdraw(sensor_val);
	}
	if (i%sample_interval == 0) {
		reset_counter++;
	}
	if (reset_counter > reset_interval) {
		reset_via_watchdog_timer();
	}
	i++;
}

