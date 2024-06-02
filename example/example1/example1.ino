#include<pezm.h>

pezm energyMeter(1, 9600, D7, D6); //  (nodeid, baudrate,  rx,  tx)

void setup() {
    Serial.begin(115200);
    energyMeter.begin();
}

void loop() {
    energyMeter.getEnergy();
    
    Serial.print("Voltage: ");
    Serial.println(energyMeter.energyMeter.voltage);
    
    Serial.print("current: ");
    Serial.println(energyMeter.energyMeter.current);
    
    Serial.print("frequency: ");
    Serial.println(energyMeter.energyMeter.frequency);
    
    Serial.print("power: ");
    Serial.println(energyMeter.energyMeter.power);
    
    Serial.print("energy: ");
    Serial.println(energyMeter.energyMeter.energy);
    
    Serial.print("power Factor: ");
    Serial.println(energyMeter.energyMeter.powerFactor);
    
    delay(1000); // Read every second
}