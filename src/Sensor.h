
#pragma once

#include <Arduino.h>
#include <unordered_map>

class Sensor {
public:
    /**
     * 传感器数据
     * @return hash映射
     */
    virtual const std::unordered_map<std::string, std::string> data() = 0;
    
    /**
     * 传感器类型
     * @return 类型名称
     */
    virtual const char* type() = 0;
    
    /**
     * 初始化。
     * 
     * 由于传感器对象在全局创建，而初始化需要在setup函数中，
     * 因此初始化操作都放在这个函数，构造函数不涉及硬件相关初始化。
     */
    virtual void begin() = 0;
};
