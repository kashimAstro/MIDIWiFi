#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL; 

int potpin_1 = A0; 
int butpin_0 = 3;
int butpin_1 = 4;
int butpin_2 = 5;
int ledstate = 6;
int msg[1];
int oldPotA;
int oldPotB;
int range = 2;

void setup(void){
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);

  pinMode(butpin_0,INPUT);
  pinMode(butpin_1,INPUT);
  pinMode(butpin_2,INPUT);
  pinMode(ledstate, OUTPUT);
  
  digitalWrite(butpin_0,1);
  digitalWrite(butpin_1,1);
  digitalWrite(butpin_2,1);

  Serial.println("TransmitterRF24");
}

void sendMsg(String _msg) {
  String theMessage = _msg;
  int messageSize = theMessage.length();
  for (int i = 0; i < messageSize; i++) {
    int charToSend[1];
    charToSend[0] = theMessage.charAt(i);
    radio.write(charToSend,1);
  }
  //terminate string
  char * m = ".";
  radio.write(m,1);

  radio.powerDown();
  delay(10);
  radio.powerUp();
}

void loop() {
  int analogA = analogRead(A0);
  if(analogA != oldPotA && analogA > (oldPotA+range) || analogA < (oldPotA-range)){
    Serial.println(analogA);
    oldPotA=analogA;
    String potA = String(map(analogA,0,1023,0,200));
    sendMsg(potA);
  } 
  /**/

  if(digitalRead(butpin_0)==0){
    Serial.println("debug 0");
    String msg = "A";
    sendMsg(msg);
  }
  
  if(digitalRead(butpin_1)==0){
    Serial.println("debug 1");
    String msg = "B";
    sendMsg(msg);
  }

  if(digitalRead(butpin_2)==0){
    Serial.println("debug 2");
    String msg = "C";
    sendMsg(msg);
  }
  /**/
}





