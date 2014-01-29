/* Read Quadrature Encoder
  * Connect Encoder to Pins encoder0PinA, encoder0PinB, and +5V.
  *
  * Sketch by max wolf / www.meso.net
  * v. 0.1 - very basic functions - mw 20061220
  *
  */  


#include <NewRemoteTransmitter.h>

NewRemoteTransmitter transmitter(2536422, 11, 260, 3);

int val; 
int encoder0PinA = 3;
int encoder0PinB = 4;
int encoder0Pos = 0;
int encoder0PinALast = LOW;
int n = LOW;

void setup() { 
  pinMode (encoder0PinA,INPUT);
  pinMode (encoder0PinB,INPUT);
  Serial.begin (9600);
} 

void loop() { 
  n = digitalRead(encoder0PinA);
  if ((encoder0PinALast == LOW) && (n == HIGH)) {
    if (digitalRead(encoder0PinB) == LOW) {
      if (encoder0Pos > 1) { 
        encoder0Pos--;
      }
    } else {
      if (encoder0Pos < 15) {
        encoder0Pos++;
      }
    }
    Serial.println (encoder0Pos);
//    transmitter.sendDim(0, encoder0Pos);
    Serial.println("Sent");
  } 
  encoder0PinALast = n;
} 


