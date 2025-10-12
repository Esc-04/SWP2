
#include <Servo.h>
#define PIN_SERVO 10
Servo ms;
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO
// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 50     // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance

void setup() {
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW); 
  ms.attach(PIN_SERVO);
  ms.write(0);
  Serial.begin(57600);
}

float easeInOutSine(float x){
  return -0.5 * (cos(PI * x) - 1);
}
void singraph(int start_angle, int end_angle) {
  int steps = 40;
  for (int i = 0; i <= steps; i++) {
    float x = i / (float)steps;       
    float s = easeInOutSine(x);
    int angle = start_angle + s * (end_angle - start_angle);
    ms.write(angle);
    delay(30); 
  }
}
bool open1=false;
void loop() {
  float distance = USS_measure(PIN_TRIG, PIN_ECHO); // read distance

  if ((distance <70) && !open1) {
      singraph(0,90);
      open1=true;
      delay(1000);
  } else if ((distance>150) && open1){ 
      singraph(90,0);    
      open1=false;    
  }

 
  Serial.print("distance:");  Serial.print(distance);
  Serial.println("");
 
  
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm

}
