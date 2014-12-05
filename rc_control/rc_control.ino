#include <Servo.h>
Servo servoStr; 
Servo servoEsc;

float a=-.75;
float turn=0;
int maxSpeed=0;
int angle=90;

void setup()
{
   servoStr.attach(9);
   servoEsc.attach(10);
   Serial.begin(9800);
}

void setVel(int v)
{
  servoEsc.write(v);
}

void setTurn(float turn)
{
  float radTurn=(turn/180)*2;
  servoStr.writeMicroseconds(1250+radTurn*(350));
  delay(1000);
  
}
int speedCalc(float v)
{
  int v1;
  if(v>0)
  {
    v1=90-(v*(maxSpeed+90));
    
  }
  else
  {
    v1=(90+(-v*(maxSpeed+90)));
  }
  return v1;
}

void loop()
{
   int v;
   v=speedCalc(a);
   setVel(v);
   setTurn(turn);
   
}
