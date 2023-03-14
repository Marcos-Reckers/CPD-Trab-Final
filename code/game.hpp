#pragma once
#include <iostream>
#include <string>

namespace cpd
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

    class ReleaseDate
    {
    private:
        int day;
        Months month;
        int year;
        int decade;

    public:
        ReleaseDate(const std::string &date);
        int getDay(){ return day; };
        Months getMonth(){ return month; };
        int getYear(){ return year; };
        int getDecade(){ return decade; };
        static Months StrToMonth(const std::string &month);
        static std::string MonthToStr(Months month);
        friend std::ostream &operator<<(std::ostream &os, const ReleaseDate &release)
        {
            os << MonthToStr(release.month) << " " << release.day << ", " << release.year;
            return os;
        };
    };

    class Game
    {
    private:
        int appid; // AppID
        std::string name; // Name
        std::string developer; // Developer
        std::string publisher; // Publisher
        ReleaseDate release_date; // Release Date
        std::string tags; // Tags
        int price; // Price (in cents)
        Reviews reviews; // Reviews (Overwhelmingly Positive, Very Positive, Positive, Mixed, Negative, Very Negative, Overwhelmingly Negative)
    public:
        Game(int appid, const std::string &name, const std::string &developer, const std::string &publisher, const std::string &release_date, const std::string &tags, const std::string &price, const std::string &reviews);
        const int getAppid(){ return appid;};
        std::string getName(){ return name;};
        std::string getDeveloper(){ return developer;};
        std::string getPublisher(){ return publisher;};
        ReleaseDate getReleaseDate(){ return release_date;};
        std::string getTags(){ return tags;};
        int getPrice(){ return price;};
        Reviews getReviews(){ return reviews;};
    };
}