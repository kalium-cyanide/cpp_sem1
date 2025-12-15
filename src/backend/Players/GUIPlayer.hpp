#pragma once

#include "../Players.hpp"
#include "../utility.hpp"
#include <condition_variable>
#include <mutex>
#include <optional>


class GuiHumanPlayer : public IPlayer {
public:
    GuiHumanPlayer(color_t color, string_t name) : m_color(color), m_name(name) {
    }

    string_t getName() override {
        return m_name;
    }

    color_t getColor() override {
        return m_color;
    }

    Move doMove() override {
        std::unique_lock<std::mutex> lock(m_mutex);

        m_cv.wait(lock, [this] { return m_pending_move.has_value(); });

        Move move = m_pending_move.value();

        m_pending_move.reset();

        return move;
    }


    void provideMove(const Move &move) {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_pending_move = move;
        }

        m_cv.notify_one();
    }

private:
    color_t m_color;
    string_t m_name;

    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::optional<Move> m_pending_move;
};