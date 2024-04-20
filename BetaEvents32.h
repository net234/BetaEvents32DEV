#pragma once
//EventManager32 uses ESP8266 kernel 3.0
static_assert(sizeof(time_t) == 8, "This version works with time_t 32bit, please move to ESP8266 kernel 3.0");

// #define D3  0     //!FLASH    BP0
// #define D4  2     //!LED2     LED0


#include "EventsManager32.h"
// Events Manager build an instance called "Events" who care about events




//if SERIAL is needed it should be declare first othewise others evHandlers cant print during begin
evHandlerSerial Keyboard;  //Only one instance  Must be named KeyBoard


// instance debugger
evHandlerDebug  Debug;


// instance poussoir sud D3 (FLASH) ou BP0_PIN

// definition GPIO sur D3 pour BP0 si celuici n'est pas defini
#ifndef BP0_PIN
#define BP0_PIN D3 // (flash) 
#endif

evHandlerButton BP0(evBP0, BP0_PIN);


// instance LED si evLed0 existe

//definition GPIO sur LED_BUILTIN pour LED0 si il n'est pas defini par l'utilisateur
#ifndef LED0_PIN
#define LED0_PIN D4  // led builtin gpio 2
#endif


// led clignotante a 1Hz   logique inversée sur ESP
evHandlerLed    Led0(evLed0, LED0_PIN, false , 1);
