#include "../include/DataCollector.h"

void DataCollector::addSensor(ISensor* sensor) {
    if (sensor != nullptr) {
        sensors.push_back(sensor);
    }
}

std::map<std::string, float> DataCollector::collectAllData() {
    std::map<std::string, float> result;

    for (auto* sensor : sensors) {
        if (sensor && sensor->isAvailable()) {
            float value = sensor->readValue();
            result[sensor->getSensorId()] = value;
        }
    }

    return result;
}

int DataCollector::getAvailableSensorCount() const {
    int count = 0;
    for (auto* sensor : sensors) {
        if (sensor && sensor->isAvailable()) {
            ++count;
        }
    }
    return count;
}
