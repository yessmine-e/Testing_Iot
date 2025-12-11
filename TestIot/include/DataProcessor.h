//
// Created by Lenovo on 09/12/2025.
//

#ifndef EXAMTPIOT_DATAPROCESSOR_H
#define EXAMTPIOT_DATAPROCESSOR_H
#include <map>
#include <vector>

#include "IStorage.h"

class DataProcessor {
private:
    IStorage* storage;
    float tempThresholdHigh;
    float tempThresholdLow;

public:
    DataProcessor(IStorage* storage, float lowTemp, float highTemp);

    struct ProcessedData {
        std::map<std::string, float> values;
        std::vector<std::string> alerts;
        std::string timestamp;
    };

    ProcessedData processData(const std::map<std::string, float>& rawData);
    bool validateData(const std::map<std::string, float>& data);
    void saveHistoricalData(const ProcessedData& data);
};
#endif //EXAMTPIOT_DATAPROCESSOR_H