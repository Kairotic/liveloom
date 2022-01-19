/*
 * Arduino code to control 16 channel relay with Arduino UNO
 * 
 * Written by Ahmad Shamshiri for Robojax.com on Sunday Oct 08, 2018 
 * at 10:35 in Ajax, Ontario, Canada
 * Watch video instruction for this code: https://youtu.be/Q9aBI4ELKC4
 * 
 * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.* 
 * This code has been download from Robojax.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

// weave 1
//const int=-\\\\\\\ controlPin[16] = {6,10,A1,11,5,9,A2,12,4,8,A3,A0,3,7,2,13}; // define pins
// weave 2
//const int controlPin[16] = {8,A4,9,A3,6,A1,7,A2,4,12,5,A0,2,10,3,11}; // define pins
//const int controlPin[16] = {11,3,12,4,A0,5,13,6,A1,7,A2,8,A3,9,2,10};
// weave 3
const int controlPin[16] = {2,3,4,5,6,7,8,9,10,11,13,A0,A1,A2,A3,A4}; // define pins

int external_tempo = false;
int debug = false;


void setup() {
  for(int i=0; i<16; i++)
  {
    pinMode(controlPin[i], OUTPUT);// set pin as output
    digitalWrite(controlPin[i], LOW); 
  }
  
  Serial.begin(115200);// initialize serial monitor with 115200 baud
  Serial.print("hello\n");
}

int maxBits = 16;
int incoming[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int active[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int incoming_n = 0;
int weakness = 2;
double tempo = (110.0/60.0*2)/1000.0;
unsigned long first_tick_time = millis();
int tick = 0;
int updated_tick = 0;
int hold_ticks = 16;
int t = 0;

void update_pattern() {
  for (int i=0; i<maxBits; ++i) {
    active[i] = incoming[i];
    incoming[i] = 0;
  }
  incoming_n = 0;
  // todo - modulo this to sync with measure?
  updated_tick = tick;
}

int update_tick() {
  if (!external_tempo) {
    unsigned long since = millis() - first_tick_time;
    // calculate from start time - avoids compounding floating point errors
    int now_tick = floor(((double)since) * tempo);
    if (now_tick > tick) {
      tick = now_tick;
    }
  }
  return(tick);
}

void pulse_solenoids() {
  static int pulses = 0;
  int pulsed[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  
  int last_tick = update_tick();
  while (update_tick() == last_tick) {
    delay(1); // is this needed?
  }
  //Serial.write("aha\n");
  last_tick = update_tick();

  for (int i=0; i < (maxBits + 12); ++i) {
    long int d = 0;
    int pulse = 0;
    while (update_tick() == last_tick) {
      for (int j=0; j < maxBits; ++j) {
        int on = ((j == i) || ((j < i) && ((pulse % weakness) == (j % weakness)))) && active[j];
        digitalWrite(controlPin[j], on ? HIGH : LOW);
      } 
      pulse++;
    }
    last_tick = update_tick();
  }
  
  for (int j=0; j < maxBits; ++j) {
    digitalWrite(controlPin[j], LOW);
  } 
  

  /*
  if (debug) {
    char msg[256];
    sprintf(msg, "tick %d - %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",tick, 
            pulsed[0], pulsed[1], pulsed[2], pulsed[3], 
            pulsed[4], pulsed[5], pulsed[6], pulsed[7], 
            pulsed[8], pulsed[9], pulsed[10], pulsed[11], 
            pulsed[12], pulsed[13], pulsed[14], pulsed[15]
           );
    Serial.write(msg);
  }
  */

}

void loop() {
  int c;

  while(Serial.available()) {
    c = Serial.read();
    if ((c == '0' || c == '1') && incoming_n < maxBits) {
      incoming[incoming_n] = c == '1';
      incoming_n++;
    }
    else {
      if (c=='x') {
        update_pattern();
        pulse_solenoids();

      }
      else if (c=='t' && external_tempo) {
        tick++;
      }
    }
  }
}
