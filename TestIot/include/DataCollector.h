//
// Created by Lenovo on 09/12/2025.
//

#ifndef EXAMTPIOT_DATACOLLECTOR_H
#define EXAMTPIOT_DATACOLLECTOR_H
#include <string>
#include <vector>
#include <map>

#include "../include/ISensor.h"

class DataCollector {
private:
    std::vector<ISensor*> sensors;

public:
    void addSensor(ISensor* sensor);
    std::map<std::string, float> collectAllData();
    int getAvailableSensorCount() const;
};
#endif //EXAMTPIOT_DATACOLLECTOR_H