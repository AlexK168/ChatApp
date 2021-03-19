#include "MsgQueue.h"

bool MsgQueue::Item::operator<(const MsgQueue::Item& item) const {
    return this->id < item.id;
}

void MsgQueue::output() {
    clearScreen();
    for (const auto &item : this->queue) {
        std::cout << item.holder << ": " << item.message << std::endl;
    }
}

void MsgQueue::push(int id, std::string msg, std::string holder) {
    const std::lock_guard<std::mutex> lock(mtx);
    if(queue.size() >= size)
        queue.pop_front();
    MsgQueue::Item newItem {id, std::move(msg), std::move(holder)};
    queue.push_back(newItem);
    sort(queue.begin(), queue.end());
    output();
}

MsgQueue::MsgQueue(int size) {
    this->size = size;
}

void MsgQueue::clearScreen() {
    {
        if (!cur_term)
        {
            int result;
            setupterm( NULL, STDOUT_FILENO, &result );
            if (result <= 0) return;
        }

        putp( tigetstr( "clear" ) );
    }
}


