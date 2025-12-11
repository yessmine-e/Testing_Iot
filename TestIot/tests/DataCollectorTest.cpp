#include <gtest/gtest.h>
#include "MockSensor.h"
#include "../include/DataCollector.h"

using ::testing::Return;

// 1) Ajout de capteurs
TEST(DataCollectorTest, AddSensorIncreasesAvailableCountWhenSensorIsAvailable) {
    DataCollector collector;
    MockSensor sensor;

    collector.addSensor(&sensor);

    EXPECT_CALL(sensor, isAvailable())
        .WillOnce(Return(true));

    EXPECT_EQ(collector.getAvailableSensorCount(), 1);
}

// 2) Collecte avec tous les capteurs disponibles
TEST(DataCollectorTest, CollectAllDataWithAllSensorsAvailable) {
    DataCollector collector;
    MockSensor sensor1;
    MockSensor sensor2;

    collector.addSensor(&sensor1);
    collector.addSensor(&sensor2);

    EXPECT_CALL(sensor1, isAvailable())
        .WillOnce(Return(true));
    EXPECT_CALL(sensor2, isAvailable())
        .WillOnce(Return(true));

    EXPECT_CALL(sensor1, readValue())
        .WillOnce(Return(25.0f));
    EXPECT_CALL(sensor2, readValue())
        .WillOnce(Return(60.0f));

    EXPECT_CALL(sensor1, getSensorId())
        .WillOnce(Return("temperature"));
    EXPECT_CALL(sensor2, getSensorId())
        .WillOnce(Return("humidity"));

    auto data = collector.collectAllData();

    ASSERT_EQ(data.size(), 2);
    EXPECT_FLOAT_EQ(data["temperature"], 25.0f);
    EXPECT_FLOAT_EQ(data["humidity"], 60.0f);
}

// 3) Gestion des capteurs non disponibles
TEST(DataCollectorTest, SkipUnavailableSensorsInCollection) {
    DataCollector collector;
    MockSensor available;
    MockSensor unavailable;

    collector.addSensor(&available);
    collector.addSensor(&unavailable);

    EXPECT_CALL(available, isAvailable())
        .WillOnce(Return(true));
    EXPECT_CALL(unavailable, isAvailable())
        .WillOnce(Return(false));

    EXPECT_CALL(available, readValue())
        .WillOnce(Return(30.0f));
    EXPECT_CALL(available, getSensorId())
        .WillOnce(Return("temperature"));

    auto data = collector.collectAllData();

    ASSERT_EQ(data.size(), 1);
    EXPECT_TRUE(data.contains("temperature"));
}

// 4) Comptage des capteurs disponibles
TEST(DataCollectorTest, CountOnlyAvailableSensors) {
    DataCollector collector;
    MockSensor s1;
    MockSensor s2;
    MockSensor s3;

    collector.addSensor(&s1);
    collector.addSensor(&s2);
    collector.addSensor(&s3);

    EXPECT_CALL(s1, isAvailable()).WillOnce(Return(true));
    EXPECT_CALL(s2, isAvailable()).WillOnce(Return(false));
    EXPECT_CALL(s3, isAvailable()).WillOnce(Return(true));

    EXPECT_EQ(collector.getAvailableSensorCount(), 2);
}

// 5) Cas limite : aucun capteur
TEST(DataCollectorTest, NoSensorsReturnsEmptyDataAndZeroCount) {
    DataCollector collector;

    auto data = collector.collectAllData();

    EXPECT_TRUE(data.empty());
    EXPECT_EQ(collector.getAvailableSensorCount(), 0);
}
