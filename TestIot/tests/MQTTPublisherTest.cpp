#include <gtest/gtest.h>
#include "MockMQTTClient.h"
#include "MockStorage.h"
#include "../include/MQTTPublisher.h"

using ::testing::Return;
using ::testing::_;

// 1) Publication réussie au premier essai
TEST(MQTTPublisherTest, PublishSuccessFirstTry) {
    MockMQTTClient client;
    MockStorage storage;
    MQTTPublisher publisher(&client, &storage, "broker", 1883);

    EXPECT_CALL(client, isConnected())
        .WillOnce(Return(true));
    EXPECT_CALL(client, publish("topic", _))
        .WillOnce(Return(true));
    EXPECT_CALL(storage, save(_, _))
        .Times(0);

    std::map<std::string, float> data{{"temperature", 25.0f}};
    bool result = publisher.publishData("topic", data);

    EXPECT_TRUE(result);
}

// 2) Échec de publication → sauvegarde locale
TEST(MQTTPublisherTest, PublishFailureSavesLocally) {
    MockMQTTClient client;
    MockStorage storage;
    MQTTPublisher publisher(&client, &storage, "broker", 1883);

    EXPECT_CALL(client, isConnected())
        .WillOnce(Return(true));
    EXPECT_CALL(client, publish("topic", _))
        .WillOnce(Return(false));
    EXPECT_CALL(storage, save("failed_topic", _))
        .Times(1);

    std::map<std::string, float> data{{"temperature", 25.0f}};
    bool result = publisher.publishData("topic", data);

    EXPECT_FALSE(result);
}

// 3) Retry réussit au 2e essai
TEST(MQTTPublisherTest, RetrySucceedsOnSecondAttempt) {
    MockMQTTClient client;
    MockStorage storage;
    MQTTPublisher publisher(&client, &storage, "broker", 1883);

    EXPECT_CALL(client, isConnected())
        .WillOnce(Return(true));
    EXPECT_CALL(client, publish("topic", "msg"))
        .WillOnce(Return(false))   // 1ère tentative
        .WillOnce(Return(true));   // 2ème tentative
    EXPECT_CALL(storage, save(_, _))
        .Times(0);

    bool result = publisher.publishWithRetry("topic", "msg", 3);

    EXPECT_TRUE(result);
}

// 4) Retry échoue après max tentatives
TEST(MQTTPublisherTest, RetryFailsAfterMaxAttempts) {
    MockMQTTClient client;
    MockStorage storage;
    MQTTPublisher publisher(&client, &storage, "broker", 1883);

    EXPECT_CALL(client, isConnected())
        .WillOnce(Return(true));
    EXPECT_CALL(client, publish("topic", "msg"))
        .Times(3)
        .WillRepeatedly(Return(false));
    EXPECT_CALL(storage, save("failed_topic", "msg"))
        .Times(1);

    bool result = publisher.publishWithRetry("topic", "msg", 3);

    EXPECT_FALSE(result);
}

// 5) Client MQTT déconnecté avant publication et échec de reconnexion
TEST(MQTTPublisherTest, ClientDisconnectedAndCannotReconnect) {
    MockMQTTClient client;
    MockStorage storage;
    MQTTPublisher publisher(&client, &storage, "broker", 1883);

    EXPECT_CALL(client, isConnected())
        .WillOnce(Return(false));
    EXPECT_CALL(client, connect("broker", 1883))
        .WillOnce(Return(false));
    EXPECT_CALL(storage, save("failed_topic", _))
        .Times(1);

    std::map<std::string, float> data{{"temperature", 25.0f}};
    bool result = publisher.publishData("topic", data);

    EXPECT_FALSE(result);
}

// 6) Cas limite : maxRetries = 0
TEST(MQTTPublisherTest, RetryWithZeroMaxRetriesDoesNotPublish) {
    MockMQTTClient client;
    MockStorage storage;
    MQTTPublisher publisher(&client, &storage, "broker", 1883);

    EXPECT_CALL(client, isConnected())
        .WillOnce(Return(true));
    EXPECT_CALL(client, publish(_, _))
        .Times(0);
    EXPECT_CALL(storage, save("failed_topic", "msg"))
        .Times(1);

    bool result = publisher.publishWithRetry("topic", "msg", 0);

    EXPECT_FALSE(result);
}


