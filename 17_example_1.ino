#include <Servo.h>

// Arduino pin assignment
#define PIN_LED 9
#define PIN_POTENTIOMETER 3 // Potentiometer at Pin A3

// Add IR Sensor Definition Here !!!
#define IR_Sensor 0
#define PIN_SERVO 10

#define _DUTY_MIN 550   // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1475 // servo neutral position (90 degree)
#define _DUTY_MAX 2400 // servo full counter-clockwise position (180 degree)

#define DIST_MIN 100
#define DIST_MAX 250

#define LOOP_INTERVAL 20  // Loop Interval (unit: msec)
#define _EMA_ALPHA 0.5

Servo myservo;
unsigned long last_loop_time ;   // unit: msec

float dist;
float dist_prev = DIST_MAX;
float dist_ema = DIST_MAX;

void setup()
{
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  pinMode(PIN_LED, OUTPUT);
  
  Serial.begin(1000000);
}

void loop()
{
  unsigned long time_curr = millis();
  int duty, a_value;

  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Read IR Sensor value !!!
  a_value = analogRead(IR_Sensor);
  
  // Convert IR sensor value into distance !!!
  dist = (6762.0/(a_value-9.0)-4.0)*10.0-60.0;
  
  // we need distance range filter here !!!
  if(dist >= DIST_MIN && dist <= DIST_MAX){
    digitalWrite(PIN_LED, 0);
    dist_prev = dist;
  }
  else if(dist < DIST_MIN){
    digitalWrite(PIN_LED,1); //LED OFF
    dist = dist_prev;
  }
  else{
    digitalWrite(PIN_LED,1);
    dist = dist_prev;
  }
  // we need EMA filter here !!!
  dist_ema = (1 - _EMA_ALPHA)*dist_ema + dist*_EMA_ALPHA;
  duty = int (1850.0 * (dist_ema - 100.0)/150 + 550); //0~1023 -> 550~2400
  //myservo.writeMicroseconds(duty);

  if(dist_ema > DIST_MIN && dist_ema < DIST_MAX){
    myservo.writeMicroseconds(duty);
  }
  else if(dist_ema <= DIST_MIN){
    myservo.writeMicroseconds(_DUTY_MIN);
  }
  else{
    myservo.writeMicroseconds(_DUTY_MAX);
    dist = DIST_MAX;
  }

  // print IR sensor value, distnace, duty !!!

  Serial.print("MIN: ");  Serial.print(DIST_MIN);
  Serial.print(",IR: ");  Serial.print(a_value);
  Serial.print(",dist: ");  Serial.print(dist);
  Serial.print(",ema: ");  Serial.print(dist_ema);
  Serial.print(",servo: ");  Serial.print(duty);
  Serial.print(",MAX: ");  Serial.print(DIST_MAX);
  Serial.println("");
}
