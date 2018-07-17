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

#include "config.h"
#include "basic_config.h"

Config::Config() : _mqttServer("mqtt.coolex.info"), _mqttPort(8883),
    _mqttUser(MQTT_USER), _mqttPass(MQTT_PASS), _mqttTopic("/sensor"),
    _ssid(SSID), _wifiPassword(PASSWORD)
{
    _id = "esp8266-" + String(ESP.getChipId(), HEX);
}

