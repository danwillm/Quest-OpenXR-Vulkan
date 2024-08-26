#pragma once

#include <atomic>

#include <android/native_window.h>

struct app_state
{
    std::atomic<bool> b_app_running = false;

    ANativeWindow *p_native_window = nullptr;

    int npx_width;
    int npx_height;
};