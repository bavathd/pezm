#include <pezm.h>




pezm::pezm(uint8_t nodeid, uint16_t baudrate, uint8_t rx, uint8_t tx) 
    : _nodeid(nodeid), _baudRate(baudrate), _rxPin(rx), _txPin(tx), node() {}

void pezm::begin() {
    Serial2.begin(_baudRate, SERIAL_8N1, _rxPin, _txPin);
    node.begin(_nodeid, Serial2);
}

void pezm::getEnergy(){
   uint8_t result = node.readInputRegisters(0x0000, 9);
   if (result == node.ku8MBSuccess) {
        energyMeter.voltage =  node.getResponseBuffer(0x0000) / 10.0;
        energyMeter.current = tempDouble(0x0001,0x0002)/1000.0;
        energyMeter.power = tempDouble(0x0003,0x0004)/10.0;
        energyMeter.energy = tempDouble(0x0005,0x0006);
        energyMeter.frequency = node.getResponseBuffer(0x0007)/10.0;
        energyMeter.powerFactor = node.getResponseBuffer(0x0008)/100.0;
   }
   else 
   {
        energyMeter.voltage = 0xffffffff;
        energyMeter.current = 0xffffffff;
        energyMeter.power = 0xffffffff;
        energyMeter.energy = 0xffffffff;
        energyMeter.frequency = 0xffffffff;
        energyMeter.powerFactor = 0xffffffff;
   }
}

uint32_t pezm::tempDouble(uint16_t lowRegister, uint16_t highRegister){
    uint32_t tempdouble = 0x00000000;

    tempdouble |= node.getResponseBuffer(lowRegister);
    tempdouble |= node.getResponseBuffer(highRegister) << 8;
    return tempdouble;
}

void pezm::resetEnergy(){
  //The command to reset the slave's energy is (total 4 bytes):
  //Slave address + 0x42 + CRC check high byte + CRC check low byte.
  uint16_t u16CRC = 0xFFFF;
  static uint8_t resetCommand = 0x42;
  u16CRC = crc16_update(u16CRC, _nodeid);
  u16CRC = crc16_update(u16CRC, resetCommand);
  Serial.println("Resetting Energy");
  Serial2.write(_nodeid);
  Serial2.write(resetCommand);
  Serial2.write(lowByte(u16CRC));
  Serial2.write(highByte(u16CRC));
  delay(1000);
}

void pezm::changeAddress(uint8_t oldAddress, uint8_t newAddress)
{
    static uint8_t SlaveParameter = 0x06;
  static uint16_t registerAddress = 0x0002; // Register address to be changed
  uint16_t u16CRC = 0xFFFF;
  u16CRC = crc16_update(u16CRC, oldAddress);  
  u16CRC = crc16_update(u16CRC, SlaveParameter);
  u16CRC = crc16_update(u16CRC, highByte(registerAddress));
  u16CRC = crc16_update(u16CRC, lowByte(registerAddress));
  u16CRC = crc16_update(u16CRC, highByte(newAddress));
  u16CRC = crc16_update(u16CRC, lowByte(newAddress));
 
  Serial.println("Changing Slave Address");
 
  Serial2.write(oldAddress);
  Serial2.write(SlaveParameter);
  Serial2.write(highByte(registerAddress));
  Serial2.write(lowByte(registerAddress));
  Serial2.write(highByte(newAddress));
  Serial2.write(lowByte(newAddress));
  Serial2.write(lowByte(u16CRC));
  Serial2.write(highByte(u16CRC));
  delay(1000); 
}