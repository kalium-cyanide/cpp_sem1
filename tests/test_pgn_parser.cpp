#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include "utility.hpp"

TEST(PgnParser, ReadFile)
{
    std::string path = PROJECT_SOURCE_DIR;
    path += "/tests/a.txt";
    std::ifstream in(path);
    ASSERT_TRUE(in.good());

    std::string line;
    int count = 0;
    while(std::getline(in, line))
    {
        if(line.empty() || line[0] == '#') continue;
        
        std::istringstream iss(line);
        std::string from, to;
        
        if(iss >> from >> to) {
            if(from.length() == 2 && to.length() == 2) {
                count++;
            }
        }
    }
    EXPECT_EQ(count, 18);
}