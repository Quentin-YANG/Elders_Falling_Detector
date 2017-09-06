#ifndef _PULSESENSOR_H_
#define _PULSESENSOR_H_  

#include "Arduino.h"

class PulseSensor {
private:
//    int pulseSensorPin = 0;            // Pulse Sensor purple wire connected to analog pin 0
//    int pulseBlinkPin = 13;            // pin to blink led at each beat
    
    volatile int rate[10];                    // array to hold last ten IBI values
    volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
    volatile unsigned long lastBeatTime = 0;           // used to find IBI
    volatile int P =512;                      // used to find peak in pulse wave, seeded
    volatile int T = 512;                     // used to find trough in pulse wave, seeded
    volatile int thresh = 530;                // used to find instant moment of heart beat, seeded
    volatile int amp = 0;                   // used to hold amplitude of pulse waveform, seeded
    volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
    volatile boolean secondBeat = false;      // used to seed rate array so we startup with reasonable BPM
    
public:
    // Volatile Variables, used in the interrupt service routine!
    volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
    volatile int Signal;                // holds the incoming raw data
    volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
    volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
//    volatile boolean QS = false;        // becomes true when Arduoino finds a beat.
    volatile boolean pulseFlag;         // indecator for pulseSensor LED
    
    // Funcs
    void pulseDetect();
    void interruptSetup();
};

#endif /* _PULSESENSOR_H_ */
