#include "fmt.h"

std::mutex FMT::lock;
std::vector<std::thread::id> FMT::threads;

void FMT::progress_bar(f32 progress)
{
    if(progress > 1.0) return;
    i32 barWidth = 70;

    std::stringstream msg;
    msg << "[";

    i32 pos = barWidth * progress;
    for (i32 i = 0; i < barWidth; ++i) {
        if (i < pos) msg << "=";
        else if (i == pos) msg << ">";
        else msg << " ";
    }

    msg << "] " << i32(progress * 100.0) << " %";
    log(msg);
}

void FMT::log(std::stringstream &msg)
{
    std::thread::id id = std::this_thread::get_id();
        
    bool found = false;
    for(const auto &thread: threads) {
        if(thread == id) {found = true; break;}
    }
    if(!found) threads.push_back(id);

    lock.lock();

    auto itr = std::find(threads.begin(), threads.end(), id);
    i32 index = std::distance (threads.begin (), itr);

    SetConsoleCursorPosition(hConsole, COORD{0, (short)(index + 15)});
    std::clog << "\rThread " << index << ": " << msg.str();
    std::clog.flush();

    SetConsoleCursorPosition(hConsole, COORD{0, (short)(index + 20)});

    lock.unlock();
}

void fmt::hide_console_cursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void fmt::show_console_cursor()
{
    HANDLE consoleHandle = GetStdHandle(STD_ERROR_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(consoleHandle, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}