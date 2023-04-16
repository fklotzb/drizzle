// static
#define HUMIDITY_SENSOR A1
#define VALVE 2
#define EHUM_LIMIT 50

// var
int ehum_raw;
short ehum_pct;
short ehum_min;
short ehum_max;

void setup(){
  pinMode(HUMIDITY_SENSOR, INPUT);
  pinMode(VALVE, OUTPUT);

  ehum_raw = 0;
  ehum_pct = 0;
  ehum_min = 1023;
  ehum_max = 0;

  Serial.begin(9600);
}

void loop(){
  // reset data
  ehum_raw= 0;
  // collect data
  for (byte i=0; i<100; i++){
    // earth humidity
    ehum_raw += analogRead(HUMIDITY_SENSOR);
  }
  // calculate average
  ehum_raw = ehum_raw / 1000;
  // set boundaries
  if (ehum_raw > ehum_max){
    ehum_max = ehum_raw;
  }
  if (ehum_raw < ehum_min){
    ehum_min = ehum_raw;
  }
  // map percentage
  ehum_pct = map(ehum_raw, ehum_min, ehum_max, 100, 0);
  // log data
  Serial.print("\n");
  Serial.print(" min:");
  Serial.print(ehum_min);
  Serial.print(" max:");
  Serial.print(ehum_max);
  Serial.print(" raw:");
  Serial.print(ehum_raw);
  Serial.print(" pct:");
  Serial.print(ehum_pct);
  Serial.print(" lvl:");

  for (int i=ehum_pct/2; i>0; i--){
    Serial.print("|");
  }

  if (ehum_pct < EHUM_LIMIT){
    digitalWrite(VALVE, HIGH);
  }
  else {
    digitalWrite(VALVE, LOW);
  }
}