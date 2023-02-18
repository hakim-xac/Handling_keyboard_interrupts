#include <iostream>
#include "Interface.h"

namespace KHAS {
    HookKeyBourd::HookKeyBourd()
    {
    }
    void HookKeyBourd::loop() {
        //for (;;) {
            hhook_ = SetWindowsHookEx(WH_KEYBOARD_LL, &hookKeyBourd, GetModuleHandle(NULL), 0);
            
        //}
    }

    LRESULT CALLBACK HookKeyBourd::hookKeyBourd(int nCode, WPARAM wParam, LPARAM lParam) {
        
        if (nCode == HC_ACTION) {
            //if (HookKeyBourd::hkb_->func) HookKeyBourd::hkb_->func(nCode, wParam, lParam);
        }
        return CallNextHookEx(0, nCode, wParam, lParam);
    }
    void HookKeyBourd::initHookKeyBourd(int nCode, WPARAM wParam, LPARAM lParam)
    {
        std::cout << "hello";
    }
}