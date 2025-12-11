//
// Created by Lenovo on 09/12/2025.
//

#ifndef EXAMTPIOT_MOCKMQTTCLIENT_H
#define EXAMTPIOT_MOCKMQTTCLIENT_H

#include <gmock/gmock.h>
#include "../include/IMQTTClient.h"

class MockMQTTClient : public IMQTTClient {
public:
    MOCK_METHOD(bool, connect, (const std::string& broker, int port), (override));
    MOCK_METHOD(bool, publish, (const std::string& topic, const std::string& message), (override));
    MOCK_METHOD(bool, disconnect, (), (override));
    MOCK_METHOD(bool, isConnected, (), (const, override));
};

#endif //EXAMTPIOT_MOCKMQTTCLIENT_H
