//
// Created by Lenovo on 09/12/2025.
//

#ifndef EXAMTPIOT_MOCKSENSOR_H
#define EXAMTPIOT_MOCKSENSOR_H
#include <gmock/gmock.h>
// Votre code ici
#include "../include/ISensor.h"
class MockSensor : public ISensor {
public:
    MOCK_METHOD(float, readValue, (), (override));
    MOCK_METHOD(bool, isAvailable, (), (override));
    MOCK_METHOD(std::string, getSensorId, (), (const, override));
};
#endif //EXAMTPIOT_MOCKSENSOR_H