#include <gtest/gtest.h>
#include "MockStorage.h"
#include "../include/DataProcessor.h"

using ::testing::_;
using ::testing::Return;

// 1) Données valides sans alertes
TEST(DataProcessorTest, ValidDataNoAlerts) {
    MockStorage storage;
    DataProcessor processor(&storage, 0.0f, 50.0f);

    std::map<std::string, float> data{
        {"temperature", 25.0f},
        {"humidity", 40.0f}
    };

    auto result = processor.processData(data);

    EXPECT_EQ(result.values.size(), 2);
    EXPECT_TRUE(result.alerts.empty());
    EXPECT_FALSE(result.timestamp.empty());
}

// 2) Alerte température trop haute
TEST(DataProcessorTest, HighTemperatureGeneratesAlert) {
    MockStorage storage;
    DataProcessor processor(&storage, 0.0f, 30.0f);

    std::map<std::string, float> data{
        {"temperature", 35.0f}
    };

    auto result = processor.processData(data);

    ASSERT_EQ(result.alerts.size(), 1);
    EXPECT_EQ(result.alerts[0], "HIGH_TEMPERATURE");
}

// 3) Alerte température trop basse
TEST(DataProcessorTest, LowTemperatureGeneratesAlert) {
    MockStorage storage;
    DataProcessor processor(&storage, 10.0f, 40.0f);

    std::map<std::string, float> data{
        {"temperature", 5.0f}
    };

    auto result = processor.processData(data);

    ASSERT_EQ(result.alerts.size(), 1);
    EXPECT_EQ(result.alerts[0], "LOW_TEMPERATURE");
}

// 4) Rejet de données invalides (NaN)
TEST(DataProcessorTest, RejectsNaNData) {
    MockStorage storage;
    DataProcessor processor(&storage, 0.0f, 50.0f);

    std::map<std::string, float> data{
        {"temperature", std::numeric_limits<float>::quiet_NaN()}
    };

    bool valid = processor.validateData(data);
    EXPECT_FALSE(valid);

    auto result = processor.processData(data);
    EXPECT_TRUE(result.alerts.empty()); // pas d'alertes car données invalides
}

// 5) Sauvegarde correcte des données historiques
TEST(DataProcessorTest, SaveHistoricalDataUsesTimestampKey) {
    MockStorage storage;
    DataProcessor processor(&storage, 0.0f, 50.0f);

    DataProcessor::ProcessedData pdata;
    pdata.values = {{"temperature", 20.0f}, {"humidity", 50.0f}};
    pdata.timestamp = "2025-12-11T10:15:00";

    EXPECT_CALL(storage,
                save("history_2025-12-11T10:15:00", _))
        .Times(1);

    processor.saveHistoricalData(pdata);
}

// 6) Traitement avec plusieurs capteurs
TEST(DataProcessorTest, ProcessMultipleSensors) {
    MockStorage storage;
    DataProcessor processor(&storage, 0.0f, 50.0f);

    std::map<std::string, float> data{
        {"temperature", 22.0f},
        {"humidity", 55.0f},
        {"air_quality", 80.0f}
    };

    auto result = processor.processData(data);

    EXPECT_EQ(result.values.size(), 3);
    EXPECT_TRUE(result.alerts.empty());
}
