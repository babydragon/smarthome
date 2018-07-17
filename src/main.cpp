#include <memory>

#include <ESP8266WiFi.h>
#include <MQTTClient.h>

#include "common.h"
#include "config.h"

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "mqttclient.h"
#include "ntpclient.h"
#include "dhtsersor.h"
#include "pmssensor.h"

WiFiClientSecure wifiClient;
std::shared_ptr<MQTTClient> mqttClient(new MQTTClient());

// DHT Sensor
const int DHTPin = D1;

unsigned long SERIAL_BAUD = 115200;
SerialOut sout;
bool hasError = false;
// 每次数据读取间隔
uint32_t interval = 30000;
uint32_t lastTs = 0;

auto &config = Config::instance();
NtpClient ntp("ntp1.aliyun.com");

std::unique_ptr<MqttPubClient> mqttPubClient;
std::unique_ptr<Sensor> sensors[] = {
    std::unique_ptr<Sensor>(new DHTSersor(DHTPin)),
    std::unique_ptr<PmsSensor>(new PmsSensor()),
};

void initWiFi();
void initMqtt();
void readAndSend();

// only runs once on boot
void setup() {
    // turn off led
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    initWiFi();
    initMqtt();
    
    for(auto &&sensor : sensors) {
        sensor->begin();
    }
    
    ntp.begin();
    
    lastTs = millis();
}

// runs over and over again
void loop() {
    uint32_t now = millis();
    if(now - lastTs > interval) {
        lastTs = now;
        if(hasError) {
        showError();
        } else {
            readAndSend();
        }
    }
    
    mqttClient->loop();
}

void initWiFi()
{
    sout << "connecting to " << config.ssid() << "\n";
    
    WiFi.begin(config.ssid(), config.wifiPassword());
  
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
        sout << ".";
    }
    
    sout << "\n";
    sout << "WiFi connected!\n";
    sout << "IP: " << WiFi.localIP() << "\n";
}

void initMqtt()
{
    if(mqttClient->connected()) {
        mqttClient->disconnect();
    }
    mqttClient->begin(config.mqttServer(), config.mqttPort(), wifiClient);
    if(mqttClient->connect(config.id(), config.mqttUser(), config.mqttPass())) {
        hasError = false;
        mqttPubClient = std::unique_ptr<MqttPubClient>(
            new MqttPubClient(mqttClient, config.mqttTopic()));

    } else {
        hasError = true;
    }
}

void readAndSend()
{
    for(auto &&sensor : sensors) {
        auto d = sensor->data();
        
        rapidjson::StringBuffer s;
        rapidjson::Writer<rapidjson::StringBuffer> writer(s);
        
        writer.StartObject();
        writer.Key("message_type");
        writer.String(sensor->type());
        writer.Key("device_id");
        writer.String(config.id());
        writer.Key("timestamp");
        writer.Uint64(ntp.getTime());
        
        writer.Key("data");
        writer.StartObject();
        for(auto &item : d) {
            writer.Key(item.first.c_str());
            writer.String(item.second.c_str());
        }
        writer.EndObject();
        
        writer.EndObject();
        
        *mqttPubClient << s.GetString();
    }
}

