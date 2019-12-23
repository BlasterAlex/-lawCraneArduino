/***
 * За алгоритм степпера огромное спасибо:
 * Дмитрию Осипову http://www.youtube.com/user/d36073?feature=watch
 */

#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

class StepperMotor {

  int motorPin1; // Blue / Синий - 28BYJ48 pin 1
  int motorPin2; // Pink  / Розовый - 28BYJ48 pin 2
  int motorPin3; // Yellow / Желтый - 28BYJ48 pin 3
  int motorPin4; // Orange  / Оранжевый - 28BYJ48 pin 4
  int reverseSpeed; // задержка между итерациями (микросекунды) 1200 - 10000

  // Для вращения степпера
  const int lookup[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};

  // Вращение - считывает байт, начиная справа
  void setOutput(int out) {
    digitalWrite(motorPin1, bitRead(lookup[out], 0));
    digitalWrite(motorPin2, bitRead(lookup[out], 1));
    digitalWrite(motorPin3, bitRead(lookup[out], 2));
    digitalWrite(motorPin4, bitRead(lookup[out], 3));
  }

public:
  // Создание объекта класса
  StepperMotor(int pin1, int pin2, int pin3, int pin4, int revSpeed) {
    motorPin1 = pin1;
    motorPin2 = pin2;
    motorPin3 = pin3;
    motorPin4 = pin4;
    reverseSpeed = revSpeed;
  }

  // Инициализация класса степпера
  void setup() {
    pinMode(motorPin1, OUTPUT);
    pinMode(motorPin2, OUTPUT);
    pinMode(motorPin3, OUTPUT);
    pinMode(motorPin4, OUTPUT);
  }

  // По часовой стрелке
  void clockwise(int steps) {
    while (--steps >= 0)
      for (int i = 7; i >= 0; i--) {
        setOutput(i);
        delayMicroseconds(reverseSpeed);
      }
  }

  // Против часовой стрелки
  void counterclockwise(int steps) {
    while (--steps >= 0)
      for (int i = 0; i < 8; i++) {
        setOutput(i);
        delayMicroseconds(reverseSpeed);
      }
  }
};

#endif // STEPPERMOTOR_H
