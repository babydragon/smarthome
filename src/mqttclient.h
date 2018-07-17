/*
 * <one line to give the library's name and an idea of what it does.>
 * Copyright (C) 2018  jin lingjie <jlj.babydragon@gmail.com>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MQTTCLIENT_H
#define MQTTCLIENT_H

#include <MQTTClient.h>
#include <memory>

/**
 * 封装的mqtt client
 */
class MqttPubClient {
public:
    MqttPubClient(std::shared_ptr<MQTTClient> client, const String &topic);
    
    MqttPubClient & operator<< (const String &s);
    MqttPubClient & operator<< (const char *s);
    
private:
    std::shared_ptr<MQTTClient> _client;
    String _topic;
};

#endif // MQTTCLIENT_H
