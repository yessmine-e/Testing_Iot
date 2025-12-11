//
// Created by Lenovo on 09/12/2025.
//

#ifndef EXAMTPIOT_MQTTPUBLISHER_H
#define EXAMTPIOT_MQTTPUBLISHER_H
#include <map>

#include "IMQTTClient.h"
#include "IStorage.h"

class MQTTPublisher {
private:
    IMQTTClient* mqttClient;
    IStorage* storage;
    std::string brokerAddress;
    int brokerPort;

public:
    MQTTPublisher(IMQTTClient* client, IStorage* storage,
                  const std::string& broker, int port);

    bool publishData(const std::string& topic,
                     const std::map<std::string, float>& sensorData);

    bool publishWithRetry(const std::string& topic,
                          const std::string& message,
                          int maxRetries = 3);
};
#endif //EXAMTPIOT_MQTTPUBLISHER_H