#include <gtest/gtest.h>

#include "Figures/King.hpp"
#include "Figures/Queen.hpp"
#include "Figures/Rook.hpp"
#include "Figures/Bishop.hpp"
#include "Figures/Knight.hpp"
#include "Figures/Pawn.hpp"
#include "utility.hpp"

using namespace std;
using namespace chess;

/* ---------- King ---------- */
TEST(King, NormalMove)
{
    King k(WHITE);
    EXPECT_TRUE (k.isPossibleMove({{4,4}, {4,5}}));
    EXPECT_TRUE (k.isPossibleMove({{4,4}, {5,5}}));
    EXPECT_FALSE(k.isPossibleMove({{4,4}, {4,6}}));
    EXPECT_FALSE(k.isPossibleMove({{4,4}, {4,4}}));
}

/* ---------- Queen ---------- */
TEST(Queen, Move)
{
    Queen q(WHITE);
    EXPECT_TRUE(q.isPossibleMove({{3,3}, {3,7}}));
    EXPECT_TRUE(q.isPossibleMove({{3,3}, {7,3}}));
    EXPECT_TRUE(q.isPossibleMove({{3,3}, {5,5}}));
    EXPECT_FALSE(q.isPossibleMove({{3,3}, {5,6}}));
}

/* ---------- Rook ---------- */
TEST(Rook, Move)
{
    Rook r(WHITE);
    EXPECT_TRUE(r.isPossibleMove({{0,0}, {0,7}}));
    EXPECT_TRUE(r.isPossibleMove({{0,0}, {7,0}}));
    EXPECT_FALSE(r.isPossibleMove({{0,0}, {1,1}}));
}

/* ---------- Bishop ---------- */
TEST(Bishop, Move)
{
    Bishop b(WHITE);
    EXPECT_TRUE(b.isPossibleMove({{2,2}, {5,5}}));
    EXPECT_TRUE(b.isPossibleMove({{2,2}, {0,4}}));
    EXPECT_FALSE(b.isPossibleMove({{2,2}, {2,4}}));
}

/* ---------- Knight ---------- */
TEST(Knight, Move)
{
    Knight n(WHITE);
    EXPECT_TRUE(n.isPossibleMove({{3,3}, {5,4}}));
    EXPECT_TRUE(n.isPossibleMove({{3,3}, {1,2}}));
    EXPECT_FALSE(n.isPossibleMove({{3,3}, {4,4}}));
}

/* ---------- Pawn ---------- */
TEST(Pawn, Move)
{
    Pawn p(WHITE);
    EXPECT_TRUE(p.isPossibleMove({{0,1}, {0,2}}));
    EXPECT_TRUE(p.isPossibleMove({{0,1}, {0,3}}));
    EXPECT_FALSE(p.isPossibleMove({{0,1}, {1,2}}));
    EXPECT_FALSE(p.isPossibleMove({{0,1}, {0,1}}));
}