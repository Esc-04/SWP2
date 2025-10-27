// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25       // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100     // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300     // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // coefficent to convert duration to distance

#define _EMA_ALPHA 0.7  // EMA weight of new sample (range: 0 to 1)
                          // Setting EMA to 1 effectively disables EMA filter.

// global variables
unsigned long last_sampling_time;   // unit: msec
float dist_prev = _DIST_MAX;        // Distance last-measured
float dist_ema;  // EMA distance
const int N=3; //최근 값을 저장할 최대 리스트 길이 상수 (N=3,10,30)
//#include <vector> // 데이터를 저장할 벡터 선언
float dlist[N]={0};//아두이노에선 벡터가 지원 안됨..!! 대신 배열을 선언하자
int count=0; //배열에 들어간 데이터 수
using namespace std;

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_TRIG,OUTPUT);
  pinMode(PIN_ECHO,INPUT);
  digitalWrite(PIN_TRIG, LOW);
  // initialize serial port
  Serial.begin(57600);
}
float getMid(float arr[],int count){ 
  //정렬해서 중위수 값 구하는 함수 작성
  float tmp[count];
  for (int i=0;i<count;i++) tmp[i]=arr[i];
  for (int i=0;i<count-1;i++){
    for (int j=0;j<count-1-i;j++){
      if (arr[j]>arr[j+1]){
        float t=tmp[j];
        tmp[j]=tmp[j+1];
        tmp[j+1]=t; //서로 위치 바꾸기
      }
    }
  }
  if (count % 2 == 0) {
    // 짝수개 -> 가운데 두 개 평균
    return (arr[count/2] + arr[count/2 - 1]) / 2.0;
  } else {
    // 홀수개 -> 가운데 원소
    return arr[count/2];
  }
}
void loop() {
  float dist_raw, dist_filtered,dist_median;
  
  // wait until next sampling time. 
  // millis() returns the number of milliseconds since the program started. 
  // will overflow after 50 days.
  if (millis() < last_sampling_time + INTERVAL)
    return;

  // get a distance reading from the USS
  dist_raw = USS_measure(PIN_TRIG,PIN_ECHO);
  if (count<N){
    dlist[count]=dist_raw;
    count++;
  }
  else{
    for (int i=0;i<N-1;i++){
      dlist[i]=dlist[i+1];
    }
    dlist[N-1]=dist_raw;    
  }
  dist_median=getMid(dlist,count);
  



  dist_ema=_EMA_ALPHA*dist_median+(1-_EMA_ALPHA)*dist_ema;
  
 

  // output the distance to the serial port
  Serial.print("Min:");   Serial.print(_DIST_MIN);
  Serial.print(",raw:"); Serial.print(dist_raw);
  Serial.print(",ema: "); Serial.print(dist_ema);
  Serial.print(",median: ");  Serial.print(dist_median);
  Serial.print(",Max:");  Serial.print(_DIST_MAX);
  Serial.println("");

  // do something here
  if ((dist_raw < _DIST_MIN) || (dist_raw > _DIST_MAX))
    digitalWrite(PIN_LED, 1);       // LED OFF
  else
    digitalWrite(PIN_LED, 0);       // LED ON

  // update last sampling time
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO)
{
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm


}

