#include <Servo.h>
#define PIN_SERVO 10

#define _DUTY_MIN 552 // 
#define _DUTY_NEU 1476 // 
#define _DUTY_MAX 2400 

#define _SERVO_SPEED 3 // servo angular speed (unit: degree/sec)

// Loop Interval
#define INTERVAL 1000     // servo update interval (unit: msec)

// global variables
unsigned long last_sampling_time; // unit: msec
Servo myservo;

int duty_change_per_interval; // maximum duty difference per interval
int duty_target;    // Target duty time
int duty_curr;      // Current duty time

int toggle_interval, toggle_interval_cnt;
void setup() {
  // put your setup code here, to run once:
  myservo.attach(PIN_SERVO); 
  
  duty_curr = _DUTY_MIN;
  duty_target = _DUTY_MAX;
  myservo.writeMicroseconds(duty_curr);
  Serial.begin(115200); 
  duty_change_per_interval = 
    (float)(_DUTY_MAX - _DUTY_MIN) * (_SERVO_SPEED / 180.0) * (INTERVAL / 1000.0);
  
  // initialize last sampling time
  last_sampling_time = 0;
  
}

void loop() {
  if (millis() < (last_sampling_time + INTERVAL))
    return;
  if (duty_target > duty_curr) {
    duty_curr += duty_change_per_interval;
    if (duty_curr > duty_target)
        duty_curr = duty_target;
  } 
  myservo.writeMicroseconds(duty_curr);
  Serial.print(",duty_curr:");   Serial.print(duty_curr);
  Serial.print(",duty_change_per_interval:");   Serial.print(duty_change_per_interval);
  Serial.println();

  last_sampling_time += INTERVAL;
}
