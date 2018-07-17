/*
 * <one line to give the program's name and a brief idea of what it does.>
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

#include "pmssensor.h"

PmsSensor::PmsSensor() : _pms(Serial)
{
}

const std::unordered_map<std::string, std::string> PmsSensor::data()
{
    PMS::DATA d;
    // 清空串口缓存
    while (Serial.available()) { Serial.read(); }
    _pms.requestRead();
    
    if(_pms.readUntil(d)) {
        std::unordered_map<std::string, std::string> result = {
            {"pm1.0", String(d.PM_AE_UG_1_0).c_str()},
            {"pm2.5", String(d.PM_AE_UG_2_5).c_str()},
            {"pm10", String(d.PM_AE_UG_10_0).c_str()}
        };
        
        return result;
    } else {
        return std::unordered_map<std::string, std::string>();
    }
}

void PmsSensor::begin()
{
    Serial.begin(PMS::BAUD_RATE);
    _pms.passiveMode();
    _pms.wakeUp();
}
