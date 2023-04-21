#pragma once

#include <Windows.h>
#include "nvapi.h"

struct RGBW
{
    NvU8 red        = 0xFF;          // Red color channel
    NvU8 green      = 0xFF;          // Green color channel
    NvU8 blue       = 0xFF;          // Blue color channel
    NvU8 white      = 0;             // White color channel
    NvU8 brightness = 0x64; // MAX 100% Brightness
};

class NvidiaAPIWrapper
{
public:

    NvidiaAPIWrapper();

    void SelectGPU(NvU8 n);
    void SetRGBW(NvU8 red, NvU8 green, NvU8 blue, NvU8 white = 0xFF, NvU8 brightness = 0x64);
    void SetRGBW(RGBW rgbw);
    void Init();
    void Read();
    void SetTargetAddress(NvU8 deviceAddress, NvU8* registerAddress, NvU32 registerAddressSize);
    void Unload();

    inline NvAPI_Status GetStatus() { return m_Status; }

private:

    NvDisplayHandle m_HandleDisplay_a[NVAPI_MAX_PHYSICAL_GPUS * NVAPI_MAX_DISPLAY_HEADS] = { 0 };
    NvPhysicalGpuHandle m_HandleGPU_a[NVAPI_MAX_PHYSICAL_GPUS] = { 0 }; // handle to GPUs
    NvAPI_Status m_Status;
    NV_GPU_CLIENT_ILLUM_ZONE_CONTROL_PARAMS m_PIllumZonesControl;
    NV_I2C_INFO_V3 m_I2cInfo = { 0 };
    NvU8 m_i2cDeviceAddr;
    NvU8 m_I2cWriteDeviceAddr;
    NvU8 m_I2cReadDeviceAddr;
    NvU8* m_RegisterAddr;
    NvU32 m_RegisterAddSize;
    NvPhysicalGpuHandle m_HandleGpu = NULL;
    NvU32 m_PGpuCount = 0;

};
