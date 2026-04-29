#include <Arduino.h>


#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#include <Dabble.h>

// Definicao de pinos
const int IN1 = 4, IN2 = 5, IN3 = 6, IN4 = 7, WEAPON1 = 10, WEAPON2 = 8, ENC = 9, ENA = A2, ENB = A1;
int spd,spdWeapon, coeficienteSpd = 1;
bool onWeapon = true; 
unsigned long lastPress = 0, lastPress1 = 0;


void moveRobot(int spd1, int spd2){
  // Mova para frente
  if (spd1 > 0 && spd2 > 0){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  }
  // Mova para tras
  else if (spd1 < 0 && spd2 < 0){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

  }
  // Curva direita
  else if (spd1 > 0 && spd2 < 0){
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);

  }
  // Curva esquerda
  else if (spd1 < 0 && spd2 > 0){
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

  }
  // Pare
  else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

  analogWrite(ENA, abs(spd1));
  analogWrite(ENB, abs(spd2));

}

void moveRobotJoystick(float x_axis, float y_axis){
  // Ajeitando o valor do x e y
  x_axis = (int) x_axis;
  y_axis = (int) y_axis;


  int forward = map(y_axis, -7, 7, -255, 255);
  int turn = map(x_axis, -7,7,-255,255);

  int spd1 =  forward + turn;
  int spd2 = forward - turn;


  spd1 = constrain(spd1, -255, 255);
  spd2 = constrain(spd2, -255, 255);

  moveRobot(coeficienteSpd*spd1, coeficienteSpd*spd2);
}

void setup() {
  
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
  if((x != 0) || (y != 0)) {
      moveRobotJoystick(x,y);
  }
  // Modo - Dpad
  else{
    spd = 255;
    if (GamePad.isUpPressed())
    {
      Serial.println("Up");
      moveRobot(spd*coeficienteSpd,spd*coeficienteSpd);
    }
    else if (GamePad.isDownPressed())
    {
      Serial.print("Down");
      moveRobot(-spd*coeficienteSpd,-spd*coeficienteSpd);
    }
    else if (GamePad.isLeftPressed())
    {
      Serial.print("Left");
      moveRobot(-spd*coeficienteSpd, spd*coeficienteSpd);
    }
    else if (GamePad.isRightPressed())
    {
      Serial.print("Right");
      moveRobot(spd * coeficienteSpd, -spd*coeficienteSpd);
    } else {
      moveRobot(0,0);
    }

  }


  // Controle da arma
  if (GamePad.isCirclePressed())
  { 
    if(millis() - lastPress > 300){
      onWeapon = !onWeapon;
      lastPress = millis();
      Serial.print("O");
    }
  }

  if(onWeapon){
    digitalWrite(WEAPON1, HIGH);
    digitalWrite(WEAPON2, LOW);
    analogWrite(ENC, spdWeapon);
  } else{
    digitalWrite(WEAPON1, LOW);
    digitalWrite(WEAPON2, LOW);
    analogWrite(ENC, 0); 
  }

    
  // Inverter movimento
  if (GamePad.isCrossPressed())
  { 
    if(millis() - lastPress1 > 300){
      coeficienteSpd *= -1;

      lastPress1 = millis();
      Serial.print("X");
    }
  }

}






