#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>
#include <Ultrasonic.h>

MeDCMotor m1(M1);
MeDCMotor m2(M2);
Ultrasonic left(9,10);
Ultrasonic right(A1,A0);
MeUltrasonicSensor forward(PORT1);

int regSpeed = 100;
float leftPrev = 0;
float rightPrev = 0;
float leftSum = 0;
float rightSum = 0;
float Kp = 1;
float Kd = .5;
float Ki = .3;
float setpoint = 17;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(forward.distanceCm());
  //Serial.println(left.Ranging(CM));
  Serial.println(right.Ranging(CM));
  while(forward.distanceCm() > setpoint && left.Ranging(CM) > setpoint && right.Ranging(CM) > setpoint){
    m1.run(-regSpeed);
    m2.run(regSpeed);
  }

  while(forward.distanceCm() < setpoint){
    //m1.stop();
    //m2.stop();
    if(left.Ranging(CM) < right.Ranging(CM)){
      m2.run(regSpeed);
      //delay(2500);
      m1.stop();
    }else{
      m1.run(-regSpeed);
      //delay(2500);
      m2.stop();  
    }
  }

  while(left.Ranging(CM) < setpoint){
      float error = setpoint - left.Ranging(CM);
      float prevDifference = error - leftPrev;
      leftPrev = error;
      leftSum += error;
      float c = Kp*error + Kd*prevDifference + Ki*leftSum;
      m1.run(-(regSpeed + c)); 
  }
  while(right.Ranging(CM) < setpoint){
      float error = setpoint - right.Ranging(CM);
      float prevDifference = error - rightPrev;
      rightPrev = error;
      rightSum += error;
      float c = Kp*error + Kd*prevDifference + Ki*leftSum;
      m1.run(regSpeed + c); 
  }
}
