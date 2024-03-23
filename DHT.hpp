#ifndef DHT_HPP
#define DHT_HPP

#include "mbed.h"

class DHT {
public:
    DHT(PinName pin);
    void setDelay(unsigned long delay);
    int readTemperature();
    int readHumidity();
    int readTemperatureHumidity(int &temperature, int &humidity);
    String getErrorString(int errorCode);

private:
    void startSignal();
    int readRawData(uint8_t data[5]);
    uint8_t readByte();

    static const int ERROR_TIMEOUT = 253;
    static const int ERROR_CHECKSUM = 254;
    static const int TIMEOUT_DURATION = 1000;

    DigitalInOut _pin;
    unsigned long _delayMS;
};

#endif /* DHT_HPP */
