/*************************************************

  ***********************************************
    Sketch betaEvents32Code.ino   validation of lib betaEvents to deal nicely with events programing with Arduino
    Copyright 2020 Pierre HENRY net23@frdev.com All - right reserved.

  This file is part of betaEvents.

    betaEvents is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    betaEvents is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with betaEvents.  If not, see <https://www.gnu.org/licenses/lglp.txt>.

  betaEvents32Code is a an exemple of stand alone code to understand Betaevents concept
  this version is specifty crafted for 32 bit processor as ESP8266

  it use no specific lib and no specific Hardware except ESP8266

  BetaEvents Object :

  event         structure minimaliste pour gerer un evenement
  evManager     class du noyau evenementiel permetant de gerer les evenements  une seule instance nomée "Events"
  evHandler     class permetant de gerer un interface specifique (pousoir led relai ...) de maniere evenementiel


  Internal Objets
  evHandlerList class permetant de gerer une liste d'evenement


  fichier :
  BetaEvent.h           modele d'utilisation basic des events (a utiliser pour debuter)
  EventsManager32.h/cpp   code du noyau
  evHandler.h/cpp       code de divers handler de base (input output poussoir led relais clavier debug)


. Variables and constants in RAM (global, static), used 28396 / 80192 bytes (35%)
║   SEGMENT  BYTES    DESCRIPTION
╠══ DATA     1504     initialized variables
╠══ RODATA   988      constants       
╚══ BSS      25904    zeroed variables
. Instruction RAM (IRAM_ATTR, ICACHE_RAM_ATTR), used 60331 / 65536 bytes (92%)
║   SEGMENT  BYTES    DESCRIPTION
╠══ ICACHE   32768    reserved space for flash instruction cache
╚══ IRAM     27563    code in IRAM    
. Code in flash (default, ICACHE_FLASH_ATTR), used 243872 / 1048576 bytes (23%)
║   SEGMENT  BYTES    DESCRIPTION
╚══ IROM     243872   code in flash   

. Variables and constants in RAM (global, static), used 28440 / 80192 bytes (35%)
║   SEGMENT  BYTES    DESCRIPTION
╠══ DATA     1504     initialized variables
╠══ RODATA   1008     constants       
╚══ BSS      25928    zeroed variables
. Instruction RAM (IRAM_ATTR, ICACHE_RAM_ATTR), used 60331 / 65536 bytes (92%)
║   SEGMENT  BYTES    DESCRIPTION
╠══ ICACHE   32768    reserved space for flash instruction cache
╚══ IRAM     27563    code in IRAM    
. Code in flash (default, ICACHE_FLASH_ATTR), used 244760 / 1048576 bytes (23%)
║   SEGMENT  BYTES    DESCRIPTION
╚══ IROM     244760   code in flash   

*/



// Name of this application
#define APP_NAME "BetaEvent32Code V1.0"


// Eventslist
/* Evenements du Manager (voir evHandlers.h)
  evNill = 0,      // No event  about 1 every milisecond but do not use them for delay Use delayedPushMillis(delay,event)
  ev100Hz,         // tick 100HZ    non cumulative (see betaEvent.h)
  ev10Hz,          // tick 10HZ     non cumulative (see betaEvent.h)
  ev1Hz,           // un tick 1HZ   cumulative (see betaEvent.h)
  ev24H,           // 24H when timestamp pass over 24H
  evInit,
  evInChar,
  evInString,
*/


// List of Event specific for this APP
//namespace EventCode {

// Utilisation d'enum class pour une meilleure encapsulation et un typage fort
enum tevUser : uint8_t {
  evBP0 = 100,
  evLed0,
  evD1,
  evR1,

};
//}



#include "BetaEvents32.h"  // this will create an evManager called Events with a push buton BP0 and a Led LED0


void setup() {
  //Serial.begin(115200);
  //Serial.println(F("\r\n\n" APP_NAME));
  // delay(5000);
  Serial.println(F("\r\n\n PRE INIT"));
  Events.begin();
  Serial.println(F(" PRE INIT END"));
  Serial.println(F("\r\n\n" APP_NAME));

  T_println("Bonjour...");
}

bool sleep = true;

void loop() {
  Events.get(sleep);  //get the next event
  //if (Events.code >= 100) { DV_println(Events.code); };
  Events.handle();        //pass the event to every evHandlers
  switch (Events.code) {  //deal whith th event
    case evInit:          //evInit append once affter every evHandles haves done with 'begin'
      {
        Serial.println("Init");  //just infor the user that an evInit append
      }
      break;

    case evD1:
      T_println("D1 done");
      break;

    case evR1:
      Serial.print('R');
      break;




    case ev1Hz:
      {
        //DV_print(second());
        //DV_println(helperFreeRam());
      }
      break;

      /*
    case evLed0:  //
      //DV_println(Events.ext);
      switch (Events.ext) {
        case evxOn:
          Serial.println("O");
          break;
        case evxOff:
          Serial.println(".");
          break;
      }
      break;
*/

    case evBP0:  //
      //DV_println(Events.ext);
      switch (Events.ext) {
        case evxOn:
          Serial.println("BP0 ON");
          Led0.setMillisec(500);
          break;
        case evxLongOn:
          Serial.println("BP0 LONG ON");
          break;
        case evxOff:
          Serial.println("BP0 OFF");
          Led0.setMillisec(5000);
          break;
        case evxLongOff:
          Serial.println("BP0 LONG OFF");
          break;
      }
      break;

    case evInChar:
      //DV_println(Events.cParam);
      switch (Events.cParam) {
        case '1': delay(10); break;
        case '2': delay(100); break;
        case '3': delay(1000); break;
        case '4': delay(10000); break;
      }
      break;



    case evInString:
      if (Events.strPtrParam->equals("S")) {
        sleep = not sleep;
        V_println(sleep);
      }
      if (Events.strPtrParam->equals("D1")) {
        Events.delayedPushMillis(500, evD1);
        T_println("D1 start 500ms");
      }
      if (Events.strPtrParam->equals("D2")) {
        Events.delayedPushMillis(2000, evD1);
        T_println("D1 start 2s");
      }
      if (Events.strPtrParam->equals("D10")) {
        Events.delayedPushSeconds(10, evD1);
        T_println("D1 start 10s");
      }
      if (Events.strPtrParam->equals("R1")) {
        Events.repeatedPushMillis(500, evR1);
        T_println("R1 start every 500ms");
      }
      if (Events.strPtrParam->equals("R2")) {
        Events.repeatedPushSeconds(2, evR1);
        T_println("R1 start every 2 second");
      }
      if (Events.strPtrParam->equals("LEDON")) {
        Led0.setOn(true);
        T_println("LED ON");
      }

      if (Events.strPtrParam->equals("LEDOFF")) {
        Led0.setOn(false);
        T_println("LED OFF");
      }
      break;
  }
}
