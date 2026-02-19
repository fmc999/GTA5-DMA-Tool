#pragma once

class ArmorManager {
public:
    // 静态成员变量
    static bool bLockArmor;
    static float currentArmor;
    
    // 主要功能函数
    static bool UpdateArmor();
    static bool SetArmor(float value);
    static bool OnDMAFrame();
    
private:
    // 内存读取相关
    static bool ReadArmorFromMemory(float& value);
    static bool WriteArmorToMemory(float value);
};