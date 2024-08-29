#include <iostream>
#include <chrono>
#include <iomanip>

#include "send_time2brd.h"

int main() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    uint64_t timestamp = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time_t);
    std::cout << std::put_time(now_tm, "%Y-%m-%d %H:%M:%S") << std::endl;
    std::cout << "timestamp: " << timestamp << std::endl;

    send_time2brd(timestamp);

    return 0;
}
