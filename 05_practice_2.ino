#define PIN_LED 7
void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_LED,OUTPUT);
  while(!Serial){
    ;}
}

void loop() {
  // put your main code here, to run repeatedly:
  pinMode(PIN_LED,1);
  delay(1000);
  for (int i=0;i<5;i++){
    pinMode(PIN_LED,0);
    delay(100);
    pinMode(PIN_LED,1);
    delay(100);
  }
  while(1){
    pinMode(PIN_LED,0);
  }
}
