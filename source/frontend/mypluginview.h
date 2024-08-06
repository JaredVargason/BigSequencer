#pragma once

#include "pluginterfaces/gui/iplugview.h"
#include <d3d11.h>
#include <windows.h>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_impl_dx11.h"

using namespace Steinberg;

namespace vargason::bigsequencer::frontend {
    class MyPluginView : public Steinberg::IPlugView {
    public:
        MyPluginView();
        ~MyPluginView();

        // Inherited via IPlugView
        tresult PLUGIN_API attached(void* parent, FIDString type) SMTG_OVERRIDE;
        tresult PLUGIN_API removed() SMTG_OVERRIDE;
        tresult PLUGIN_API isPlatformTypeSupported(FIDString type);
        tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) override;
        uint32 PLUGIN_API addRef() override;
        uint32 PLUGIN_API release() override;
        tresult PLUGIN_API onWheel(float distance) override;
        tresult PLUGIN_API onKeyDown(char16 key, int16 keyCode, int16 modifiers) override;
        tresult PLUGIN_API onKeyUp(char16 key, int16 keyCode, int16 modifiers) override;
        tresult PLUGIN_API getSize(ViewRect* size) override;
        tresult PLUGIN_API onSize(ViewRect* newSize) override;
        tresult PLUGIN_API onFocus(TBool state) override;
        tresult PLUGIN_API setFrame(IPlugFrame* frame) override;
        tresult PLUGIN_API canResize() override;
        tresult PLUGIN_API checkSizeConstraint(ViewRect* rect) override;
    private:

        HWND hwnd;
        ID3D11Device* device;
        ID3D11DeviceContext* deviceContext;
        IDXGISwapChain* swapChain;
        ID3D11RenderTargetView* renderTargetView;

        void initD3D();
        void cleanupD3D();

        void initImGui();
        void cleanupImGui();

        void render();
    };
}
