#include "mbed.h"
#include "DHT.hpp"

int main() {
    // Assuming DHT is connected to pin D8
    DHT dht(D8);

    int temperature, humidity;
    int result = dht.readTemperatureHumidity(temperature, humidity);

    if (result == 0) {
        printf("Temperature: %d°C\n", temperature);
        printf("Humidity: %d%%\n", humidity);
    } else {
        printf("%s\n", dht.getErrorString(result).c_str());
    }

    return 0;
}
