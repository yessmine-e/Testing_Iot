//
// Created by Lenovo on 09/12/2025.
//

#ifndef EXAMTPIOT_ISENSOR_H
#define EXAMTPIOT_ISENSOR_H
#include <string>

// ISensor.h - Interface pour les capteurs
class ISensor {
public:
    virtual ~ISensor() = default;
    virtual float readValue() = 0;
    virtual bool isAvailable() = 0;
    virtual std::string getSensorId() const = 0;
};
#endif //EXAMTPIOT_ISENSOR_H