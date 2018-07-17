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

#ifndef NTPCLIENT_H
#define NTPCLIENT_H

#include <Arduino.h>
#include <WiFiUdp.h>

/**
 * ntp客户端，用于获取网络时钟，发送数据时带上时间戳
 */
class NtpClient
{
public:
    /**
     * Default constructor
     */
    NtpClient(const char* server, unsigned int interval = 300000, unsigned int localPort = 2800);

    /**
     * Destructor
     */
    ~NtpClient();
    
    void begin();
    
    unsigned long getTime();

private:
    unsigned int _localPort;
    IPAddress _address;
    WiFiUDP _udp;
    
    unsigned int _interval;
    unsigned long _lastSync;
    unsigned long _timestamp;
    static const unsigned int UDP_TIMEOUT = 1500;
    static const int NTP_PACKET_SIZE = 48;
    byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
    
private:
    void updateTs();
};

#endif // NTPCLIENT_H
