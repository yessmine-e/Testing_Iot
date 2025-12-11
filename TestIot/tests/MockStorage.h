//
// Created by Lenovo on 09/12/2025.
//

#ifndef EXAMTPIOT_MOCKSTORAGE_H
#define EXAMTPIOT_MOCKSTORAGE_H

#include <gmock/gmock.h>
#include "../include/IStorage.h"

class MockStorage : public IStorage {
public:
    MOCK_METHOD(bool, save, (const std::string& key, const std::string& data), (override));
    MOCK_METHOD(std::string, load, (const std::string& key), (override));
    MOCK_METHOD(bool, exists, (const std::string& key), (override));
};

#endif //EXAMTPIOT_M
