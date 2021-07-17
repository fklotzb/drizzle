#include <Scheduler.h>
#include <Arduino_APDS9960.h>
#include <Arduino_HTS221.h>
#include <ArduinoBLE.h>

#define EHUM_SENSOR A0
#define VALVE A3
#define LED_R 22
#define LED_G 23
#define LED_B 24
#define LED_P 25
#define LED_S 13

void setup() {
  pinMode(EHUM_SENSOR, INPUT);
  
  Serial.begin(9600);
  while (!Serial) {
    digitalWrite(LED_S, HIGH);
    delay(100);
    digitalWrite(LED_S, LOW);
    delay(100);
  }
  while (!APDS.begin()) {
    Serial.println("ERROR: failed initializing APDS9960 sensor");
    digitalWrite(LED_R, HIGH);
    delay(10000);
  }
  while (!HTS.begin()) {
    Serial.println("ERROR: failed initializing HTS221 sensor");
    digitalWrite(LED_R, HIGH);
    delay(10000);
  }
  digitalWrite(LED_R, LOW)
}


void loop() {
  ehum = 0;
  abright = 0;
  ahum = 0.0;
  atemp = 0.0;
  
  for (byte i=0; i<100; i++) {
    int r, g, b, a;
    while (! APDS.colorAvailable()) {
      delay(5);
    }
    ehum = ehum + analogRead(SENSOR);
    APDS.readColor(r, g, b, a);
    abright = abright + a;
    ahum = ahum + HTS.readHumidity();
    atemp = atemp + HTS.readTemperature();
  }

  ehum = ehum / 100;
  abright = abright / 100;
  ahum = ahum / 100;
  atemp = atemp / 100;
  
  Serial.print("EHUM= ");
  Serial.print(ehum);
  Serial.print("  ABRIGHT= ");
  Serial.print(abright);
  Serial.print("  AHUM= ");
  Serial.print(ahum);
  Serial.print("  ATEMP= ");
  Serial.println(atemp);

  delay(100);
}
