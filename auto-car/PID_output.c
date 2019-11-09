#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "lcd.h"
#include "camera.h"




int last_error,Sum=0;
int time_diff=0;//  need a timer function here

int PID_output(float Kp,float Ki,float Kd, int error){
	if(time_diff>1000){ //reset the integration/sum part periodically
		Sum=0;
		time_diff=0;
	}

	Sum += error; //Integral part
	int derivative = error - last_error;

	int output= Kp*error + Ki*Sum +  Kd*derivative;

	last_error = error;

	return output;
}

int main(void) {

}

