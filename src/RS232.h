#pragma once
#include "Arduino.h"
#include "HardwareSerial.h"

class RS232Config{
public:
  uint32_t baudrate;
  uint32_t config;
  int8_t pinRX;
  int8_t pinTX;
  int8_t pinDTR;
  int8_t pinDSR;
};

class RS232 : public HardwareSerial{
public:
	RS232(int uart_nr);
  RS232(const HardwareSerial& serial);
  void begin(size_t baud, uint32_t config = SERIAL_8N1, int8_t rxPin=-1, int8_t txPin=-1, int8_t dtrPin=-1, int8_t dsrPin = -1, uint32_t timeout = 100);
  void begin(RS232Config conf);
  bool beginTransmission();
  void setTimedOut(uint32_t timeout);
  void setDataTerminalReady(bool state);
  bool isDataSetReady();
  size_t write(uint8_t);
  size_t write(const uint8_t *buffer, size_t size);
  bool send(uint8_t *data,uint16_t length);
  void endTransmission();
  int8_t pinDTR; // Data Terminal Ready
  int8_t pinDSR; // Data Set Ready
  int8_t pinTx; // TX Pin
  int8_t pinRx; // RX Pin
  uint32_t timeoutTime;
private:
  uint32_t timeoutStart;
};