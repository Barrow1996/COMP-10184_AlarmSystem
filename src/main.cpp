// COMP-10184 - Mohawk College
// Alarm System
//STAGE 5 COMPLETED
//@author Shawn Barrow
//@id     000332631
//
// I, Shawn Barrow, 000332631 certify that this material is my original work and
//no other person's work has been used without due acknowledgement.

#include <Arduino.h>
//Ditial input for motion sensor
#define PIN_PIR D5
//Digital input for push button
#define PIN_BUTTON D6
//Constants used to track alarm states
#define ALARM_DISABLED 0
#define ALARM_ENABLE 1
#define ALARM_COUNTDOWN 2
#define ALARM_ACTIVE 3
//Delay to control number of blinks per second
#define BLINK_DELAY 125
//Delay to read push button state
#define READ_DELAY 100
//Number of blinks in 10 seconds.
#define BLINK_COUNT 40

int pushButton;
int alarmState;
bool bPIR;
void setup() {
  // configure the USB serial monitor
  Serial.begin(115200);
  // configure the LED output
  pinMode(LED_BUILTIN, OUTPUT);
  // Pushbutton
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  // PIR sensor
  pinMode(PIN_PIR, INPUT);

  alarmState = ALARM_ENABLE;
  bPIR = false;
  pushButton = true;
}

//This function flashes the light 4 times per second for 10 seconds, before the light remains solid.
// if the button is pressed during the 10 seconds the alarm is cancelled and the light goes out.
void flash() {
  int count = 0;
  pushButton = true;
  while (count != BLINK_COUNT && pushButton)
    {
      pushButton = digitalRead(PIN_BUTTON);
      delay(1);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(BLINK_DELAY);
      digitalWrite(LED_BUILTIN, LOW);
      delay(BLINK_DELAY);
      count++;
    }

  //4 blinks X 10 seconds
  if(count == BLINK_COUNT){
    alarmState = ALARM_ACTIVE;
  }
  else{
    digitalWrite(LED_BUILTIN, HIGH);
    alarmState = ALARM_DISABLED;
  }
}

// Switch alarm states
void loop() {
  
  switch(alarmState) 
  {
  case ALARM_DISABLED:
    pushButton = digitalRead(PIN_BUTTON);
    delay(READ_DELAY);
      if(!pushButton) alarmState = ALARM_ENABLE;
    break;

  case ALARM_ENABLE:
    digitalWrite(LED_BUILTIN, HIGH);
    bPIR = digitalRead(PIN_PIR);
    delay(READ_DELAY);
      if(bPIR) alarmState = ALARM_COUNTDOWN;
    break;
    
  case ALARM_COUNTDOWN:
    flash();
    break;
    
  case ALARM_ACTIVE:
    digitalWrite(LED_BUILTIN, false);
    break;
  }
}
