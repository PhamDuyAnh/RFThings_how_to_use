/*
 * Pham Duy Anh - CKD - 0908984010 - phamduyanh@gmail.com
 * Create: 08-09-2017
 * Update: 05-11-2023
 * Githup: https://github.com/PhamDuyAnh/LEDStatus
  
  # LEDStatus
   This function will flash a LED with cycles
                                                           pinState          (stat flashing led, HIGHT or LOW);
               1       2       3       4 <---------------- numOfFlashes      (the number of times the leds flash in one cycle)
            ->| |<---------------------------------------- timeLED           (time led flash, ms)   >2ms
            ->| | | | |<---------------------------------- timePeriod        (time of a period, ms) >450ms
            ->| |                                     |<-- intervalLEDStatus (time of a cycle, ms)
   Led on      _       _       _       _
   Led off  __| |_____| |_____| |_____| |_____|_______|
            --------------------------------->|       |<-- end of a cycle, LED off minimum a timePeriod or longer
  
 # Example:
    //LEDStatus(byte ledpin, byte numOfFlashes, byte maxFlashes, unsigned int intervalLEDStatus, byte pinState)
    #include "src/LEDStatus.h"
    or
    #include <LEDStatus.h>
  
    LEDStatus ledstatus(LED_PIN, 1, 5, 2500, 1);
    void loop()
    {
      ledstatus.run();
    }
   
    Set numOfFlashes or intervalLEDStatus
      ledstatus.setFlashes(X); X < maxFlashes   --> set numOfFlashes
      ledstatus.setInterval(X)                  --> set intervalLEDStatus
    LED flasher
      ledstatus.ledFlasher()                    --> led flashed one time
      ledstatus.ledFlasher(X)                   --> led flashed X times
 # Attention:
     The cycle will not be accurate if the loop() time is too large
*/

#include "LEDStatus.h"
//  LEDStatus(byte ledpin, byte numOfFlashes, byte maxFlashes, unsigned int intervalLEDStatus, byte pinState)
LEDStatus::LEDStatus(byte ledpin, byte numOfFlashes, byte maxFlashes, unsigned int intervalLEDStatus, byte pinState) {
  _ledpin = ledpin;
  _maxFlashes = maxFlashes;

  if (numOfFlashes <= _maxFlashes) _numOfFlashes = numOfFlashes;
  else _numOfFlashes = _maxFlashes;

  if (intervalLEDStatus > _maxFlashes * _timePeriod + _timePeriod)
    _intervalLEDStatus = intervalLEDStatus;
  else
    _intervalLEDStatus = _maxFlashes * _timePeriod + _timePeriod;

  _pinState = pinState;
  pinMode(_ledpin, OUTPUT);
}
// setNum(unsigned int numOfFlashes)
void LEDStatus::setFlashes(byte numOfFlashes) {
  if (numOfFlashes <= _maxFlashes)
    _numOfFlashes = numOfFlashes;
  else
    _numOfFlashes = _maxFlashes;
}
// setInterval(unsigned int intervalLEDStatus)
void LEDStatus::setInterval(unsigned int intervalLEDStatus) {
  if (intervalLEDStatus > _maxFlashes * _timePeriod + _timePeriod)
    _intervalLEDStatus = intervalLEDStatus;
  else
    _intervalLEDStatus = _maxFlashes * _timePeriod + _timePeriod;
}
void LEDStatus::run() {
  if (millis() - _timeLEDStatus > _intervalLEDStatus) {
    _ledStatus = true;
    _numOfFlashed = 1;
    _timeLEDStatus = millis();
  }

  long _checkTime = millis() - (_timeLEDStatus + (unsigned long)_numOfFlashed * _timePeriod);

  if (_ledStatus && (_checkTime > 0)) {
    _numOfFlashed++;
    if (_numOfFlashed > _numOfFlashes) {
      _numOfFlashed = 0;
      _ledStatus = false;
    }
    digitalWrite(_ledpin, _pinState);
    delay(_timeLED);
    digitalWrite(_ledpin, !_pinState);
  } else {
    digitalWrite(_ledpin, !_pinState);
  }
}
/**
 * [ledFlash description]
 * @access public
 * @param  {[type]} bytes numOfFlashes  [description]
 */
void LEDStatus::ledFlasher(byte numOfFlashes) {
  for (byte i = 0; i < numOfFlashes; i++) {
    digitalWrite(_ledpin, _pinState);
    delay(_timeLED);
    digitalWrite(_ledpin, !_pinState);
    if (numOfFlashes > 1) delay(_timePeriod);
  }
}
void LEDStatus::ledFlasher() {
  ledFlasher(1);
}