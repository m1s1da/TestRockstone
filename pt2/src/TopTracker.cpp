
#include "TopTracker.h"

TopTracker::TopTracker(size_t max_actions) : max_actions(max_actions), work_guard(boost::asio::make_work_guard(io)) {
    worker = std::jthread([this] { io.run(); });
}

TopTracker::~TopTracker() {
    work_guard.reset();
    io.stop();
}

void TopTracker::AddAction(Action action, std::chrono::milliseconds ttl) {
    auto item = std::make_shared<Item>(std::move(action), io, ttl);

    std::lock_guard lock(mutex_);

    // FIFO
    if (actions_.size() >= max_actions) {
        auto oldest = actions_.front();
        oldest->timer.cancel();
        actions_.pop_front();
    }

    // Добавляем новый
    auto it = actions_.insert(actions_.end(), std::move(item));

    // Удаление по TTL
    auto weak_self = std::weak_ptr<Item>(*it);
    (*it)->timer.async_wait([this, weak_self](const boost::system::error_code &ec) {
        if (ec == boost::asio::error::operation_aborted) return;

        if (!ec) {
            std::lock_guard lock(mutex_);
            if (auto sp = weak_self.lock()) {
                actions_.remove(sp);
            }
        } else {
            std::clog << "Timer error: " << ec.message() << '\n';
        }
    });
}

std::vector<Action> TopTracker::GetActions() {
    std::lock_guard lock(mutex_);
    std::vector<Action> result;
    result.reserve(actions_.size());

    for (const auto &item: actions_) {
        result.push_back(item->action);
    }

    return result;
}