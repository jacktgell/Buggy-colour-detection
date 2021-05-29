#include "mbed.h"
#include "tunes.h"



void ultra_sonic_distance(void);
int motor(float speedA, float speedB); 
void stop_o();
float duty_o=0.9f;
extern float distance;

//-----------------Motion Control-------------------//

void find_object(float objectRange){
	ultra_sonic_distance();
	while (distance > objectRange)
	{
		ultra_sonic_distance();
		motor(duty_o,-duty_o);
		wait_ms(90);
		stop_o();
	}
	motor(duty_o,-duty_o);
	wait_ms(15);
	stop_o();
}

void get_in_range (int minRange, int maxRange)
{
	ultra_sonic_distance();
	while(distance>=minRange)
	{ 
		ultra_sonic_distance();
		wait(0.25f);
		if (distance<maxRange)
		{
			break;
		}
		motor(0.99f,1.0f);
		wait(0.1f);
		stop_o();
	}
	stop_o();
}



void find_second_object(float objectRange){
	ultra_sonic_distance();
	while (distance > objectRange)
	{
		ultra_sonic_distance();
		motor(-duty_o,duty_o);
		wait_ms(90);
		stop_o();
	}
	motor(-duty_o,duty_o);
	wait_ms(15);
	stop_o();
}

void stop_o(){
	motor(0.0f,0.0f);
	wait(0.1f);
}



