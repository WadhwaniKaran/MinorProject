#pragma once

#include <mutex>
#include <iostream>
#include <sstream>
#include <thread>

#include <Windows.h>
#include <vector>
#include <algorithm>

#include "commons.h"

class FMT
{
    public:
    FMT() 
    {
        hConsole = GetStdHandle(STD_ERROR_HANDLE);
    }

    void progress_bar(f32 progress);

    private:
    void log(std::stringstream &msg);

    private:
    static std::mutex lock;
    static std::vector<std::thread::id> threads;
    HANDLE hConsole;
};


namespace fmt
{
void hide_console_cursor();
void show_console_cursor();
}