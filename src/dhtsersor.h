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

#ifndef DHTSERSOR_H
#define DHTSERSOR_H

#include "Sensor.h"

#include <DHT.h>

/**
 * dht22传感器数据获取
 */
class DHTSersor : public Sensor
{
public:
    /**
     * Default constructor
     */
    DHTSersor(const int& pin);

    virtual const std::unordered_map<std::string, std::string> data();
    virtual const char* type() {
        return _type;
    };
    virtual void begin();
    
private:
    struct Data {
        float t; // 温度
        float h; // 湿度
        float hi; //体感温度
    };
    Data _data();
    const static char *_type;

private:
    DHT _dht;
};

#endif // DHTSERSOR_H
