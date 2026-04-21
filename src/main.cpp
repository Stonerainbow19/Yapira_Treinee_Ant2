#include <Arduino.h>


#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

// Definicao de pinos

const int IN1 = 4, IN2 = 5, IN3 = 6, IN4 = 7, WEAPON1 = 10, WEAPON2 = 8, ENC = 9, ENA = A2, ENB = A1, led = 13;
int spdRightMotor, spdLeftMotor, spdWeapon;
bool offWeapon = true;


void moveRobot(int spd1, int spd2){
  // Move Forward
  if (spd1 > 0 && spd2 > 0){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, spd1);
    analogWrite(ENB, spd2);
  }
  // Move Backward
  else if (spd1 < 0 && spd2 < 0){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, -spd1);
    analogWrite(ENB, -spd2);
  }
  // Turn Right
  else if (spd1 > 0 && spd2 < 0){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENA, spd1);
    analogWrite(ENB, -spd2);
  }
  // Turn Left
  else if (spd1 < 0 && spd2 > 0){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, -spd1);
    analogWrite(ENB, spd2);
  }
  // Stop
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
  }

}

void moveRobotJoystick(float x_axis, float y_axis){
  // Ajeitando o valor do x e y
  x_axis = x_axis / 7;
  y_axis = y_axis / 7;

  int spd1 = (int)(y_axis * 255);
  int spd2 = (int)(y_axis * 255);

  if (x_axis > 0) { 
    spd1 -= (int)(x_axis * 255);
    spd2 += (int)(x_axis * 255);
  } else if (x_axis < 0) { 
    spd1 += (int)(-x_axis * 255);
    spd2 -= (int)(-x_axis * 255);
  }

  spd1 = constrain(spd1, -255, 255);
  spd2 = constrain(spd2, -255, 255);

  moveRobot(spd1, spd2);
}

void setup() {
  
  pinMode(led, OUTPUT);
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(WEAPON1, OUTPUT);
  pinMode(WEAPON2, OUTPUT);
  
  spdWeapon = 255;

  Serial.begin(19200);
  Dabble.begin(9600);
}

void loop() {
   Dabble.processInput(); 
  
  float x = GamePad.getXaxisData();
  Serial.print("x_axis: ");
  Serial.print(x);
  Serial.print('\t');
  float y = GamePad.getYaxisData();
  Serial.print("y_axis: ");
  Serial.println(y);
  Serial.println();


  // Modo - Joystick
  if((x != 0) && (y != 0)) {
      moveRobotJoystick(x,y);
  }
  // Modo - Dpad
  else{
    spdRightMotor = 255;
    spdLeftMotor = 255;
    if (GamePad.isUpPressed())
    {
      Serial.println("Up");
    }
    else if (GamePad.isDownPressed())
    {
      Serial.print("Down");
      spdRightMotor = -spdRightMotor;
      spdLeftMotor = -spdLeftMotor;
    }
    else if (GamePad.isLeftPressed())
    {
      Serial.print("Left");
      spdLeftMotor = -spdLeftMotor;
    }

    else if (GamePad.isRightPressed())
    {
      Serial.print("Right");
      spdRightMotor = -spdRightMotor;
    } else {
       spdRightMotor = 0;
       spdLeftMotor = 0;
    }
    moveRobot(spdLeftMotor,spdRightMotor);

  }


  
  // Controle da arma
  unsigned long lastPress = 0;
  if (GamePad.isCirclePressed())
  { 
    if(millis() - lastPress > 300){
      offWeapon = !offWeapon;
      lastPress = millis();
      Serial.print("Circle");
    }
  }

  if(offWeapon){
    digitalWrite(WEAPON1, HIGH);
    digitalWrite(WEAPON2, LOW);
    analogWrite(ENC, spdWeapon);
  } else{
    digitalWrite(WEAPON1, HIGH);
    digitalWrite(WEAPON2, HIGH);
    analogWrite(ENC, 0); 
  }
}






