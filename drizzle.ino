#include <Scheduler.h>
#include <Arduino_APDS9960.h>

#define EHUM_SENSOR A0
#define VALVE A2
#define LED_R 22
#define LED_G 23
#define LED_S 13
#define LED_P 25

int ehum;                 // earth humidity [%]
int ehum_min = 50;        // minimum earth humidity [%]
int ehum_delay = 10;      // interval for measuring ehum [s]
bool ehum_dry = true;    // humidity status

bool valve_open = false;  // status of valve
int valve_open_time = 15; // time valve stays open after activation [s]
int valve_delay = 3600;   // time valve stays closed after activation [s]

int abright;              // ambient brightness level
int abright_min = 5;     // brightness for switching to nightmode
bool nightmode = false;   // nightmode activated

unsigned long valve_last; // timestamp of last check [ms]


void setup() {
  // set pin modes
  pinMode(EHUM_SENSOR, INPUT);
  pinMode(VALVE, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_S, OUTPUT);
  pinMode(LED_P, OUTPUT);
  
  digitalWrite(LED_R, HIGH);
  digitalWrite(LED_G, HIGH);

  valve_last = valve_delay * -1000;

  // open serial port
  Serial.begin(9600);

  // initialize brightness sensor
  APDS.begin();

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
    if (ehum_dry) {
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
    // earth humidity
    ehum = ehum + analogRead(EHUM_SENSOR);
    // ambient brightness
    int r, g, b, a;
    while (! APDS.colorAvailable()) {
      delay(5);
    }
    APDS.readColor(r, g, b, a);
    abright = abright + a;
  }
  // build avg
  ehum = ehum / 100;
  abright = abright / 100;
  // build percentage
  ehum = map(ehum, 0, 1023, 0, 100);
  abright = map(abright, 0, 1023, 0, 100);
  // print data
  Serial.print("EHUM ");
  Serial.print(ehum);
  Serial.print("%");
  Serial.print("  ABRIGHT= ");
  Serial.print(abright);
  Serial.println("%");
  // check if to dry, then set dry status
  if (ehum < ehum_min) {
    ehum_dry = true;
  }
  else {
    ehum_dry = false;
  }
  // check if dark, then activate nightmode
  if (abright < abright_min) {
    nightmode = true;
  }
  else {
    nightmode = false;
  }
  // update leds
  led_power();
  led_status();

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

void led_status() {
  if (ehum_dry) {
    digitalWrite(LED_R, LOW);
    if (nightmode) {
      delay(50);
      digitalWrite(LED_R, HIGH);
    }
  }
  else {
    digitalWrite(LED_R, HIGH);
  }
}

void led_power() {
  digitalWrite(LED_P, HIGH);
  if (nightmode) {
    delay(50);
    digitalWrite(LED_P, LOW);
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
