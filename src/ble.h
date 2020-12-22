#pragma once 
#include<Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

class BLE_MIDI: BLEServerCallbacks {
  BLEServer *pServer;
  BLEService *pService;
  BLECharacteristic *pCharacteristic;
  BLEAdvertising *pAdvertising;

  bool connected = false;

  void onConnect(BLEServer* pServer);
  void onDisconnect(BLEServer* pServer);
  void generateHeader(uint8_t *header);
  void send(uint8_t *data);

public:
  void init();
  void connect();
  void noteOn(uint8_t ch, uint8_t note, uint8_t vel);
  void noteOff(uint8_t ch, uint8_t note);
  void control(uint8_t ch, uint8_t cc, uint8_t value);
  bool isConnected();
};