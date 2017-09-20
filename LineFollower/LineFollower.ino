#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMCore.h>

MeDCMotor m1(M1);
MeDCMotor m2(M2);
MeLineFollower right(PORT4);
MeLineFollower left(PORT3);
int rturns = 0;
int lturns = 0;
int turnaround = 0;
int regSpeed = 125;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(left.readSensors()==2 && right.readSensors()==1){
      turnaround = 0;
      m1.run(-regSpeed);
      m2.run(regSpeed);
  }
  if(right.readSensors()==2 && rturns<=3){
      turnaround = 0;
      rturns++;
      lturns = 0;
      delay(500);
      m2.stop();
      do{
        m1.run(-regSpeed);
     }while(left.readSensors() != 2 && right.readSensors() != 1);
  }
  if(left.readSensors()==0 && lturns<=3){
      turnaround = 0;
      lturns++;
      rturns = 0;
      delay(500);
      m1.stop();
      do{
        m2.run(regSpeed);
    }while(left.readSensors() != 2 && right.readSensors() != 1);
  }
  if(left.readSensors()==0 && right.readSensors()==0){
    turnaround = 0;
    m1.run(-regSpeed);
    m2.run(regSpeed);
    delay(1000);
    if(left.readSensors()==0 && right.readSensors()==0){
       exit(0); 
    }else{
      if(left.readSensors()==2 || right.readSensors()==1){
        m1.run(-regSpeed);
        m2.run(regSpeed);
      }else{
      
      m1.run(regSpeed);
      m2.run(-regSpeed);
      delay(2000);
      
      if(lturns>=3){
        lturns = 0;
        rturns = 1;
        do{
          m2.stop();
          m1.run(-regSpeed);
        }while(left.readSensors() != 2 && right.readSensors() != 1);
      }
      if(rturns>=3){
        rturns = 0;
        lturns = 1;
        do{
          m1.stop();
          m2.run(regSpeed);
        }while(left.readSensors() != 2 && right.readSensors() != 1);
      }
      if(rturns<3 && lturns<3){
        rturns++;
        do{
          m2.stop();
          m1.run(-regSpeed);
        }while(left.readSensors() != 2 && right.readSensors() != 1);
      }
    }
  }
  }
  if(left.readSensors()==3 && right.readSensors()==3){
     turnaround++;
     Serial.println("DANGER WILL ROBINSON");
     if(turnaround>1){
      do{
        m1.run(2*regSpeed);
        m2.run(2*regSpeed);
      }while(left.readSensors()!=2 && right.readSensors()!=1);
     }
  }
  if(left.readSensors()==3 && right.readSensors()!=3){
    turnaround = 0;
    m2.run(regSpeed);
    m1.run(-regSpeed - 50);
  }
  if(right.readSensors()==3 && left.readSensors()!=3){
    turnaround = 0;
    m1.run(-regSpeed);
    m2.run(regSpeed + 50);
  }
}
