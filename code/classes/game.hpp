#pragma once
#include <iostream>
#include <string>
#include <vector>

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
        GamePrice(const std::string &price)
        {
            if (price.length() == 0)
            {
                this->price = -1;
            }
            else if (price[0] == '$')
            {
                this->price = static_cast<int>(std::stof(price.substr(1)) * 100);
            }
            else
            {
                this->price = 0;
            }
        };
        int getPrice() { return price; };
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
        int getDecade() { return decade; };
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
        int appid;                // AppID
        std::string name;         // Name
        std::string developer;    // Developer
        std::string publisher;    // Publisher
        ReleaseDate release_date; // Release Date
        std::string tags;         // Tags
        GamePrice price;          // Price (in cents)
        Reviews reviews;          // Reviews (Overwhelmingly Positive, Very Positive, Positive, Mixed, Negative, Very Negative, Overwhelmingly Negative)
    public:
        Game(const std::string &AppId, const std::string &Name, const std::string &Developer, const std::string &Publisher, const std::string &Release_date, const std::string &Tags, const std::string &Price, const std::string &Reviews);
        Game();
        ~Game() = default;
        const int getAppid() { return appid; };
        std::string getName() { return name; };
        std::string getDeveloper() { return developer; };
        std::string getPublisher() { return publisher; };
        ReleaseDate getReleaseDate() { return release_date; };
        std::string getTags() { return tags; };
        int getPrice() { return price.getPrice(); };
        Reviews getReviews() { return reviews; };
        friend std::ostream &operator<<(std::ostream &os, const Game &game)
        {
            os << "AppID: " << game.appid << " Name: " << game.name
               << " Developer: " << game.developer << " Publisher: " << game.publisher
               << " Release Date: " << game.release_date << " Tags: " << game.tags
               << " Price: " << game.price << " Reviews: " << ReviewsToStr(game.reviews);
            return os;
        };
    };
}