//
// Created by Lenovo on 09/12/2025.
//

#ifndef EXAMTPIOT_ISTORAGE_H
#define EXAMTPIOT_ISTORAGE_H
#include <string>

// IStorage.h - Interface pour le stockage local
class IStorage {
public:
    virtual ~IStorage() = default;
    virtual bool save(const std::string& key, const std::string& data) = 0;
    virtual std::string load(const std::string& key) = 0;
    virtual bool exists(const std::string& key) = 0;
};
#endif //EXAMTPIOT_ISTORAGE_H