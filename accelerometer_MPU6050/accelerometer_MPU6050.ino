#include<Wire.h>
#include <SoftwareSerial.h>

const int MPU=0x68;//MPU6050 I2C주소
int AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
void get6050();

SoftwareSerial bt(3, 2);   //bluetooth module Tx:Digital 2 Rx:Digital 3
unsigned long t_prev, t_start;

void setup(){
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);//MPU6050 을 동작 대기 모드로 변경
  Wire.endTransmission(true);
  Serial.begin(9600);

  bt.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, true);

  for(int i=0; i<10 ; i++){
    digitalWrite(13, true);
    delay(100);
    digitalWrite(13, false);
    delay(900);
  }
  bt.println("==Recording start===");
  digitalWrite(13, true);
  t_prev = millis();
  t_start = t_prev;
}

void loop(){
  get6050();//센서값 갱신
  Serial.print("t ");
  Serial.print( (t_prev - t_start) / 1000.0 );
  Serial.print(" a ");
  Serial.println( AcY / 1755.0);
  
  bt.print("t ");
  bt.print( (t_prev - t_start) / 1000.0 );
  bt.print(" a ");
  bt.println( AcY / 1755.0);
  while( t_prev + 100 > millis() ){
    // 100 밀리초 동안 대기
  }
  t_prev += 100;
}

void get6050(){
  Wire.beginTransmission(MPU);//MPU6050 호출
  Wire.write(0x3B);//AcX 레지스터 위치 요청
  Wire.endTransmission(false);
  Wire.requestFrom(MPU,14,true);//14byte의 데이터를 요청
  AcX=Wire.read()<<8|Wire.read();//두개의 나뉘어진 바이트를 하나로 이어붙입니다.
  AcY=Wire.read()<<8|Wire.read();
  AcZ=Wire.read()<<8|Wire.read();
  Tmp=Wire.read()<<8|Wire.read();
  GyX=Wire.read()<<8|Wire.read();
  GyY=Wire.read()<<8|Wire.read();
  GyZ=Wire.read()<<8|Wire.read();
}
