#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL; 

int potpin = A0; 
int butpin = 4;
int ledstate = 6;

int msg[1];
int oldPot;
int range = 2;

void setup(void){
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);

  pinMode(butpin,INPUT);
  pinMode(ledstate, OUTPUT);
  
  digitalWrite(butpin,1);

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
  char * m = ".";/* end string */
  radio.write(m,1);

  radio.powerDown();
  delay(100);
  radio.powerUp();
}

boolean tap = false;

void loop() {
  int analogA = analogRead(potpin);
  
  if(analogA != oldPot && analogA > (oldPot+range) || analogA < (oldPot-range)){
    Serial.println(analogA);
    oldPot=analogA;
    String pot = String(map(analogA,0,1023,0,200));
    sendMsg(pot);
  } 
  
  int button_state = digitalRead(butpin);
  
  if(button_state==0&&tap==false){
    Serial.println("debug 0");
    String msg = "A";
    sendMsg(msg);
    tap=true;
  }else if(button_state==1&&tap==true){
    Serial.println("debug 1");
    String msg = "B";
    sendMsg(msg);
    tap=false;
  } 
}





