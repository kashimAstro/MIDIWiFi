#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

#define MIDI_COMMAND_NOTE_OFF       0x80
#define MIDI_COMMAND_NOTE_ON        0x90
#define MIDI_CONTROL_CHANGE         0x0E

#define SERIAL_BAUND 0

typedef struct {
  uint8_t command;
  uint8_t channel;
  uint8_t data2;
  uint8_t data3;
} 
t_midiMsg;

uint8_t note[] = { 
  0x50,
  0x51,
  0x52,
  0x53,
  0x54,
  0 
};

int msg[1];

RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

int lastmsg = 1;
int velocity = 100;
String theMessage = "";

void setup(void){
  if(SERIAL_BAUND == 0)
    Serial.begin(9600);
  else
    Serial.begin(31250);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  if(SERIAL_BAUND == 0)
    Serial.println("ReceiverRF24");
} 

void loop(void){
  if (radio.available()){
    bool done = false;
    done = radio.read(msg, 1);
    char theChar = msg[0];
    if (msg[0] != '.'){
      theMessage.concat(theChar);
    }
    else {
      if(SERIAL_BAUND == 0){
        //int commaIndex = theMessage.indexOf('P');
        //int secondCommaIndex = theMessage.indexOf('P', commaIndex+1);
        //theMessage.remove(1);
        if(theMessage=="A"){
          Serial.println("1");
        }
        if(theMessage=="B"){
          Serial.println("2");
        }
        if(theMessage=="C"){
          Serial.println("3");
        }
        if( theMessage != "A" && theMessage != "B" && theMessage != "C" && theMessage != "" && theMessage != "0"){
          Serial.println("pot");
        }
      } 
      else {
        if( theMessage != "A" && theMessage != "B" && theMessage != "C" && theMessage != "" && theMessage != "0"){
          MIDImessage(0, MIDI_CONTROL_CHANGE, map(theMessage.toInt(),0,200,0,127));
        }
        if(theMessage=="A"){
          MIDImessage(MIDI_COMMAND_NOTE_ON,  note[0], velocity);
        }
        if(theMessage=="B"){
          MIDImessage(MIDI_COMMAND_NOTE_ON,  note[1], velocity);
        }
        if(theMessage=="C"){
          MIDImessage(MIDI_COMMAND_NOTE_ON,  note[2], velocity);
        }
      }
      theMessage= "";
    }
  }
  delay(5);
}

void MIDImessage(int command, int MIDInote, int MIDIvelocity) {
  t_midiMsg xmsg;
  xmsg.command = command;
  xmsg.channel = 1;
  xmsg.data2   = MIDInote;
  xmsg.data3   = MIDIvelocity;
  Serial.write((uint8_t *)&xmsg, sizeof(xmsg));
}



