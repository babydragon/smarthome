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

#ifndef COMMON_H
#define COMMON_H

#include <Printable.h>
#include <Arduino.h>

#include "config.h"

class SerialOut {
public:
    SerialOut();
    ~SerialOut();
    SerialOut & operator<< (const char *d);
    template<typename T>
    SerialOut & operator<< (const T &value) {
        Serial1.print(value);
        return *this;
    }
};

extern SerialOut sout;

inline void showError() {
    digitalWrite(LED_BUILTIN, LOW);
}

#endif // COMMON_H

