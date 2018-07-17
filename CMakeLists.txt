project(smarthome)

cmake_minimum_required(VERSION 2.8)

set(ESP_ROOT "$ENV{HOME}/code/embed/esp8266")
set(ESP_BOARD "nodemcu")
# Compile
set(ARDUINO_FCPU         "80000000L")
set(ESP_FLASH_LD "eagle.flash.4m1m.ld")

# Upload
set(ARDUINO_UPLOAD_SPEED "115200")
set(ARDUINO_PROTOCOL     "esp")
set(ARDUINO_PORT         "/dev/ttyUSB0")
set(ESP_FLASH_MODE "dio")
set(ESP_FLASH_FREQ "40")
set(ESP_FLASH_SIZE "4M")

#deps
include_directories(include)
set(USER_LIB_DIR "$ENV{HOME}/Arduino/libraries")
set(LIBS ESP8266WiFi ESP8266WebServer DHT_sensor_library Adafruit_Unified_Sensor MQTT PMS_Library)

include(${CMAKE_SOURCE_DIR}/esp8266.cmake)

if(NOT DEFINED SSID)
    message(FATAL_ERROR "ssid not set")
elseif(NOT DEFINED PASSWORD)
    message(FATAL_ERROR "password not set")
elseif(NOT DEFINED MQTT_USER)
    message(FATAL_ERROR "mqtt user not set")
elseif(NOT DEFINED MQTT_PASS)
    message(FATAL_ERROR "mqtt password not set")
endif()

configure_file(basic_config.h.in basic_config.h @ONLY)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
