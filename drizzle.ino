#include <Scheduler.h>

#define EHUM_SENSOR A0
#define VALVE A2
#define LED_R 22
#define LED_G 23
#define LED_S 13

int ehum;                 // earth humidity [%]
int ehum_min = 50;        // minimum earth humidity [%]
int ehum_delay = 10;      // interval for measuring ehum [s]

bool valve_open = false;  // status of valve
int valve_open_time = 5;  // time valve stays open after activation [s]
int valve_delay = 3600;   // time valve stays closed after activation [s]

unsigned long valve_last = 0; // timestamp of last check [ms]


void setup() {
  // set pin modes
  pinMode(EHUM_SENSOR, INPUT);
  pinMode(VALVE, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_S, OUTPUT);
  
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);

  // open serial port
  Serial.begin(9600);

  // start loops
  Scheduler.startLoop(measure);
  Scheduler.startLoop(valve);
  Scheduler.startLoop(led_serial);
}

void loop() {
  // check if valve delay has elapsed
  if (millis() - valve_last > valve_delay * 1000) {
    // update last check
    valve_last = millis();
    // check if too dry
    if (ehum < ehum_min) {
      valve_open = true;
    }
  }
  // check if valve delay is close to elapse (<=1s), delay if not
  else if (valve_delay * 1000 - millis() - valve_last > 1000) {
    delay(500);
  }
}

void measure() {
  // reset data
  ehum = 0;
  // collect data
  for (byte i=0; i<100; i++) {
    ehum = ehum + analogRead(EHUM_SENSOR);
  }
  // build avg
  ehum = ehum / 100;
  // build percentage
  ehum = map(ehum, 0, 1023, 0, 100);
  // print data
  Serial.print("EHUM ");
  Serial.print(ehum);
  Serial.println("%");
  // check if to dry, then indicate with red led
  if (ehum < ehum_min) {
    digitalWrite(LED_R, LOW);
  }
  else {
    digitalWrite(LED_R, HIGH);
  }
  delay(ehum_delay*1000);
}

void valve() {
  if (valve_open) {
    // open valve for defined time indicate with green led
    digitalWrite(LED_G, LOW);
    digitalWrite(VALVE, HIGH);
    delay(valve_open_time*1000);
    // close valve, reset valve status, deactivate green led
    digitalWrite(VALVE, LOW);
    digitalWrite(LED_G, HIGH);
    valve_open = false;
  }
}

void led_serial() {
  // indicate serial connection
  if (Serial) {
    digitalWrite(LED_S, HIGH);
    delay(50);
    digitalWrite(LED_S, LOW);
  }
  delay(250);
}
