cmake_minimum_required(VERSION 4.0)
project(ExamTPIoT)

set(CMAKE_CXX_STANDARD 20)

# Définir les chemins
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)

# Inclure les répertoires d'en-têtes
include_directories(${INCLUDE_DIR})

# ====================================
# Téléchargement et configuration de GoogleTest
# ====================================
include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)

# Pour Windows : empêcher l'override des flags du compilateur parent
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

# Activer les tests
enable_testing()

# ====================================
# Bibliothèque principale du projet
# ====================================
set(PROJECT_SOURCES
        ${SRC_DIR}/DataCollector.cpp
        ${SRC_DIR}/DataProcessor.cpp
        ${SRC_DIR}/MQTTPublisher.cpp
)

set(PROJECT_HEADERS
        ${INCLUDE_DIR}/ISensor.h
        ${INCLUDE_DIR}/IMQTTClient.h
        ${INCLUDE_DIR}/IStorage.h
        ${INCLUDE_DIR}/DataCollector.h
        ${INCLUDE_DIR}/DataProcessor.h
        ${INCLUDE_DIR}/MQTTPublisher.h
)

# Créer une bibliothèque statique avec le code source
add_library(iot_lib STATIC ${PROJECT_SOURCES})
target_include_directories(iot_lib PUBLIC ${INCLUDE_DIR})

# ====================================
# Configuration des tests
# ====================================

# Macro pour ajouter un test facilement
macro(add_iot_test test_name test_source)
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name}
            PRIVATE
            iot_lib
            gtest
            gtest_main
            gmock
            gmock_main
    )

    # Ajouter le test à CTest
    add_test(NAME ${test_name} COMMAND ${test_name})

    # Définir le répertoire de travail pour les tests
    set_tests_properties(${test_name} PROPERTIES
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endmacro()

# ====================================
# Tests unitaires individuels
# ====================================

# Test pour DataCollector
add_iot_test(DataCollectorTest ${TEST_DIR}/DataCollectorTest.cpp)

# Test pour DataProcessor
add_iot_test(DataProcessorTest ${TEST_DIR}/DataProcessorTest.cpp)

# Test pour MQTTPublisher
add_iot_test(MQTTPublisherTest ${TEST_DIR}/MQTTPublisherTest.cpp)

# ====================================
# Tests d'intégration
# ====================================

add_iot_test(IntegrationTest ${TEST_DIR}/IntegrationTest.cpp)

# ====================================
# Exécutable pour tous les tests
# ====================================

set(ALL_TEST_SOURCES
        ${TEST_DIR}/DataCollectorTest.cpp
        ${TEST_DIR}/DataProcessorTest.cpp
        ${TEST_DIR}/MQTTPublisherTest.cpp
        ${TEST_DIR}/IntegrationTest.cpp
)

add_executable(run_all_tests ${ALL_TEST_SOURCES})
target_link_libraries(run_all_tests
        PRIVATE
        iot_lib
        gtest
        gtest_main
        gmock
        gmock_main
)

#add_executable(ExamTPIoT main.cpp
#       include/ISensor.h
#        include/IMQTTClient.h
#        include/IStorage.h
#        include/DataCollector.h
#        include/DataProcessor.h
#        include/MQTTPublisher.h
#        src/DataCollector.cpp
#        src/DataProcessor.cpp
#        src/MQTTPublisher.cpp
#        tests/MockSensor.h
#        tests/MockMQTTClient.h
#        tests/MockStorage.h
#        tests/DataCollectorTest.cpp
#        tests/DataProcessorTest.cpp
#        tests/MQTTPublisherTest.cpp
#        tests/IntegrationTest.h)
