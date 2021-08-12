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


const int controlPin[16] = {2,3,4,5,6,7,8,9,10,11,12,13,A0,A1,A2,A3}; // define pins

int t = 0;
int external_tempo = false;

void setup() {
  for(int i=0; i<16; i++)
  {
    pinMode(controlPin[i], OUTPUT);// set pin as output
    digitalWrite(controlPin[i], LOW); 
  }
  
  Serial.begin(9600);// initialize serial monitor with 9600 baud
  Serial.print("hello\n");
}

int maxBits = 16;
int incoming[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int active[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int incoming_n = 0;
int power = 2;
double tempo = (130.0/60.0)/1000.0;
unsigned long first_tick_time = millis();
int tick = 0;
int updated_tick = 0;
int hold_ticks = 4;

void update_pattern() {
  for (int i=0; i<maxBits; ++i) {
    active[i] = incoming[i];
    incoming[i] = 0;
  }
  incoming_n = 0;
  // todo - modulo this to sync with measure?
  updated_tick = tick;
}

void pulse_solenoids() {
  static int pulses = 0;
  int pulsed[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  int ticks_active = tick - updated_tick;

  if (ticks_active > (maxBits + hold_ticks)) {
    down();
  }
  else {
    if (ticks_active > 0) {
      for (int i=0; i < ticks_active; ++i) {
        int full_power = (ticks_active - 1) == i;
        if (active[i] && (full_power || ((pulses % power) == (i % power)))) {
          pulsed[i] = 1;
        }
      }
    }
  }
  if (false) {
    char msg[256];
    sprintf(msg, "tick %d - %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",tick, 
            pulsed[0], pulsed[1], pulsed[2], pulsed[3], 
            pulsed[4], pulsed[5], pulsed[6], pulsed[7], 
            pulsed[8], pulsed[9], pulsed[10], pulsed[11], 
            pulsed[12], pulsed[13], pulsed[14], pulsed[15]
           );
    Serial.write(msg);
  }
        
  for (int i=0; i < maxBits; ++i) {
    digitalWrite(controlPin[i], pulsed[i] ? HIGH : LOW);
  }
  
  pulses++;
}

void down() {
  for(int i = 0; i < maxBits; i++)
  {
    digitalWrite(controlPin[i], LOW); 
  }
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
      }
      else if (c=='t' && external_tempo) {
        tick++;
      }
    }
  }

  if (!external_tempo) {
    unsigned long since = millis() - first_tick_time;
    // calculate from start time - avoids compounding floating point errors
    int now_tick = floor(((double)since) * tempo);
    if (now_tick > tick) {
      tick = now_tick;
    }
  }
  pulse_solenoids();
}
