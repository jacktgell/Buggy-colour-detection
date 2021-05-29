#include "mbed.h"
#include "tunes.h"

AnalogIn ldr(A1);           //Instance of the AnalogIn class called 'ldr'
DigitalOut red(D13);			  //Three coloured LEDs for the single Pixel Camera
DigitalOut green(D12);
DigitalOut blue(D11);

#define RED 	0
#define GREEN 1
#define BLUE	2
#define AVG_AMOUNT 100

float array[AVG_AMOUNT+1];
float colour[3];
float light=0.0f;                 //light is Light Dependant Resistor LDR sensor read by ADC and scaled byt 3V3 to calibrate to the refernce Voltage Level

//Serial pc(USBTX,USBRX); 

//-------------COLOUR DETECTION---------------------//
void detect_colour(){
	red=0;green=0;blue=0;		//ALL LEDs OFF!
    while(1)
		{
		//--Artificial RED--//
		red=1;green=0;blue=0;	//ILLUMINATE WITH RED ONLY!
		wait_ms(50);			//wait 50ms for LED/LDR response time before reading with ADC
			
		for (int i=1;i<=AVG_AMOUNT;i++){
				array[i]=ldr*2.65f;;
				array[0]=array[0]+array[i];
				wait_ms(5);
		}
		array[0]=array[0]/AVG_AMOUNT;
		light=array[0];
		colour[RED]=light;
		//pc.printf("RED %4.2fV\n\r",light);	//Send LDR response in Volts to PC via PuTTY
		wait(1.0f);
		
		//--RED--//
		
		red=1;green=0;blue=0;	//ILLUMINATE WITH RED ONLY!
		wait_ms(50);	
		
		for (int i=1;i<=AVG_AMOUNT;i++){
				array[i]=ldr*2.65f;;
				array[0]=array[0]+array[i];
				wait_ms(5);
		}
		array[0]=array[0]/AVG_AMOUNT;
		light=array[0];
		colour[RED]=light;
		//pc.printf("RED %4.2fV\n\r",light);	//Send LDR response in Volts to PC via PuTTY
		wait(0.5f);
		
		
		//--GREEN--//
		red=0;green=1;blue=0;	//ILLUMINATE WITH GREEN ONLY!
		wait_ms(50);			//wait 50ms for LED/LDR response time before reading with ADC
				
		for (int i=1;i<=AVG_AMOUNT;i++){
				array[i]=ldr*1.73f;;
				array[0]=array[0]+array[i];
				wait_ms(5);
		}
		array[0]=array[0]/AVG_AMOUNT;
		light=array[0];
		colour[GREEN]=light;
		//pc.printf("GREEN %4.2fV\n\r",light);	//Send LDR response in Volts to PC via PuTTY
		wait(0.5f);
		
		
		//--BLUE--//
		red=0;green=0;blue=1;	//ILLUMINATE WITH BLUE ONLY!
		wait_ms(50);			//wait 50ms for LED/LDR response time before reading with ADC
		
		
				for (int i=1;i<=AVG_AMOUNT;i++){
				array[i]=ldr*1.43f;
				array[0]=array[0]+array[i];
				wait_ms(5);
		}
		array[0]=array[0]/AVG_AMOUNT;
		light=array[0];
		colour[BLUE]=light;
		//pc.printf("BLUE %4.2fV\n\r",light);	//Send LDR response in Volts to PC via PuTTY
		wait(0.5f);
		
	//--Decision--//
		
		if(colour[RED]<colour[BLUE]&&colour[RED]<colour[GREEN]){
			for (int i=0;i<=4;i++)
			{
				red=1;blue=0;green=0;
				tone1();
				wait_ms(25);
				red=0;blue=0;green=0;
				wait_ms(25);
				red=1;blue=0;green=0;
//				tone1();
				wait_ms(25);
				red=0;blue=0;green=0;
			}
//			pc.printf("ITS A ....>>>>RED<<<<.... CARD!!!\n\r");
			break;
		}
		else if (colour[BLUE]<colour[RED] && colour[BLUE]<colour[GREEN] && colour[RED]-colour[BLUE]>0.6f && colour[RED]-colour[BLUE]<1.40f){
			for (int i=0;i<=4;i++)
			{
				red=0;blue=1;green=0;
				tone2();
				wait_ms(25);
				red=0;blue=0;green=0;
				wait_ms(25);
				red=0;blue=1;green=0;
//				tone2();
				wait_ms(25);
				red=0;blue=0;green=0;
			}
//			pc.printf("RED-BLUE %4.2fV\n\r",colour[RED]-colour[BLUE]);
//			pc.printf("ITS A ....>>>>BLUE<<<<.... CARD!!!\n\r");
			break;
		}
		else{};
	
	}
}
