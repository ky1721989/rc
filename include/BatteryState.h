#pragma once

#include <Arduino.h>

#include <stdio.h>
#include <stdarg.h>

#include "Debounce.h"
#include "tx_config.h"
#include "TXInput.h"
#include "AudioManager.h"

extern HC06Interface externalBTSerial;

#define BATTERY_CHECK_PERIOD_FIRST_MS 10000
#define BATTERY_CHECK_PERIOD_MS 10000
#define BATTERY_REPEAT_PERIOD_MS 60000

//=====================================================================
//=====================================================================
class BatteryState: public Debounce
{
protected:    

    bool readState( bool lastState ) override
    {
      //Serial.println(TXInput::instance.ADC[BAT_ADC]);
      if ( lastState )
      {
        return TXInput::instance.ADC[BAT_ADC] < BAT_LOW_HIGH;
      }
      else
      {
        return TXInput::instance.ADC[BAT_ADC] < BAT_LOW_LOW;
      }

     return true;
    }

    void onDebouncedChange( bool state ) override
    {
      if ( state )
      {
        AudioManager::instance.play( "/low_battery.mp3", AUDIO_GROUP_NONE );
      }
    }

    void onRepeat( bool state ) override
    {
      if ( state )
      {
        AudioManager::instance.play( "/low_battery.mp3", AUDIO_GROUP_NONE );
      }
      /*
      char msg[100];
      sprintf( msg, "%d\n\r", TXInput::instance.ADC[BAT_ADC] );
      char *p = msg;
      while ( *p )
      {
        externalBTSerial.write( *p++ );
      }
      */
    }

  public:

    BatteryState() : Debounce( false, BATTERY_CHECK_PERIOD_FIRST_MS, BATTERY_CHECK_PERIOD_MS, BATTERY_REPEAT_PERIOD_MS )
    {
    }

};

