#pragma once
#include <functional>
#include <Windows.h>

namespace KHAS {
    class HookKeyBourd
    {
    public:
        HookKeyBourd();
        void loop();
    public:
        std::function<void(int, WPARAM, LPARAM)> func;
    private:
        static LRESULT CALLBACK hookKeyBourd(int nCode, WPARAM wParam, LPARAM lParam);
        void initHookKeyBourd(int nCode, WPARAM wParam, LPARAM lParam);
    private:
        HHOOK hhook_{};
        int frequency_{ 1000 };
        int length_second_{ 1000 };
    private:
        static HookKeyBourd hkb_;
    };

}
