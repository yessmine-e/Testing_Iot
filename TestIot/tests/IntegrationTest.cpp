#include <gtest/gtest.h>
#include "MockSensor.h"
#include "MockMQTTClient.h"
#include "MockStorage.h"
#include "../include/DataCollector.h"
#include "../include/DataProcessor.h"
#include "../include/MQTTPublisher.h"

using ::testing::_;
using ::testing::Return;

TEST(IntegrationTest, CollectProcessAndPublish) {
    // Mocks
    MockSensor tempSensor;
    MockSensor humSensor;
    MockMQTTClient client;
    MockStorage storage;

    // Composants
    DataCollector collector;
    collector.addSensor(&tempSensor);
    collector.addSensor(&humSensor);

    DataProcessor processor(&storage, 0.0f, 40.0f);
    MQTTPublisher publisher(&client, &storage, "broker", 1883);

    // Capteurs disponibles
    EXPECT_CALL(tempSensor, isAvailable()).WillOnce(Return(true));
    EXPECT_CALL(humSensor, isAvailable()).WillOnce(Return(true));

    EXPECT_CALL(tempSensor, readValue()).WillOnce(Return(25.0f));
    EXPECT_CALL(humSensor, readValue()).WillOnce(Return(50.0f));

    EXPECT_CALL(tempSensor, getSensorId()).WillOnce(Return("temperature"));
    EXPECT_CALL(humSensor, getSensorId()).WillOnce(Return("humidity"));

    // MQTT
    EXPECT_CALL(client, isConnected()).WillOnce(Return(true));
    EXPECT_CALL(client, publish("env/topic", _))
        .WillOnce(Return(true));
    EXPECT_CALL(storage, save(_, _))
        .Times(0);

    // Pipeline complet
    auto raw = collector.collectAllData();
    auto processed = processor.processData(raw);
    bool ok = publisher.publishData("env/topic", processed.values);

    EXPECT_TRUE(ok);
}
