#include <system_configuration.h>
#include <unwind-cxx.h>
#include <utility.h>

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Include falling detection algrothim
#include "FallingKNN.h"

// Include pulse sensor
#include "PulseSensor.h"

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;

// Falling algorithm instance
FallingKNN fallingknn;

//#define LED_PIN 13
//bool blinkState = false;
int pulseSensorPin = 0;            // Pulse Sensor purple wire connected to analog pin 0
int pulseBlinkPin = 13;            // pin to blink led at each beat
volatile int Signal;
PulseSensor pulsedetector;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400, SERIAL_8E1);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
    
    // Configure Arduino LED for pulse sensor
    //pinMode(LED_PIN, OUTPUT);
    pulsedetector.interruptSetup();
    pinMode(pulseBlinkPin,OUTPUT);
}

void loop() {
   
    // read raw accel/gyro measurements from device
//    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Gyro & accel data print
    // these methods (and a few others) are also available
    //accelgyro.getAcceleration(&ax, &ay, &az);
    //accelgyro.getRotation(&gx, &gy, &gz);
    //Serial.print(ax); Serial.print("\t");
    //Serial.print(ay); Serial.print("\t");
    //Serial.println(az);
    //Serial.print(time_spot);

    // Falling detection
//    char label;
//    label = fallingknn.get_label(ax,ay,az);
//    if(label == 'F') {
//      Serial.print(label); Serial.print("\t");
//      Serial.print(ax); Serial.print("\t");
//      Serial.print(ay); Serial.print("\t");
//      Serial.println(az);
//    }
    
    // Pulse sensor indictor
    digitalWrite(pulseBlinkPin, pulsedetector.pulseFlag); 
    Serial.print(pulsedetector.BPM);
    Serial.print(",");
    Serial.print(pulsedetector.IBI);
    Serial.print(",");
    Serial.println(pulsedetector.Signal);
    delay(50);
}


// Interrupt function
ISR(TIMER2_COMPA_vect){                       // triggered when Timer2 counts to 124
  cli();                                      // disable interrupts while we do this
  pulsedetector.Signal = analogRead(pulseSensorPin);              // read the Pulse Sensor   
  pulsedetector.pulseDetect();
  sei();                                      // enable interrupts when youre done!
}// end isr
