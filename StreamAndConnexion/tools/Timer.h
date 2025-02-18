//
// Created by Albin Horlaville & Hugo Girard on 10/02/2025.
//

#pragma once

#include <functional>
#include <thread>
#include <chrono>
#include <future>
#include <exception>
#include <iostream>

class Timer {
public:
    Timer() : active(false) {}

    ~Timer() {
        stop();
    }

    void setTimeout(std::function<void()> function, int delay) {
        stop();  // Annule l'ancien timer s'il y en a un en cours
        active = true;

        promise = std::make_shared<std::promise<void>>();
        future = promise->get_future();

        thread = std::thread([this, function, delay]() {
            if (future.wait_for(std::chrono::milliseconds(delay)) == std::future_status::timeout) {
                if (active) function();
            }
        });
    }

    void setInterval(std::function<void()> function, int interval) {
        stop();  // Annule l'ancien intervalle s'il y en a un en cours
        active = true;

        promise = std::make_shared<std::promise<void>>();
        future = promise->get_future();

        thread = std::thread([this, function, interval]() {
            while (future.wait_for(std::chrono::milliseconds(interval)) == std::future_status::timeout) {
                if (!active) return;
                function();
            }
        });
    }

    void stop() {
        try {
            if (thread.joinable()) {
                active = false;
                promise->set_value();  // Force l'arrêt du `wait_for`
                thread.join();         // Attend la fin du thread proprement
            }
        }catch(std::exception const& e) {
            std::cout << "Connection lost !" << std::endl;
        }
    }

private:
    std::thread thread;
    std::shared_ptr<std::promise<void>> promise;
    std::future<void> future;
    bool active;
};
