#include <Scheduler.h>
#include <Arduino_APDS9960.h>
#include <Arduino_HTS221.h>
#include <ArduinoBLE.h>

#define SENSOR PIN_A0
#define VALVE A3
#define PLUS A5
#define LEDR 22
#define LEDG 23
#define LEDB 24
#define LEDP 25

bool err_status;
bool warn_status;
bool ble_status;
bool valve_status;

bool valve_lock;
byte valve_max_time;
int last_checktime;

int earth_humidity;
int brightness;
float air_humidity;
float air_temperature;

/* bluetooth functions
// Device name
const char* nameOfPeripheral = "drizzle";
const char* uuidOfService = "0000181a-0000-1000-8000-00805f9b34fb";
const char* uuidOfRxChar = "00002A3D-0000-1000-8000-00805f9b34fb";
const char* uuidOfTxChar = "00002A58-0000-1000-8000-00805f9b34fb";
// BLE Service
BLEService microphoneService(uuidOfService);
// Setup the incoming data characteristic (RX).
const int RX_BUFFER_SIZE = 256;
bool RX_BUFFER_FIXED_LENGTH = false;
// RX / TX Characteristics
BLECharacteristic rxChar(uuidOfRxChar, BLEWriteWithoutResponse | BLEWrite, RX_BUFFER_SIZE, RX_BUFFER_FIXED_LENGTH);
BLEByteCharacteristic txChar(uuidOfTxChar, BLERead | BLENotify | BLEBroadcast);


void startBLE() {
  if (!BLE.begin())
  {
    Serial.println("ERROR: starting BLE failed!");
  }
}

void onRxCharValueUpdate(BLEDevice central, BLECharacteristic characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print("Characteristic event, read: ");
  byte tmp[256];
  int dataLength = rxChar.readValue(tmp, 256);

  for(int i = 0; i < dataLength; i++) {
    Serial.print((char)tmp[i]);
  }
  Serial.println();
  Serial.print("Value length = ");
  Serial.println(rxChar.valueLength());
}

void onBLEConnected(BLEDevice central) {
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  ble_status = true;
}

void onBLEDisconnected(BLEDevice central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  ble_status = false;
}
*/

/*
 * PROGRAM
 */
void start() {
  err_status = false;
  warn_status = false;
  ble_status = false;
  valve_status = false;

  byte ehum[24] = {};
  byte bright[24] = {};
  byte ahum[24] = {};
  byte atemp[24] = {};
  
  Serial.begin(9600);

  pinMode(SENSOR, INPUT);
  pinMode(VALVE, OUTPUT);
  pinMode(PLUS, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(LEDP, OUTPUT);

  /* bluetooth starting
  // Start BLE.
  startBLE();
  // Create BLE service and characteristics.
  BLE.setLocalName(nameOfPeripheral);
  BLE.setAdvertisedService(microphoneService);
  microphoneService.addCharacteristic(rxChar);
  microphoneService.addCharacteristic(txChar);
  BLE.addService(microphoneService);
  // Bluetooth LE connection handlers.
  BLE.setEventHandler(BLEConnected, onBLEConnected);
  BLE.setEventHandler(BLEDisconnected, onBLEDisconnected);
  // Event driven reads.
  rxChar.setEventHandler(BLEWritten, onRxCharValueUpdate);
  // Let's tell devices about us.
  BLE.advertise();

  // Print out full UUID and MAC address.
  Serial.println("Peripheral advertising info: ");
  Serial.print("Name: ");
  Serial.println(nameOfPeripheral);
  Serial.print("MAC: ");
  Serial.println(BLE.address());
  Serial.print("Service UUID: ");
  Serial.println(microphoneService.uuid());
  Serial.print("rxCharacteristic UUID: ");
  Serial.println(uuidOfRxChar);
  Serial.print("txCharacteristics UUID: ");
  Serial.println(uuidOfTxChar);
  Serial.println("Bluetooth device active, waiting for connections...");

  if (!APDS.begin()) {
    Serial.println("err_status: failed initializing brightness sensor");
    err_status = true;
  }
  if (!HTS.begin()) {
    Serial.println("err_status: failed initializing humidity temperature sensor!");
    err_status = true;
  }
*/

  /* scheduler starting loop
  const char loops PROGMEM = {
    light_power,
    light_err,
    light_warn,
    light_ble,
    light_valve
  };

  for (byte i=0; i<sizeof(loops); i++) {
    Scheduler.startLoop(loops[i]);
  }
*/

  Scheduler.startLoop(light_power);
  Scheduler.startLoop(light_err);
  Scheduler.startLoop(light_warn);
  Scheduler.startLoop(light_ble);
  Scheduler.startLoop(light_valve);
  //Scheduler.startLoop(bluetooth);

  analogWrite(PLUS, 50);
}


void main() {  
  int ehum_sum = 0;
  int bright_sum = 0;
  int ahum_sum = 0;
  int atemp_sum = 0;


  for (int i=0; i<1000; i++){
      h_sum = h_sum + analogRead(SENSOR);
      delay(1);
  }
  earth_humidity = (h_sum/1000);
  Serial.println(earth_humidity);
  
  if (earth_humidity<900){
      digitalWrite(VALVE, HIGH);
      valve_status = true;
      /*delay(1000);
      digitalWrite(VALVE, LOW);
      valve_status = false;*/
  }
  else{
      digitalWrite(VALVE, LOW);
      valve_status = false;
  }
  delay(200);

/* bluetooth function
void bluetooth() {
  BLEDevice central = BLE.central();
  
  if (central) {
    // Only send data if we are connected to a central device.
    while (central.connected()) {
      ble_status = true; 

      // Send the buffered values to the central device.
      // print samples to the serial monitor or plotter
        txChar.writeValue(earth_humidity);      
      // Clear the read count
      }
    ble_status = false;
  }
  else {
    ble_status = false;
  }
  delay(5000);
}
*/

void open_valve(byte time) {
  if (!valve_lock) {
    if (time > valve_max_time) {
      time = valve_max_time;
    }
    digitalWrite(VALVE, HIGH);
    delay(time * 1000);
    digitalWrite(VALVE, LOW);
  }
}

/*
 * LEDS
 */
void light_power() {
  digitalWrite(LEDP, HIGH);
  delay(25);
  digitalWrite(LEDP, LOW);
  delay(20000);
}

void light_err() {
  if (err_status) {
    valve_lock = true;
    digitalWrite(LEDR, LOW);
    delay(500);
  }
  elif (!err_status && !warn_status) {
    valve_lock = false;
  }
  digitalWrite(LEDR, HIGH);
  delay(500);
}

void light_warn() {
  if (warn_status) {
    valve_lock = true;
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDG, LOW);
    delay(1000);
  }
  elif (!warn_status && !err_status) {
    valve_lock = false;
  }
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, HIGH);
  delay(1000);
}

void light_ble() {
  if (!ble_status) {
    digitalWrite(LEDB, HIGH);
  }
  delay(20000);
  digitalWrite(LEDB, LOW);
  delay(25);
}

void light_valve() {
  if (valve_status) {
    digitalWrite(LEDG, LOW);
  }
  else {
    digitalWrite(LEDG, HIGH);
  }
  delay(200);
}


void setup() {
  start()
}