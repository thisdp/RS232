#include "RS232.h"

RS232::RS232(int uart_nr):HardwareSerial(uart_nr){}
RS232::RS232(const HardwareSerial& serial):HardwareSerial(serial){}

void RS232::begin(size_t baud, uint32_t config, int8_t rxPin, int8_t txPin, int8_t dtrPin, int8_t dsrPin, uint32_t timeout){
	pinDTR = dtrPin;
	pinDSR = dsrPin;
#if defined(ESP32)
	((HardwareSerial*) this)->begin(baud,config,rxPin,txPin);
#else
	((HardwareSerial*) this)->setRx(rxPin);
	((HardwareSerial*) this)->setTx(txPin);
	((HardwareSerial*) this)->begin(baud,config);
#endif
	timeoutTime = timeout;
	if(pinDSR != -1){
		pinMode(pinDSR,INPUT_PULLUP);
	}
	if(pinDSR != -1){
		pinMode(pinDTR,OUTPUT);
		digitalWrite(pinDTR,HIGH);
	}
}

void RS232::begin(RS232Config conf){
	begin(conf.baudrate, conf.config, conf.pinRX, conf.pinTX, conf.pinDTR, conf.pinDSR);
}

void RS232::setTimedOut(uint32_t timeout){
	timeoutTime = timeout;
}

void RS232::setDataTerminalReady(bool state){
	if(pinDTR != -1) digitalWrite(pinDTR,!state);
}

bool RS232::isDataSetReady(){
	if(pinDSR == -1) return true;
	return !digitalRead(pinDSR);
}

bool RS232::send(uint8_t *data,uint16_t length){
	if(beginTransmission()){
		write(data,length);
		endTransmission();
		return true;
	}
	return false;
}

size_t RS232::write(uint8_t c){
	HardwareSerial::write(c);
	return 1;
}

size_t RS232::write(const uint8_t *buffer, size_t size){
	HardwareSerial::write(buffer,size);
	return size;
}

bool RS232::beginTransmission(){
	setDataTerminalReady(true);
	timeoutStart = millis();
	while(1){
		if(isDataSetReady()){
			return true;
		}else if(millis()-timeoutStart >= timeoutTime){ //if timed out
			endTransmission();
			return false;
		}
	}
}

void RS232::endTransmission(){
#if defined(CONFIG_IDF_TARGET_ESP32)
	flush(true);
#else
	flush();
#endif
	setDataTerminalReady(false);
}
