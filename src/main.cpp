#include <Arduino.h>
#include <StepperMotor.hpp>
#include <timer-api.h>

#define GAMEDURATION 7000 // ms
#define DEBOUNCE 200
#define STEPS 500
// #define STEPS 3000

// Номера подключенных портов
int piezo = 5;
int led = 6;
int button = 7;
StepperMotor stepper{10, 11, 12, 13, 1200};

// Состояние кнопки
bool preBtnState = LOW;
long pressingTime = 0;

// Состояние игры
bool isPlaying = false;
bool gameOver = false;
int startTime;

// Количество оставшихся попыток
int attemptsLeft = 1;

// Функция таймера
void timer_handle_interrupts(int timer) {
  int timePast = millis() - startTime;

  if (timePast < GAMEDURATION)
    Serial.println("Осталось " + String(round((float)(GAMEDURATION - timePast) / 1000)) + "s");
  else if (!gameOver) {
    Serial.println("Game over");
    gameOver = true;
  }
}

// Функция перезагрузки программы
void (*Reset)(void) = 0; // объявляем функцию reset с адресом 0

// Начальные установки
void setup() {
  Serial.begin(9600);

  stepper.setup();
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  pinMode(piezo, OUTPUT);

  Serial.println("Нажмите на кнопку для начала игры");
}

// Главный цикл программы
void loop() {

  // Текущее состояние кнопки
  const int stateButton = digitalRead(button);

  // Кнопка нажата
  if (stateButton == HIGH && preBtnState == LOW && millis() - pressingTime > DEBOUNCE) {

    // Если игра не начата
    if (!isPlaying) {

      // Начать новую игру
      isPlaying = true;
      gameOver = false;
      attemptsLeft = 1;
      startTime = millis();

      // Включить лампочку
      digitalWrite(led, HIGH);

      // Воспроизвести звук
      tone(piezo, 1000, 500);

      // Начать отсчет времени
      timer_init_ISR_1Hz(TIMER_DEFAULT);

    } else if (attemptsLeft-- > 0) { // в игре, остались доступные попытки
      stepper.clockwise(STEPS); // раскрутить степпер по часовой стрелке
      stepper.counterclockwise(STEPS); // закрутить степпер против часовой стрелки
    }

    // Сохранение времени последнего нажатия кнопки
    pressingTime = millis();
  }

  // Конец игры
  if (isPlaying && millis() - startTime >= GAMEDURATION) {
    isPlaying = false;

    // Выключить лампочку
    digitalWrite(led, LOW);

    // Воспроизвести звук
    tone(piezo, 2000, 500);

    // Остановить отсчет времени
    timer_stop_ISR(TIMER_DEFAULT);

    // Вывести результат
    if (!gameOver) {
      Serial.println("Game over");
      gameOver = true;
    }

    // Перезагрузка контролера
    delay(1000);
    Reset();
  }

  // Сохрение состояния кнопки
  preBtnState = stateButton;
}