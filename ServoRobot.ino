/*
  Скетч для серво робота из
  8 серв (2 сервы на ногу),
  Arduino Nano,
  Серво шилда PCA9685,
  Напечатонного корпуса

  By Dairis Mivreniks 2020
*/


// ************************ НАСТРОЙКИ *****************************

#define SETUP 0              // при включении все сервоприводы поворачиваются на исходную позицию

#define SETUP_ANG 5     // угол ног для исходной позиций (от 0 до 180)
#define SETUP_ANG_K 90  // угол колен в исходной позиций в setup 2
#define SETUP_ANG_L 0   // угол колен в исходной позиций в setup 1

// пины на серво шилде к которым подключены сервоприводы
// сервы ног
#define SERVO1 0  // 1 нога
#define SERVO2 1  // 2 нога
#define SERVO3 2  // 3 нога
#define SERVO4 3  // 4 нога
#define SERVO5 4  // колено 1-ой ноги
#define SERVO6 5  // колено 2-ой ноги
#define SERVO7 6  // колено 3-ей ноги
#define SERVO8 7  // колено 4-ой ноги


#define MAX_ANGLE 65 // угол для ног в переднем положении
#define MIN_ANGLE 10 // угол для ног в задним положении

#define F_ANGLE 40  // угла шага поворота при поворачивании в сторону
#define S_ANGLE 20

#define MAX_K_ANGLE 140 // угол для колен в верхним положении
#define MIN_K_ANGLE 75  // угол для колен в нижнем положении


#define SERVOMIN  145 // минимальный импульс для поворота в милисекундах (для sg90 145)
#define SERVOMAX  515 // максимальный импульс для поворота в милисекундах (для sg90 515)


#define MOVE_TIME 200 // время в милисекундах для смены ног


// *********************** ДЛЯ РАЗРАБОТЧИКОВ ***********************

unsigned long timer;

boolean flag;
byte blFlag;
byte mode;


int angle;
int angle_z;
int angle_k;
int angle_k_z;
int max_angle;
int min_angle;
int max_angle_z;
int min_angle_z;
int f_angle;
int f_angle_z;
int s_angle;
int s_angle_z;
int max_k_angle;
int min_k_angle;
int max_k_angle_z;
int min_k_angle_z;
int setup_ang_l;
int setup_ang_l_z;

String strData = "";
String blinput;
boolean recievedFlag;



#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <SoftwareSerial.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

SoftwareSerial bluetooth(10, 9);


void setup() {

  Serial.begin(9600);
  bluetooth.begin(9600);


  pwm.begin();
  pwm.setPWMFreq(50);



  angle = map(SETUP_ANG, 0, 180, SERVOMIN, SERVOMAX);
  angle_z = map(SETUP_ANG, 180, 0, SERVOMIN, SERVOMAX);
  angle_k = map(SETUP_ANG_K, 0, 180, SERVOMIN, SERVOMAX);
  angle_k_z = map(SETUP_ANG_K, 180, 0, SERVOMIN, SERVOMAX);
  max_angle = map(MAX_ANGLE, 0, 180, SERVOMIN, SERVOMAX);
  min_angle = map(MIN_ANGLE, 0, 180, SERVOMIN, SERVOMAX);
  max_angle_z = map(MAX_ANGLE, 180, 0, SERVOMIN, SERVOMAX);
  min_angle_z = map(MIN_ANGLE, 180, 0, SERVOMIN, SERVOMAX);
  s_angle = map(S_ANGLE, 0, 180, SERVOMIN, SERVOMAX);
  s_angle_z = map(S_ANGLE, 180, 0, SERVOMIN, SERVOMAX);
  f_angle = map(F_ANGLE, 0, 180, SERVOMIN, SERVOMAX);
  f_angle_z = map(F_ANGLE, 180, 0, SERVOMIN, SERVOMAX);
  max_k_angle = map(MAX_K_ANGLE, 0, 180, SERVOMIN, SERVOMAX);
  min_k_angle = map(MIN_K_ANGLE, 0, 180, SERVOMIN, SERVOMAX);
  max_k_angle_z = map(MAX_K_ANGLE, 180, 0, SERVOMIN, SERVOMAX);
  min_k_angle_z = map(MIN_K_ANGLE, 180, 0, SERVOMIN, SERVOMAX);
  setup_ang_l = map(SETUP_ANG_L, 0, 180, SERVOMIN, SERVOMAX);
  setup_ang_l_z = map(SETUP_ANG_L, 180, 0, SERVOMIN, SERVOMAX);

}

void loop() {

  // если включен сетуп то повернуть все сервы в исходную позицию
#if (SETUP == 1)
  pwm.setPWM(SERVO1, 0, angle_z);
  delay(100);
  pwm.setPWM(SERVO2, 0, angle);
  delay(100);
  pwm.setPWM(SERVO3, 0, angle_z);
  delay(100);
  pwm.setPWM(SERVO4, 0, angle);
  delay(100);
  pwm.setPWM(SERVO5, 0, setup_ang_l_z);
  pwm.setPWM(SERVO6, 0, setup_ang_l);
  pwm.setPWM(SERVO7, 0, setup_ang_l_z);
  pwm.setPWM(SERVO8, 0, setup_ang_l);

#endif
#if (SETUP == 2)
  pwm.setPWM(SERVO1, 0, angle);
  delay(100);
  pwm.setPWM(SERVO2, 0, angle);
  delay(100);
  pwm.setPWM(SERVO3, 0, angle);
  delay(100);
  pwm.setPWM(SERVO4, 0, angle);
  delay(100);
  pwm.setPWM(SERVO5, 0, angle_k_z);
  pwm.setPWM(SERVO6, 0, angle_k);
  pwm.setPWM(SERVO7, 0, angle_k_z);
  pwm.setPWM(SERVO8, 0, angle_k);
#endif
#if (SETUP == 0)

  //******************************* БЛЮТУЗ *****************************************

  while (bluetooth.available() > 0)   //if bluetooth module is transmitting data
  {
    strData += (char)bluetooth.read();
    recievedFlag = true;
    blinput = strData;
    delay(2);
  }

  if (recievedFlag) {
    Serial.println(strData);
    strData = "";
    recievedFlag = false;
  }

  //**************************************************************************

  if (blinput == "F") {
    mode = 1;
  }
  else if (blinput == "S") {
    mode = 0;
  }
  else if (blinput == "B") {
    mode = 2;
  }
  else if (blinput == "R") {
    mode = 3;
  }
  else if (blinput == "L") {
    mode = 4;
  }
  else if (blinput == "W") {
    blFlag = 1;
  }
  else if (blinput == "w") {
    blFlag = 0;
  }
  else if (blinput == "U") {
    blFlag = 2;
  }
  else if (blinput == "u") {
    blFlag = 0;
  }
  else mode = 99;


  if (blFlag == 1) {
    down();
  }

  if (blFlag == 2) {
    jump();
  }


  if (!blFlag) {
    switch (mode) {
      case 0: stape();
        break;
      case 1: front();
        break;
      case 2: back();
        break;
      case 3: right();
        break;
      case 4: left();
        break;
    }


  }
#endif
}


//********************************** ФУНКЦИИ НАПРАВЛЕНИЕ ****************************

void front() {

  if (millis() - timer > MOVE_TIME) {
    flag = !flag;
    timer = millis();
  }

  if (flag) {
    firstUp();
    delay(120);
    firstGoing();
  }

  else if (!flag) {
    secondUp();
    delay(120);
    secondGoing();
  }

}


void stape() {

  pwm.setPWM(SERVO1, 0, max_angle_z);
  delay(100);
  pwm.setPWM(SERVO2, 0, max_angle);
  delay(100);
  pwm.setPWM(SERVO3, 0, max_angle_z);
  delay(100);
  pwm.setPWM(SERVO4, 0, max_angle);
  delay(100);
  pwm.setPWM(SERVO5, 0, angle_k_z);
  pwm.setPWM(SERVO6, 0, angle_k);
  pwm.setPWM(SERVO7, 0, angle_k_z);
  pwm.setPWM(SERVO8, 0, angle_k);

}

void back() {

  if (millis() - timer > MOVE_TIME) {
    flag = !flag;
    timer = millis();
  }

  if (flag) {
    firstUp();
    delay(120);
    secondGoing();
  }

  else if (!flag) {
    secondUp();
    delay(120);
    firstGoing();
  }

}

void right() {

  pwm.setPWM(SERVO5, 0, max_k_angle_z);
  delay(100);
  pwm.setPWM(SERVO3, 0, f_angle_z);
  pwm.setPWM(SERVO2, 0, f_angle);
  pwm.setPWM(SERVO1, 0, min_angle_z);
  pwm.setPWM(SERVO4, 0, min_angle);
  delay(150);
  pwm.setPWM(SERVO5, 0, min_k_angle_z);
  delay(100);
  pwm.setPWM(SERVO8, 0, max_k_angle);
  delay(100);
  pwm.setPWM(SERVO4, 0, max_angle);
  pwm.setPWM(SERVO2, 0, s_angle);
  pwm.setPWM(SERVO1, 0, s_angle_z);
  pwm.setPWM(SERVO3, 0, max_angle_z);
  delay(150);
  pwm.setPWM(SERVO8, 0, min_k_angle);
  delay(100);
  pwm.setPWM(SERVO7, 0, max_k_angle_z);
  delay(100);
  pwm.setPWM(SERVO1, 0, f_angle_z);
  pwm.setPWM(SERVO4, 0, f_angle);
  pwm.setPWM(SERVO3, 0, min_angle_z);
  pwm.setPWM(SERVO2, 0, min_angle);
  delay(150);
  pwm.setPWM(SERVO7, 0, min_k_angle_z);
  delay(100);
  pwm.setPWM(SERVO6, 0, max_k_angle);
  delay(100);
  pwm.setPWM(SERVO1, 0, max_angle_z);
  pwm.setPWM(SERVO4, 0, s_angle);
  pwm.setPWM(SERVO3, 0, s_angle_z);
  pwm.setPWM(SERVO2, 0, max_angle);
  delay(150);
  pwm.setPWM(SERVO6, 0, min_k_angle);
  delay(100);

}

void left() {

  pwm.setPWM(SERVO5, 0, max_k_angle_z);
  delay(100);
  pwm.setPWM(SERVO3, 0, s_angle_z);
  pwm.setPWM(SERVO2, 0, s_angle);
  pwm.setPWM(SERVO1, 0, max_angle_z);
  pwm.setPWM(SERVO4, 0, max_angle);
  delay(150);
  pwm.setPWM(SERVO5, 0, min_k_angle_z);
  delay(100);
  pwm.setPWM(SERVO8, 0, max_k_angle);
  delay(100);
  pwm.setPWM(SERVO4, 0, min_angle);
  pwm.setPWM(SERVO2, 0, f_angle);
  pwm.setPWM(SERVO1, 0, f_angle_z);
  pwm.setPWM(SERVO3, 0, min_angle_z);
  delay(150);
  pwm.setPWM(SERVO8, 0, min_k_angle);
  delay(100);
  pwm.setPWM(SERVO7, 0, max_k_angle_z);
  delay(100);
  pwm.setPWM(SERVO1, 0, s_angle_z);
  pwm.setPWM(SERVO4, 0, s_angle);
  pwm.setPWM(SERVO3, 0, max_angle_z);
  pwm.setPWM(SERVO2, 0, max_angle);
  delay(150);
  pwm.setPWM(SERVO7, 0, min_k_angle_z);
  delay(100);
  pwm.setPWM(SERVO6, 0, max_k_angle);
  delay(100);
  pwm.setPWM(SERVO1, 0, min_angle_z);
  pwm.setPWM(SERVO4, 0, f_angle);
  pwm.setPWM(SERVO3, 0, f_angle_z);
  pwm.setPWM(SERVO2, 0, min_angle);
  delay(150);
  pwm.setPWM(SERVO6, 0, min_k_angle);
  delay(100);
}


void jump() {

  delay(100);
  down();
  delay(3000);
  up();
  delay(5000);

}

// ****************************** ФУНКЦИИ ДЛЯ СЕРВ ***********************

void firstGoing() {

  pwm.setPWM(SERVO1, 0, max_angle_z);
  pwm.setPWM(SERVO2, 0, min_angle);
  pwm.setPWM(SERVO3, 0, min_angle_z);
  pwm.setPWM(SERVO4, 0, max_angle);

}

void secondGoing() {

  pwm.setPWM(SERVO1, 0, min_angle_z);
  pwm.setPWM(SERVO2, 0, max_angle);
  pwm.setPWM(SERVO3, 0, max_angle_z);
  pwm.setPWM(SERVO4, 0, min_angle);

}

void firstUp() {

  pwm.setPWM(SERVO5, 0, max_k_angle_z);
  pwm.setPWM(SERVO7, 0, max_k_angle_z);
  pwm.setPWM(SERVO6, 0, min_k_angle);
  pwm.setPWM(SERVO8, 0, min_k_angle);

}

void secondUp() {

  pwm.setPWM(SERVO6, 0, max_k_angle);
  pwm.setPWM(SERVO8, 0, max_k_angle);
  pwm.setPWM(SERVO5, 0, min_k_angle_z);
  pwm.setPWM(SERVO7, 0, min_k_angle_z);

}

void down() {

  pwm.setPWM(SERVO1, 0, angle_z);
  pwm.setPWM(SERVO2, 0, angle);
  pwm.setPWM(SERVO3, 0, angle_z);
  pwm.setPWM(SERVO4, 0, angle);
  delay(100);
  pwm.setPWM(SERVO5, 0, setup_ang_l_z);
  pwm.setPWM(SERVO6, 0, setup_ang_l);
  pwm.setPWM(SERVO7, 0, setup_ang_l_z);
  pwm.setPWM(SERVO8, 0, setup_ang_l);

}

void up() {

  pwm.setPWM(SERVO5, 0, max_k_angle_z);
  pwm.setPWM(SERVO6, 0, max_k_angle);
  pwm.setPWM(SERVO7, 0, max_k_angle_z);
  pwm.setPWM(SERVO8, 0, max_k_angle);

}
