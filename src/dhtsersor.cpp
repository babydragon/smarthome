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

#include "dhtsersor.h"
#include "common.h"

const char * DHTSersor::_type = "humidity";

DHTSersor::DHTSersor(const int &pin) : _dht(pin, DHT22) {
}


const std::unordered_map<std::string, std::string> DHTSersor::data() {
    Data d = _data();
    
    std::unordered_map<std::string, std::string> result = {
        {"temperature", String(d.t, 2).c_str()},
        {"humidity", String(d.h, 2).c_str()},
        {"heatIndex", String(d.hi, 2).c_str()}
    };
    
    return result;
}

DHTSersor::Data DHTSersor::_data() {
    float t = _dht.readTemperature();
    float h = _dht.readHumidity();
    float hi = _dht.computeHeatIndex(t, h, false);
    
    return DHTSersor::Data {t, h, hi};
}

void DHTSersor::begin()
{
     _dht.begin();
}
