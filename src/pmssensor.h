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

#ifndef PMSSENSOR_H
#define PMSSENSOR_H

#include "Sensor.h"

#include <PMS.h>

/**
 * PMS7003传感器
 */
class PmsSensor : public Sensor
{
public:
    // 强制使用Serial
    PmsSensor();

    virtual const std::unordered_map<std::string, std::string> data();
    virtual const char* type() {
        return "AQI";
    };
    virtual void begin();
    
private:
    PMS _pms;
};

#endif // PMSSENSOR_H
