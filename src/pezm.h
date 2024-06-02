#ifndef __PEZM_H__
#define __PEZM_H__


#include <ModbusMaster.h>
#include <Arduino.h>


struct _energyMeterRead {
    uint32_t energy;
    float voltage;
    float current;
    float power;
    float frequency;
    float powerFactor;
};

class pezm {
    public:
    pezm(uint8_t nodeid, uint16_t baudrate, uint8_t rx, uint8_t tx);
    _energyMeterRead energyMeter;
    void begin();
    void getEnergy();
    void changeAddress(uint8_t oldAddress, uint8_t newAddress);
    void resetEnergy();

    private:
    uint8_t _nodeid;
    uint8_t _rxPin;
    uint8_t _txPin;
    uint16_t _baudRate;
    uint32_t tempDouble(uint16_t lowRegister, uint16_t highRegister);
    ModbusMaster node;
    
};


#endif