
#pragma once

#include <boost/asio.hpp>
#include <boost/timer/timer.hpp>
#include <iostream>
#include <thread>
#include <string>

// заглушка для Action
struct Action {
    std::string id;
};

class TopTracker {
public:
    TopTracker(size_t max_actions);

    ~TopTracker();

    void AddAction(Action action, std::chrono::milliseconds ttl);

    std::vector<Action> GetActions();

    struct Item {
        Action action;
        boost::asio::steady_timer timer;

        Item(Action a, boost::asio::io_context &io, std::chrono::milliseconds ttl)
                : action(std::move(a)), timer(io, ttl) {}
    };

private:
    boost::asio::io_context io;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard; // чтобы io.run не завершился, когда контейнер пустой
    size_t max_actions;
    std::list<std::shared_ptr<Item>> actions_;
    std::mutex mutex_;
    std::jthread worker;
};
