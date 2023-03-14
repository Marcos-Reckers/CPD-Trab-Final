#include "game.hpp"

int main()
{
    cpd::ReleaseDate date("Jan 1, 2000");
    std::cout << date << std::endl;
    cpd::ReleaseDate date2("Feb 12, 2014");
    std::cout << date2 << std::endl;
    cpd::ReleaseDate date3("Mar 2019");
    std::cout << date3 << std::endl;
    cpd::ReleaseDate date4("2020");
    std::cout << date4 << std::endl;
    cpd::ReleaseDate date5("NaN");
    std::cout << date5 << std::endl;
    cpd::ReleaseDate date6("TBA");
    std::cout << date6 << std::endl;
    return 0;
}