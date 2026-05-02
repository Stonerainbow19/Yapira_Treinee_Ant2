#include <Arduino.h>


#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

// Definicao de pinos
const int IN1 = 4, IN2 = 5, IN3 = 6, IN4 = 7, WEAPON1 = 12, WEAPON2 = 8, ENC = 9, ENA = 11, ENB = 10;
int speed,spdWeapon, coeficienteSpd = 1;
bool onWeapon = false, WEAPON1_DIR = LOW, WEAPON2_DIR = HIGH; 
unsigned long lastPress = 0, lastPress1 = 0;


void moveRobot(int spd1, int spd2){

  if (spd1 > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else if (spd1 < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  if (spd2 > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (spd2 < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

  analogWrite(ENA, abs(spd1));
  analogWrite(ENB, abs(spd2));

}

void moveRobotJoystick(float x_axis, float y_axis){

  int forward = (int) map(y_axis, -7, 7, -255, 255);
  int turn = (int) map(x_axis, -7,7,-255,255);

  int spd1 =  forward + turn;
  int spd2 = forward - turn;


  spd1 = constrain(spd1, -255, 255) * coeficienteSpd;
  spd2 = constrain(spd2, -255, 255) * coeficienteSpd;

  moveRobot(spd1, spd2);
}

void setup() {
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(WEAPON1, OUTPUT);
  pinMode(WEAPON2, OUTPUT);
  
  spdWeapon = 200;

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
  if((x != 0) || (y != 0)) {
      moveRobotJoystick(x,y);
  }
  // Modo - Dpad
  else{
    speed = 255 * coeficienteSpd;
    if (GamePad.isUpPressed())
    {
      Serial.println("Up");
      moveRobot(speed, speed);
    }
    else if (GamePad.isDownPressed())
    {
      Serial.print("Down");
      moveRobot(-speed, -speed);
    }
    else if (GamePad.isLeftPressed())
    { 
      moveRobot(-speed, speed);
    }
    else if (GamePad.isRightPressed())
    {
      moveRobot(speed,-speed);

    } else {
      moveRobot(0,0);
    }

  }

  // Inverter movimento
  if (GamePad.isCrossPressed())
  { 
    if(millis() - lastPress1 > 300){
      coeficienteSpd *= -1;
      WEAPON1_DIR = !WEAPON1_DIR;
      WEAPON2_DIR = !WEAPON2_DIR;
      lastPress1 = millis();
    }
  }

  // Controle da arma
  if (GamePad.isTrianglePressed())
  { 
    if(millis() - lastPress > 300){
      onWeapon = !onWeapon;
      lastPress = millis();
    }
  }

  if(onWeapon){
    digitalWrite(WEAPON1, WEAPON1_DIR);
    digitalWrite(WEAPON2, WEAPON2_DIR);
    analogWrite(ENC, spdWeapon);
  } else{
    digitalWrite(WEAPON1, LOW);
    digitalWrite(WEAPON2, LOW);
    analogWrite(ENC, 0); 
  }

    
}






