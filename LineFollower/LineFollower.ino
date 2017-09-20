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


void straight(){
  turnaround = 0;
  m1.run(-regSpeed);
  m2.run(regSpeed);
}

void adjleft(){
  turnaround = 0;
  m1.run(-regSpeed);
  m2.run(regSpeed + 50);
}


void adjright(){
  turnaround = 0;
  m2.run(regSpeed);
  m1.run(-regSpeed - 50);    
}
void deadend(){
  turnaround++;
  if(turnaround>20){
    do{
      m1.run(2*regSpeed);
      m2.run(2*regSpeed);
    }while(left.readSensors()!=2 && right.readSensors()!=1);
  }
}
void leftturn(){
  turnaround = 0;
  lturns++;
  rturns = 0;
  delay(500);
  m1.stop();
  do{
  m2.run(regSpeed);
  }while(left.readSensors() != 2 && right.readSensors() != 1);
}
void rightturn(){
  turnaround = 0;
  rturns++;
  lturns = 0;
  delay(500);
  m2.stop();
  do{
    m1.run(-regSpeed);
  }while(left.readSensors() != 2 && right.readSensors() != 1);
}

void backup(){
  m1.run(regSpeed);
  m2.run(-regSpeed);
}



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  if(left.readSensors()==2 && right.readSensors()==1){
      straight();
  }
  if(right.readSensors()==2 && rturns<=3){
      rightturn();
  }
  if(left.readSensors()==0 && lturns<=3){
      leftturn();
  }
  if(left.readSensors()==0 && right.readSensors()==0){
    straight();
    delay(1000);
    if(left.readSensors()==0 && right.readSensors()==0){
       exit(0); 
    }else{
      if(left.readSensors()==2 || right.readSensors()==1){
        straight();
      }else{
        backup();
        delay(2000);
        if(lturns>=3){
          rightturn();
        }
        if(rturns>=3){
          leftturn();
        }
        if(rturns<3 && lturns<3){
          rightturn();
        }
      }
    }
  }
  if(left.readSensors()==3 && right.readSensors()==3){
     deadend();
  }
  if(left.readSensors()==3 && right.readSensors()!=3){
    adjright();
  }
  if(right.readSensors()==3 && left.readSensors()!=3){
    adjleft();
  }
}
