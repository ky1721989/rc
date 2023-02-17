#include "StateSelectProfile.h"

#include "StateRun.h"

#include "AudioManager.h"

#include "TXMain.h"
#include "TXInput.h"

StateSelectProfile StateSelectProfile::instance;

//======================================================
//======================================================
void StateSelectProfile::onEnter(StateBase *prevState)
{
  this->stateTime = millis();
  this->waitUnpress = true;
  this->runningLight = true;
  this->exit = false;
  this->profileIndex = currentProfileIndex;

  AudioManager::instance.play( "/select_profile.mp3", AUDIO_GROUP_NONE );

  this->initLEDS4RunningLight();
}

//======================================================
//======================================================
void StateSelectProfile::onRun(uint32_t t)
{
  TXInput::instance.loop(t);

  if ( this->waitUnpress )
  {
    if ( 
      TXInput::instance.isButtonUnPressed(LEFT_BUMPER_ID) && 
      TXInput::instance.isButtonUnPressed(LEFT_TRIGGER_ID) && 
      TXInput::instance.isStickMiddle(RIGHT_STICK_X_ID) &&
      TXInput::instance.isStickMiddle(LEFT_STICK_X_ID) 
      )
    {
      this->waitUnpress = false;
      this->runningLight = false;
      this->SetLEDS4Profile(profileIndex);
    }
    if ( this->runningLight )
    {
      this->SetLEDS4RunningLight();
    }
  }
  else
  {
    if ( this->exit ) 
    {
      currentProfileIndex = this->profileIndex;
      TXMain::instance.saveLastProfile();
      StateBase::Goto(&StateRun::instance);
    }
    if ( t % 256 < 128 )
    {
      this->SetLEDS4Profile( this->profileIndex );
    }
    else
    {
      TXMain::instance.setLEDS4( 0 );
    }

    if ( TXInput::instance.isStickMin(LEFT_STICK_X_ID) || TXInput::instance.isStickMin(RIGHT_STICK_X_ID))
    {
      this->waitUnpress = true;
      if ( profileIndex == 0)
      {
        profileIndex = 7;
      }
      else
      {
        profileIndex--;
      }
      this->SetLEDS4Profile(profileIndex);
      AudioManager::instance.sayProfile(this->profileIndex);
      StateBase::lastProfileMessage = millis();
    }

    if ( TXInput::instance.isStickMax(LEFT_STICK_X_ID) || TXInput::instance.isStickMax(RIGHT_STICK_X_ID) )
    {
      this->waitUnpress = true;
      if ( profileIndex == 7)
      {
        profileIndex = 0;
      }
      else
      {
        profileIndex++;
      }
      this->SetLEDS4Profile(profileIndex);
      AudioManager::instance.sayProfile(this->profileIndex);
      StateBase::lastProfileMessage = millis();
    }

    if ( TXInput::instance.isButtonPressed(LEFT_TRIGGER_ID) )
    {
      this->waitUnpress = true;
      this->runningLight = true;
      this->exit = true;
    }
  }
}
