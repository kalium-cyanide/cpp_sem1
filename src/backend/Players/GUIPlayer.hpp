#include "../Players.hpp"
#include "../utility.hpp"
#include <future>

class GuiHumanPlayer : public IPlayer
{
  public:
    GuiHumanPlayer(color_t color, string_t name) : m_color(color), m_name(name)
    {
    }

    string_t getName() override
    {
        return m_name;
    }

    color_t getColor() override
    {
        return m_color;
    }

    Move doMove() override
    {
        m_move_promise = std::promise<Move>();

        std::future<Move> move_future = m_move_promise.get_future();

        return move_future.get();
    }

    void provideMove(const Move &move)
    {
        try
        {
            m_move_promise.set_value(move);
        }
        catch (const std::future_error &e)
        {
            std::cerr << "Promise error: " << e.what() << std::endl;
        }
    }

  private:
    color_t m_color;
    string_t m_name;
    std::promise<Move> m_move_promise;
};