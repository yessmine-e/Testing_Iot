#include "../include/DataProcessor.h"
#include <ctime>
#include <cmath>
#include <sstream>

DataProcessor::DataProcessor(IStorage* storage_, float lowTemp, float highTemp)
    : storage(storage_), tempThresholdHigh(highTemp), tempThresholdLow(lowTemp) {}

bool DataProcessor::validateData(const std::map<std::string, float>& data) {
    if (data.empty()) {
        return false;
    }

    for (const auto& pair : data) {
        float value = pair.second;
        if (std::isnan(value)) {
            return false;
        }
        // Exemple de valeurs aberrantes : en dehors de [-100, 100]
        if (value < -100.0f || value > 100.0f) {
            return false;
        }
    }
    return true;
}

DataProcessor::ProcessedData
DataProcessor::processData(const std::map<std::string, float>& rawData) {
    ProcessedData result;
    result.values = rawData;

    // Timestamp simple ISO-like
    std::time_t now = std::time(nullptr);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", std::localtime(&now));
    result.timestamp = buffer;

    // Valider les données
    if (!validateData(rawData)) {
        // Rien à faire si invalide : on renvoie avec alerts vide
        return result;
    }

    // Génération d'alertes sur la température (clé "temperature")
    auto it = rawData.find("temperature");
    if (it != rawData.end()) {
        float temp = it->second;
        if (temp > tempThresholdHigh) {
            result.alerts.push_back("HIGH_TEMPERATURE");
        } else if (temp < tempThresholdLow) {
            result.alerts.push_back("LOW_TEMPERATURE");
        }
    }

    return result;
}

void DataProcessor::saveHistoricalData(const DataProcessor::ProcessedData& data) {
    if (!storage) return;

    // clé horodatée, par ex : "history_2025-12-11T10:15:00"
    std::string key = "history_" + data.timestamp;

    // sérialisation simplifiée : "sensor1=12.3;sensor2=45.6"
    std::ostringstream oss;
    bool first = true;
    for (const auto& pair : data.values) {
        if (!first) {
            oss << ";";
        }
        first = false;
        oss << pair.first << "=" << pair.second;
    }

    storage->save(key, oss.str());
}
//
// Created by Lenovo on 09/12/2025.
//