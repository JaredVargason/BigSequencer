#pragma once

#include "mypluginview.h"

using namespace Steinberg;

namespace vargason::bigsequencer::frontend {

    MyPluginView::MyPluginView() {
        hwnd = nullptr;
        device = nullptr;
        deviceContext = nullptr;
        swapChain = nullptr;
        renderTargetView = nullptr;
    }

    MyPluginView::~MyPluginView() {

    }

    tresult PLUGIN_API MyPluginView::attached(void* parent, FIDString type) {
        if (strcmp(type, kPlatformTypeHWND) == 0) {
            hwnd = (HWND)parent;
            this->initD3D();
            this->initImGui();
            return kResultOk;
        }
        return kResultFalse;
    }

    tresult PLUGIN_API MyPluginView::removed() {
        cleanupImGui();
        cleanupD3D();
        return kResultOk;
    }

    void MyPluginView::initD3D() {
        UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

        DXGI_SWAP_CHAIN_DESC scd;
        ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
        scd.BufferCount = 1; // double buffering
        scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        scd.OutputWindow = hwnd;
        scd.SampleDesc.Count = 1; // No multi-sampling
        scd.Windowed = TRUE; // Windowed mode

        D3D_FEATURE_LEVEL featureLevel;

        D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            0,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &scd,
            &swapChain,
            &device,
            &featureLevel,
            &deviceContext
        );
    }

    void MyPluginView::cleanupD3D() {
        delete renderTargetView;
        delete device;
        delete deviceContext;
        delete swapChain;
        hwnd = nullptr;
    }

    void MyPluginView::initImGui() {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls

        // Initialize ImGui for DirectX and Win32
        ImGui_ImplWin32_Init(hwnd);
        ImGui_ImplDX11_Init(device, deviceContext);
    }

    void MyPluginView::cleanupImGui() {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void MyPluginView::render() {
        // Start the ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Your ImGui code goes here
        ImGui::Begin("Hello, World!");
        ImGui::Text("This is some text.");
        ImGui::End();

        // Rendering
        ImGui::Render();

        const float clear_color[4] = { 0.45f, 0.55f, 0.60f, 1.00f };
        deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
        deviceContext->ClearRenderTargetView(renderTargetView, clear_color);
        
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        swapChain->Present(1, 0);
    }

    tresult PLUGIN_API MyPluginView::isPlatformTypeSupported(FIDString type)
    {
        return kResultOk;
    }

    tresult PLUGIN_API MyPluginView::queryInterface(const TUID _iid, void** obj)
    {
        return kResultOk;
    }

    uint32 PLUGIN_API MyPluginView::addRef()
    {
        return 0;
    }

    uint32 PLUGIN_API MyPluginView::release()
    {
        return 0;
    }

    tresult PLUGIN_API MyPluginView::onWheel(float distance)
    {
        return kResultOk;
    }

    tresult PLUGIN_API MyPluginView::onKeyDown(char16 key, int16 keyCode, int16 modifiers)
    {
        return kResultOk;
    }

    tresult PLUGIN_API MyPluginView::onKeyUp(char16 key, int16 keyCode, int16 modifiers)
    {
        return kResultOk;
    }

    tresult PLUGIN_API MyPluginView::getSize(ViewRect* size)
    {
        return kResultOk;
    }

    tresult PLUGIN_API MyPluginView::onSize(ViewRect* newSize)
    {
        return kResultOk;
    }

    tresult PLUGIN_API MyPluginView::onFocus(TBool state)
    {
        return kResultOk;
    }

    tresult PLUGIN_API MyPluginView::setFrame(IPlugFrame* frame)
    {
        return kResultOk;
    }

    tresult PLUGIN_API MyPluginView::canResize()
    {
        return kResultOk;
    }

    tresult PLUGIN_API MyPluginView::checkSizeConstraint(ViewRect* rect)
    {
        return kResultOk;
    }
}