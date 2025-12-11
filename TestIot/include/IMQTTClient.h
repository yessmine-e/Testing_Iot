//
// Created by Lenovo on 09/12/2025.
//

#ifndef EXAMTPIOT_IMQTTCLIENT_H
#define EXAMTPIOT_IMQTTCLIENT_H
#include <string>

// IMQTTClient.h - Interface pour le client MQTT
class IMQTTClient {
public:
    virtual ~IMQTTClient() = default;
    virtual bool connect(const std::string& broker, int port) = 0;
    virtual bool publish(const std::string& topic, const std::string& message) = 0;
    virtual bool disconnect() = 0;
    virtual bool isConnected() const = 0;
};
#endif //EXAMTPIOT_IMQTTCLIENT_H