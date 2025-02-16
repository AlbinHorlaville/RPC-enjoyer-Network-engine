#include <functional>
#include <thread>
#include <chrono>

class Timer {
public:
    void setTimeout(std::function<void()> function, int delay) {
        this->clear = false;
        std::thread t([this, function, delay]() {
            if(this->clear) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            if(this->clear) return;
            function();
        });
        t.detach();
    }

    void setInterval(std::function<void()> function, int interval) {
        this->clear = false;
        std::thread t([this, function, interval]() {
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

private:
    bool clear = false;
};