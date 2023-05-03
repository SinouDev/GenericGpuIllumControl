#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "NvidiaAPIWrapper.h"

class MainLayer : public Walnut::Layer
{
public:

    MainLayer()
    {
        //NvU8 reg[] = { 0xF0 };

        ctrl.Init();
        ctrl.SelectGPU(0);
        //ctrl.SetTargetAddress(0x49, reg, sizeof(reg) / sizeof(NvU8));

        m_ColorFile.open(m_SaveConfig, std::ios::in | std::ios::out | std::ios::binary);
        if (!m_ColorFile.is_open())
        {
            m_RGBW.red        = 0x45;
            m_RGBW.green      = 0xC8;
            m_RGBW.blue       = 0x00;
            m_RGBW.white      = 0;
            m_RGBW.brightness = 0x64;
            MakeFile();
        }
        else 
            m_ColorFile.read((char*)&m_RGBW, sizeof(m_RGBW));

        ctrl.SetRGBW(m_RGBW);

        m_Colors[0]     = (float)m_RGBW.red      / 255.f;
        m_Colors[1]     = (float)m_RGBW.green    / 255.f;
        m_Colors[2]     = (float)m_RGBW.blue     / 255.f;
        m_Brightness    = (float)m_RGBW.brightness;
        m_White         = (float)m_RGBW.white;

        //m_ColorFile.close();
        //ctrl.Unload();
    }

    virtual ~MainLayer() override
    {

    }

    virtual void OnAttach() override
    {

    }

    virtual void OnDetach() override
    {
        if (m_ColorFile.is_open())
        {
            m_ColorFile.seekp(0);
            m_ColorFile.write((char*)&m_RGBW, sizeof(m_RGBW));
            std::cout << "Saving color\n";
        }
        m_ColorFile.close();
        ctrl.Unload();
    }

    virtual void OnUpdate(float ts) override
    {
        if (Walnut::Application::Get().GetMainWindowMinimized())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            // When minimized sleep this thread for saving CPU cycles
        }
    }

    virtual void OnUIRender() override
    {

        ImGui::Begin("Main");

        bool color_changed = false;
        
        color_changed |= ImGui::ColorEdit3("Color1", m_Colors);
        
        if (color_changed |= ImGui::ColorPicker3("Color", m_Colors)) // Undefined behaviour, need to be changed
        {
            m_RGBW.red        = m_Colors[0] * 255.f;
            m_RGBW.green      = m_Colors[1] * 255.f;
            m_RGBW.blue       = m_Colors[2] * 255.f;
        })

        if (color_changed |= (ImGui::SliderFloat("White", &m_White, 0.0f, 100.0f)))
        {
            m_RGBW.white = m_White;
        }

        if (color_changed |= (ImGui::SliderFloat("Brightness", &m_Brightness, 0.0f, 100.0f)))
        {
            m_RGBW.brightness = m_Brightness;
        }

        if (color_changed)
        {
            ctrl.SetRGBW(m_RGBW);
            std::cout << "Color changed\n";
        }

        ImGui::End();
        
    }

private:

    void MakeFile()
    {
        std::ofstream f;
        f.open(m_SaveConfig, std::ios::out | std::ios::binary);
        f.write((char*)&m_RGBW, sizeof(m_RGBW));
        f.close();
    }

private:

    NvidiaAPIWrapper ctrl;
    std::fstream m_ColorFile;
    RGBW m_RGBW;
    float m_Colors[3];
    float m_White;
    float m_Brightness;
    const char* m_SaveConfig = "nv-color-data.dat";

};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
    Walnut::ApplicationSpecification specs;
    specs.Name = "RGBW Controller";
    specs.Width = 1280;
    specs.Height = 720;
    auto app = new Walnut::Application(specs);
    app->PushLayer<MainLayer>();
    return app;
}
