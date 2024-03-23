#include "DHT.hpp"

DHT::DHT(PinName pin) : _pin(pin), _delayMS(500) {
    _pin.output();
    _pin = 1; // Set pin high
}

void DHT::setDelay(unsigned long delay) {
    _delayMS = delay;
}

int DHT::readTemperature() {
    uint8_t data[5];
    int error = readRawData(data);
    if (error != 0) {
        return error;
    }
    return data[2];
}

int DHT::readHumidity() {
    uint8_t data[5];
    int error = readRawData(data);
    if (error != 0) {
        return error;
    }
    return data[0];
}

int DHT::readTemperatureHumidity(int &temperature, int &humidity) {
    uint8_t data[5];
    int error = readRawData(data);
    if (error != 0) {
        return error;
    }
    humidity = data[0];
    temperature = data[2];
    return 0; // Success
}

String DHT::getErrorString(int errorCode) {
    switch (errorCode) {
        case ERROR_TIMEOUT:
            return "Error 253 Reading from DHT timed out.";
        case ERROR_CHECKSUM:
            return "Error 254 Checksum mismatch while reading from DHT.";
        default:
            return "Error Unknown.";
    }
}

void DHT::startSignal() {
    _pin.output();
    _pin = 0; // Set pin low
    wait_ms(18);
    _pin = 1; // Set pin high
    wait_us(40);
    _pin.input();
}

int DHT::readRawData(uint8_t data[5]) {
    wait_ms(_delayMS);
    startSignal();
    Timer timer;
    timer.start();
    while (_pin.read() == 1) {
        if (timer.read_ms() > TIMEOUT_DURATION) {
            return ERROR_TIMEOUT;
        }
    }

    if (_pin.read() == 0) {
        wait_us(80);
        if (_pin.read() == 1) {
            wait_us(80);
            for (int i = 0; i < 5; i++) {
                data[i] = readByte();
                if (data[i] == ERROR_TIMEOUT) {
                    return ERROR_TIMEOUT;
                }
            }

            if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
                return 0; // Success
            } else {
                return ERROR_CHECKSUM;
            }
        }
    }
    return ERROR_TIMEOUT;
}

uint8_t DHT::readByte() {
    uint8_t value = 0;

    for (int i = 0; i < 8; i++) {
        while (_pin.read() == 0);
        wait_us(30);
        if (_pin.read() == 1) {
            value |= (1 << (7 - i));
        }
        while (_pin.read() == 1);
    }
    return value;
}
