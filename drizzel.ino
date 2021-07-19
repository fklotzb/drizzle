#define EHUM_SENSOR A0
#define LED_S 13

void setup() {
  // set pin modes
  pinMode(EHUM_SENSOR, INPUT);
  pinMode(LED_S, OUTPUT);
  
  // open serial port and wait to connect
  Serial.begin(9600);
  while (!Serial) {
    digitalWrite(LED_S, HIGH);
    delay(100);
    digitalWrite(LED_S, LOW);
    delay(100);
  }
  digitalWrite(LED_S, LOW);
}

void loop() {
  // reset data
  ehum = 0;
  // collect data
  for (byte i=0; i<100; i++) {
    ehum = ehum + analogRead(EHUM_SENSOR);
  }
  // build avg
  ehum = ehum / 100;
  // print data
  Serial.print("EHUM= ");
  Serial.println(ehum);
  delay(100);
}
