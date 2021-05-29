/*
Faisal Fazal-ur-rehman
*/
#include "mbed.h"
#include "motor.h"
#include "tunes.h"
#include "LDR-LED.h"
#include "SONIC.h"

//--------------------------------//
#define TIME_PERIOD 2             //Constant compiler Values here 2 equates to 2ms or 500Hz base Frequency
#define DUTY 0.9f                  //DUTY of 1.0=100%, 0.4=40% etc.,
#define FOURTY_TURN_SEC 0.45f
#define M_LENGTH_SEC 2.0f
#define LDR_MIN_RANGE 30
#define LDR_MAX_RANGE 52


//--------------------------------//

Serial pc(USBTX,USBRX);   

//---------DECLARATION OF PINS--------------//
DigitalIn myButton(USER_BUTTON); 
DigitalOut led(LED1);             

Motor motor_A(D7,D8);             //Instances of the Motor Class see motor.h anf motor.cpp
Motor motor_B(D9,D10);            //They must be connected to these ports D7,D8 & D9,D10

DigitalOut Trigger(D6);     //Instance of the DigitalInOut class called 'TriggerEcho'
DigitalIn Echo(D2);     //Instance of the DigitalInOut class called 'TriggerEcho'
Timer pulse;     


//--------FUNCTIONS DECLARATION------------//
int motor(float speedA, float speedB);    
void stop();
void ultra_sonic_distance(void);
float GetDistance(void);          //Function Prototype see function for desciption
void find_object(float objectRange); //Finds an object within a given range where object range is in mm
void get_in_range (int minRange, int maxRange);
void detect_colour();
void find_second_object(float objectRange);

//--------GLOBAL VARIABLES-----------------//
float Distance;                   //Assign global variables here
float distance=0.0f;              //distance is varible used by Ultrasnic code in mm
float duty=DUTY;
float fourtyTsec=FOURTY_TURN_SEC;
float mLENGTHsec=M_LENGTH_SEC;

//---------------MAIN----------------------//
int main ()
{
	
  pc.baud(115200);            //BAUD Rate to 115200
  motor_A.Period_in_ms(TIME_PERIOD);    
  motor_B.Period_in_ms(TIME_PERIOD);
  motor(0.0f,0.0f);                    
	pulse.start();                        //Start the instance of the class timer
	while(myButton==1){}
	
	//----------First Object----------//
	
	//Turn 40ish Deg left 
	motor(duty,-duty);
	wait(FOURTY_TURN_SEC);
	stop();
	
	//Move Forward, Half length
	motor(1.0f,1.0f);
	wait(mLENGTHsec*0.45f);
	stop();
	
	//Find the object to the left
	find_object(600);
	
	//Move Forward, Quarter length
	motor(1.0f,0.9f);
	wait(mLENGTHsec*0.2f);
	stop();
	
	//Turn Right
	motor(-duty,duty);
	wait(fourtyTsec*0.75f);
	stop();
	
	//Find the object to the left
	find_object(600);
	
//	//Move Forward, 1/8 length
//	motor(1.0f,1.0f);
//	wait(mLENGTHsec*0.15f);
//	stop();
//	
//	//Turn Right
//	motor(-duty,duty);
//	wait(fourtyTsec*0.75f);
//	stop();
//	
//	//Find the object to the left
//	find_object(400);
	
	//Move forward until the object is between 50 to 30cm from the sonic sensor
	get_in_range (LDR_MIN_RANGE,LDR_MAX_RANGE);
	
	//Detect and identify the colour of the paper
	detect_colour();
	
	
	//------------Second Object-----------//
	
	//Reverse
	motor(-1.0f,-1.0f);
	wait(mLENGTHsec*0.4f);
	stop();
	
	//Big turn right
	motor(-duty,duty);
	wait(FOURTY_TURN_SEC*1.5f);
	stop();
	
	//Move forward
	motor(0.9f,1.0f);
	wait(mLENGTHsec*0.3f);
	stop();
	
	//Find the object to the right
	find_second_object(800);
	
	//Move forward
	motor(1.0f,1.0f);
	wait(mLENGTHsec*0.5f);
	stop();
	
	//Turn right
	motor(duty,-duty);
	wait(fourtyTsec*0.65f);
	stop();
	
	//Find the object to the right
	find_second_object(700);
	
	//Move forward
	motor(1.0f,1.0f);
	wait(mLENGTHsec*0.25f);
	stop();
	
	//Turn right
	motor(duty,-duty);
	wait(fourtyTsec*0.65f);
	stop();
	
	//Find the object to the right
	find_second_object(500);
	
	//Move forward until the object is between 50 to 30cm from the sonic sensor
	get_in_range (LDR_MIN_RANGE,LDR_MAX_RANGE+5);
	
	//Detect and identify the colour of the paper
	detect_colour();
}




//------------------------------------------Functions----------------------------------------------------//

//------------------MOTORS-----------------//
//Small function to control motors use as motor(1.0,-0.5) Motor A full speed forward Motor B half speed reversed
int motor(float speedA, float speedB){
   if(speedA>1.0f||speedA<-1.0f){ //CHECK speedA Value is in Range!
      return -1;                  //return ERROR code -1=speedA Value out of range! EXIT Function
   }
   if(speedB>1.0f||speedA<-1.0f){ //CHECK speedB Value is in Range!
      return -2;                  //return ERROR code -2=speedB Value out of range! EXIT Function
   }
   //If speed values have passed the checks above then the following code will be executed
   if(speedA<0.0f){
     motor_A.Rev(-speedA);
   }
   else{
     motor_A.Fwd(speedA);
   }
   if(speedB<0.0f){
     motor_B.Rev(-speedB);
   }
   else{
     motor_B.Fwd(speedB);
   }
   return 0;                      //Return ERROR code Zero i.e. NO ERROR success!
}

void stop(){
	motor(0.0f,0.0f);
	wait(0.1f);
}

//-------------ULTRA_SONIC------------//
// Function ultra_sonic_distance() will load the global variable distance with Ultra Sonic Sensor value in mm
// and then send the value to the stdio ouput i.e serial over USB
void ultra_sonic_distance(void){
   distance=GetDistance();
   //pc.printf("%dmm \n\r",(int)distance);
    
  }
  // Function GetDistance() will return a float value of the Distance from the Ultrasonic Sensor in millimetres typically use as: ‘(float)myDistance=GetDistance();’
float GetDistance(){                                  //Function Name to be called
  int EchoPulseWidth=0,EchoStart=0,EchoEnd=0;         //Assign and set to zero the local variables for this function
  Trigger = 1;                                        //Signal goes High i.e. 3V3
  wait_us(100);                                       //Wait 100us to give a pulse width Triggering the Ultrasonic Module
  Trigger = 0;                                        //Signal goes Low i.e. 0V
  pulse.reset();                                      //Reset the instance of the Timer Class
  while(Echo == 0 && EchoStart < 25000){              //wait for Echo to go high
    EchoStart=pulse.read_us();                        //Conditional 'AND' with timeout to prevent blocking!      
    }
    while(Echo == 1 && ((EchoEnd - EchoStart) < 25000)){ //wait for echo to return low
      EchoEnd=pulse.read_us();                        //Conditional 'OR' with timeout to prevent blocking!   
    }
    EchoPulseWidth = EchoEnd - EchoStart;             //time period in us
    return (float)EchoPulseWidth/5.8f;                //calculate distance in mm and return the value as a float
}


