#include "../include/MQTTPublisher.h"
#include <sstream>

MQTTPublisher::MQTTPublisher(IMQTTClient* client,
                             IStorage* storage_,
                             const std::string& broker,
                             int port)
    : mqttClient(client),
      storage(storage_),
      brokerAddress(broker),
      brokerPort(port) {}

bool MQTTPublisher::publishData(const std::string& topic,
                                const std::map<std::string, float>& sensorData) {
    if (!mqttClient) return false;

    // Conversion map -> JSON très simple
    std::ostringstream oss;
    oss << "{";
    bool first = true;
    for (const auto& pair : sensorData) {
        if (!first) {
            oss << ",";
        }
        first = false;
        oss << "\"" << pair.first << "\":" << pair.second;
    }
    oss << "}";
    std::string json = oss.str();

    // S'assurer que le client est connecté
    if (!mqttClient->isConnected()) {
        if (!mqttClient->connect(brokerAddress, brokerPort)) {
            // impossible de se connecter
            if (storage) {
                storage->save("failed_" + topic, json);
            }
            return false;
        }
    }

    // Publication
    if (mqttClient->publish(topic, json)) {
        return true;
    }

    // En cas d'échec, sauvegarde locale
    if (storage) {
        storage->save("failed_" + topic, json);
    }
    return false;
}

bool MQTTPublisher::publishWithRetry(const std::string& topic,
                                     const std::string& message,
                                     int maxRetries) {
    if (!mqttClient) return false;

    // S'assurer que le client est connecté
    if (!mqttClient->isConnected()) {
        if (!mqttClient->connect(brokerAddress, brokerPort)) {
            // échec de connexion avant même de pouvoir publier
            if (storage) {
                storage->save("failed_" + topic, message);
            }
            return false;
        }
    }

    // Boucle de retry
    for (int attempt = 1; attempt <= maxRetries; ++attempt) {
        if (mqttClient->publish(topic, message)) {
            return true; // succès à la tentative attempt
        }
    }

    // Toutes les tentatives ont échoué : sauvegarde locale
    if (storage) {
        storage->save("failed_" + topic, message);
    }
    return false;
}

