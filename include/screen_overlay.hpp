#ifndef _SCREEN_OVERLAY_HPP
#define _SCREEN_OVERLAY_HPP

#include <Windows.h>

#include <iostream>
#include <string>

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

namespace screen_overlay
{
    class ScreenOverlay
    {
    private:
        std::wstring window_name_ ;

        HWND hwnd_ ;
        HWND parent_hwnd_ ;

    public:
        template <typename WindowNameType>
        explicit ScreenOverlay(WindowNameType&& window_name, HWND parent_hwnd=NULL)
        : window_name_(std::forward<WindowNameType>(window_name)),
          hwnd_(NULL),
          parent_hwnd_(parent_hwnd)
        {}

        ScreenOverlay(const ScreenOverlay&) = default ;
        ScreenOverlay& operator=(const ScreenOverlay&) = default ;

        ScreenOverlay(ScreenOverlay&&) = default ;
        ScreenOverlay& operator=(ScreenOverlay&&) = default ;

        virtual ~ScreenOverlay() noexcept = default ;

        bool init() {
            HINSTANCE hInstance = GetModuleHandle(nullptr);

            WNDCLASSW wc = {};
            wc.lpfnWndProc = WndProc;
            wc.hInstance = hInstance;
            // wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); // 背景を透明に
            wc.lpszClassName = window_name_.c_str() ;

            RegisterClassW(&wc);

            WINDOWINFO winfo ;
            winfo.cbSize = sizeof(WINDOWINFO) ;
            if(!GetWindowInfo(GetDesktopWindow(), &winfo)) {
                return false ;
            }
            auto left = winfo.rcWindow.left ;
            auto top = winfo.rcWindow.top ;
            auto width = winfo.rcWindow.right - winfo.rcWindow.left ;
            auto height = winfo.rcWindow.bottom - winfo.rcWindow.top ;

            std::cout << left << " " << top << " " << width << " " << height ;

            hwnd_ = CreateWindowExW(
                WS_EX_LAYERED,
                window_name_.c_str(),
                window_name_.c_str(),
                WS_POPUP | WS_VISIBLE,
                0, 0, width, height,
                parent_hwnd_, NULL, hInstance, NULL);

            if (hwnd_ == NULL) {
                return false ;
            }

            // データをウィンドウに渡す
            // auto data = new std::pair<std::vector<std::string>, std::vector<POINT>>(texts, points);
            // SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));

            // レイヤードウィンドウの透明設定
            // SetLayeredWindowAttributes(hwnd, 0, 255, LWA_ALPHA);
            if(!SetLayeredWindowAttributes(hwnd_, 0, 125, LWA_ALPHA)) {
                return false ;
            }

            ShowWindow(hwnd_, SW_SHOW);

            return true ;
        }

        std::string test() {
            return "test" ;
        }

        bool draw_text(const std::string& text, std::size_t x, std::size_t y) {

        }

        bool clear_text() {

        }

        bool update() {
            MSG msg ;
            if(PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
                DispatchMessageW(&msg) ;
            }
            return true ;
        }
    } ;

}

#endif
