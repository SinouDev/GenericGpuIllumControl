#include "NvidiaAPIWrapper.h"

#pragma comment(lib, "nvapi64.lib")

NvidiaAPIWrapper::NvidiaAPIWrapper()
{
    NvU32 nvDisplayCount = 0;

    for (unsigned int i = 0; m_Status == NVAPI_OK; i++)
    {
        m_Status = NvAPI_EnumNvidiaDisplayHandle(i, &m_HandleDisplay_a[i]);

        if (m_Status == NVAPI_OK)
            nvDisplayCount++;
    }

    NvU32 gpuCount = 0;
    m_Status = NvAPI_EnumPhysicalGPUs(m_HandleGPU_a, &gpuCount);

}

void NvidiaAPIWrapper::SelectGPU(NvU8 n)
{
    m_Status = NvAPI_GetPhysicalGPUsFromDisplay(m_HandleDisplay_a[n], &m_HandleGpu, &m_PGpuCount);
}

void NvidiaAPIWrapper::SetRGBW(NvU8 r, NvU8 g, NvU8 b, NvU8 w, NvU8 brightness)
{
    m_PIllumZonesControl.zones[0].data.rgbw.data.manualRGBW.rgbwParams.brightnessPct = brightness;
    m_PIllumZonesControl.zones[0].data.rgbw.data.manualRGBW.rgbwParams.colorR = r;
    m_PIllumZonesControl.zones[0].data.rgbw.data.manualRGBW.rgbwParams.colorG = g;
    m_PIllumZonesControl.zones[0].data.rgbw.data.manualRGBW.rgbwParams.colorB = b;
    m_PIllumZonesControl.zones[0].data.rgbw.data.manualRGBW.rgbwParams.colorW = w;
    m_PIllumZonesControl.zones[1].data.singleColor.data.manualSingleColor.singleColorParams.brightnessPct = brightness;

    m_Status = NvAPI_GPU_ClientIllumZonesSetControl(m_HandleGpu, &m_PIllumZonesControl);
}

void NvidiaAPIWrapper::SetRGBW(RGBW rgbw)
{
    m_PIllumZonesControl.zones[0].data.rgbw.data.manualRGBW.rgbwParams = *reinterpret_cast<NV_GPU_CLIENT_ILLUM_ZONE_CONTROL_DATA_MANUAL_RGBW_PARAMS*>(&rgbw);
    m_PIllumZonesControl.zones[1].data.singleColor.data.manualSingleColor.singleColorParams.brightnessPct = rgbw.brightness;

    m_Status = NvAPI_GPU_ClientIllumZonesSetControl(m_HandleGpu, &m_PIllumZonesControl);
}

void NvidiaAPIWrapper::Init()
{
    m_Status = NvAPI_Initialize();

    memset(&m_PIllumZonesControl, 0, sizeof(m_PIllumZonesControl));

    m_PIllumZonesControl.version = 0x1194C; // 72012;
    m_PIllumZonesControl.numIllumZonesControl = 2;
    m_PIllumZonesControl.bDefault = false;
    m_PIllumZonesControl.rsvdField = 0;
    m_PIllumZonesControl.zones[0].type = NV_GPU_CLIENT_ILLUM_ZONE_TYPE_RGBW;
    m_PIllumZonesControl.zones[0].ctrlMode = NV_GPU_CLIENT_ILLUM_CTRL_MODE_MANUAL;
    m_PIllumZonesControl.zones[1].type = NV_GPU_CLIENT_ILLUM_ZONE_TYPE_SINGLE_COLOR;
    m_PIllumZonesControl.zones[1].ctrlMode = NV_GPU_CLIENT_ILLUM_CTRL_MODE_MANUAL;

    m_I2cInfo.version = NV_I2C_INFO_VER;
    m_I2cInfo.displayMask = 0;
    m_I2cInfo.bIsDDCPort = 0;
    m_I2cInfo.i2cDevAddress = m_I2cReadDeviceAddr;
    m_I2cInfo.pbI2cRegAddress = m_RegisterAddr;
    m_I2cInfo.regAddrSize = m_RegisterAddSize;
    m_I2cInfo.pbData = 0;
    m_I2cInfo.cbSize = 1;
    m_I2cInfo.i2cSpeed = NVAPI_I2C_SPEED_DEPRECATED;
    m_I2cInfo.i2cSpeedKhz = NVAPI_I2C_SPEED_DEFAULT;
    m_I2cInfo.portId = 0;
    m_I2cInfo.bIsPortIdSet = 0;
}

void NvidiaAPIWrapper::Read()
{
    m_Status = NvAPI_I2CRead(m_HandleGpu, &m_I2cInfo);
}

void NvidiaAPIWrapper::SetTargetAddress(NvU8 deviceAddress, NvU8* registerAddress, NvU32 registerAddressSize)
{
    m_i2cDeviceAddr = deviceAddress;
    m_I2cWriteDeviceAddr = m_i2cDeviceAddr << 1;
    m_I2cReadDeviceAddr = m_I2cWriteDeviceAddr | 1;
    m_RegisterAddr, registerAddress;
    m_RegisterAddSize = registerAddressSize;
}

void NvidiaAPIWrapper::Unload()
{
    NvAPI_Unload();
}
