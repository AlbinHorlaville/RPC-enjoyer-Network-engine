//
// Library found here : https://www.fluentcpp.com/2018/12/28/timer-cpp/
//

#ifndef TIMER_H
#define TIMER_H

#include <thread>

class Timer {
    bool clear = false;

public:
    void setTimeout(auto function, int delay) {
        this->clear = false;
        std::thread t([=]() {
            if(this->clear) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            if(this->clear) return;
            function();
        });
        t.detach();
    }

    void setInterval(auto function, int interval) {
        this->clear = false;
        std::thread t([=]() {
            while(true) {
                if(this->clear) return;
                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
                if(this->clear) return;
                function();
            }
        });
        t.detach();
    }

    void stop() {
        this->clear = true;
    }
};

#endif //TIMER_H
