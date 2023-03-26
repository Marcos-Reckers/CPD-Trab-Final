#pragma once
#include "../strings.hpp"

namespace DB
{
    enum class Reviews
    {
        Nan,
        OverwhelminglyNegative,
        VeryNegative,
        Negative,
        MostlyNegative,
        Mixed,
        MostlyPositive,
        Positive,
        VeryPositive,
        OverwhelminglyPositive
    };

    Reviews StrToReviews(const std::string &reviews);
    std::string ReviewsToStr(Reviews reviews);

    enum class Months
    {
        Nan,
        Jan,
        Feb,
        Mar,
        Apr,
        May,
        Jun,
        Jul,
        Aug,
        Sep,
        Oct,
        Nov,
        Dec
    };

    class GamePrice
    {
    private:
        int price;

    public:
        GamePrice(const std::string &price);
        int getPrice() const { return price; };
        friend std::ostream &operator<<(std::ostream &os, const GamePrice &price)
        {
            if (price.price == -1)
            {
                os << "NaN";
                return os;
            }
            else if (price.price == 0)
            {
                os << "Free";
                return os;
            }
            os << "$" << price.price / 100 << "." << price.price % 100;
            return os;
        };
        std::string toStr();
    };

    class ReleaseDate
    {
    private:
        int day;
        Months month;
        int year;
        int decade;

    public:
        ReleaseDate(const std::string &date);
        int getDay() { return day; };
        Months getMonth() { return month; };
        int getYear() { return year; };
        std::string getYearStr() const { return year > 0 ? std::to_string(year) : year == -1 ? "NaN" : "TBA" ; };
        int getDecade() { return decade; };
        static Months StrToMonth(const std::string &month);
        static std::string MonthToStr(Months month);
        std::string toStr();
        friend std::ostream &operator<<(std::ostream &os, const ReleaseDate &release)
        {
            os << MonthToStr(release.month) << " " << release.day << ", " << release.year;
            return os;
        };
    };
}