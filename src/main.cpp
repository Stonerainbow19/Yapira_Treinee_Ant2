#include <Arduino.h>

int IN1, IN2, IN3, IN4;
int led;




void setup() {
  pinMode(led, OUTPUT);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {

  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  
}


