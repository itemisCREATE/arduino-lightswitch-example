#include <avr/sleep.h>
#include <avr/power.h>

#include <CPPTimerinterface.h>
#include <Lightswitch.h>
#include <sc_types.h>
#include <StatemachineInterface.h>
#include <TimedStatemachineInterface.h>
#include <TimerInterface.h>

Lightswitch lightswitch;
CPPTimerInterface* timerInterface;
volatile bool buttonPressed = false;
volatile long last_button = -201;
long time_ms;

void setup() {
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(2, INPUT);
  pinMode(7, INPUT);

  TimerTask* tasks[Lightswitch::parallelTimeEventsCount];
  timerInterface = new CPPTimerInterface(tasks, Lightswitch::parallelTimeEventsCount);
  lightswitch.setTimer(timerInterface);

  lightswitch.init();
  lightswitch.enter();

  attachInterrupt(digitalPinToInterrupt(2), buttonISR, RISING);
  time_ms = millis();
}

void loop() {
  // check time & proceed timer service
  long now = millis();
  if(now - time_ms > 0) {
    timerInterface->proceed(now - time_ms);
    time_ms = millis();
  }

  // handle button press from ISR
  if(buttonPressed) {
    lightswitch.raise_button();
    buttonPressed = false;
  }

  // read out motion sensor
  if(digitalRead(7)) {
    lightswitch.raise_motion();
  }

  // set light
  digitalWrite(13, lightswitch.get_light());
  // set mode LEDs
  digitalWrite(9, lightswitch.get_led_timer());
  digitalWrite(10, lightswitch.get_led_motion());
  // if in Off-state, go to sleep (wake up by ISR)
  if(lightswitch.isStateActive(Lightswitch::lightswitch_Off)) {
    enterSleep();
  }
}

void buttonISR() {
  long now = millis();
  if(now - last_button > 200) {
    buttonPressed = true;
    last_button = now;
  }
}

void enterSleep(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();
  // continue after interrupt
  sleep_disable();
  power_all_enable();
}  
