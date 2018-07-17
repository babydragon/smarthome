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

#include "ntpclient.h"
#include "common.h"

#include <ESP8266WiFi.h>

NtpClient::NtpClient::NtpClient(const char* server, unsigned int interval, unsigned int localPort)
    : _localPort(localPort), _interval(interval), _timestamp(0), _lastSync(0)
{
    WiFi.hostByName(server, _address);
}

NtpClient::~NtpClient()
{
    _udp.stop();
}

void NtpClient::begin()
{
    _udp.begin(_localPort);
    updateTs();
}

void NtpClient::updateTs()
{
    while (_udp.parsePacket () > 0);   // 清空udp缓存
    // send udp package,copy from esp8266 arduino example
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011;   // LI, Version, Mode
    packetBuffer[1] = 0;     // Stratum, or type of clock
    packetBuffer[2] = 6;     // Polling Interval
    packetBuffer[3] = 0xEC;  // Peer Clock Precision
    // 8 bytes of zero for Root Delay & Root Dispersion
    packetBuffer[12]  = 49;
    packetBuffer[13]  = 0x4E;
    packetBuffer[14]  = 49;
    packetBuffer[15]  = 52;

    // all NTP fields have been given values, now
    // you can send a packet requesting a timestamp:
    _udp.beginPacket(_address, 123); //NTP requests are to port 123
    _udp.write(packetBuffer, NTP_PACKET_SIZE);
    _udp.endPacket();
    
    unsigned long startWait = millis();
    while(millis() - startWait < UDP_TIMEOUT) {
        int pkgSize = _udp.parsePacket();
        if(pkgSize >= NTP_PACKET_SIZE) {
            _udp.read(packetBuffer, NTP_PACKET_SIZE);
            
            unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
            unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
            unsigned long ts = highWord << 16 | lowWord;
            
            this->_timestamp = ts;
            this->_lastSync = millis();

            sout << "success sync NTP time: " << ts << "\n";
            return;
        }
    }
    
    sout << "wait ntp server response timeout!\n";
    return;
}

unsigned long NtpClient::getTime()
{
    unsigned long now = millis();
    if(now - _lastSync > _interval) {
        updateTs();
    } else {
        _timestamp += (now - _lastSync) / 1000;
    }
    
    return _timestamp;
}
