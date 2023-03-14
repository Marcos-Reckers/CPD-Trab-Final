#include "game.hpp"

namespace cpd
{
    ReleaseDate::ReleaseDate(const std::string &date)
    {
        if (date == "NaN")
        {
            this->day = -1;
            this->month = Months::Nan;
            this->year = -1;
            this->decade = -1;
        }
        else if (date == "TBA" || date == "Coming Soon")
        {
            this->day = -2;
            this->month = Months::Nan;
            this->year = -2;
            this->decade = -2;
        }
        else
        {
            switch (date.length())
            {
            case 4 /*2004*/:
                this->month = Months::Nan;
                this->day = -1;
                this->year = std::stoi(date.substr(0, 4));
                this->decade = std::stoi(date.substr(3, 1));
                break;
            case (3 /*Apr*/ + 1 /* */ + 4 /*2002*/):
                this->month = StrToMonth(date.substr(0, 3));
                this->day = -1;
                this->year = std::stoi(date.substr(4, 4));
                this->decade = std::stoi(date.substr(6, 1));
                break;
            case (3 /*Jan*/ + 1 /* */ + 1 /*1*/ + 2 /*, */ + 4 /*2014*/):
                this->month = StrToMonth(date.substr(0, 3));
                this->day = std::stoi(date.substr(4, 1));
                this->year = std::stoi(date.substr(7, 4));
                this->decade = std::stoi(date.substr(8, 1));
                break;
            case (3 /*Mar*/ + 1 /* */ + 2 /*14*/ + 2 /*, */ + 4 /*2015*/):
                this->month = StrToMonth(date.substr(0, 3));
                this->day = std::stoi(date.substr(4, 2));
                this->year = std::stoi(date.substr(8, 4));
                this->decade = std::stoi(date.substr(9, 1));
                break;
            default:
                break;
            }
        }
    }

    Months ReleaseDate::StrToMonth(const std::string &month)
    {
        int sum = 0; // Sum of ASCII values of characters in string
        for (auto cha : month)
            sum += cha; // Add ASCII value of each character to sum

        switch (sum) // Return corresponding month
        {
        case 281:
            return Months::Jan;
        case 269:
            return Months::Feb;
        case 288:
            return Months::Mar;
        case 291:
            return Months::Apr;
        case 295:
            return Months::May;
        case 301:
            return Months::Jun;
        case 299:
            return Months::Jul;
        case 285:
            return Months::Aug;
        case 296:
            return Months::Sep;
        case 294:
            return Months::Oct;
        case 307:
            return Months::Nov;
        case 268:
            return Months::Dec;
        default:
            return Months::Nan;
        }
    }

    std::string ReleaseDate::MonthToStr(Months month)
    {
        switch (month)
        {
        case Months::Jan:
            return static_cast<std::string>("Jan");
        case Months::Feb:
            return static_cast<std::string>("Feb");
        case Months::Mar:
            return static_cast<std::string>("Mar");
        case Months::Apr:
            return static_cast<std::string>("Apr");
        case Months::May:
            return static_cast<std::string>("May");
        case Months::Jun:
            return static_cast<std::string>("Jun");
        case Months::Jul:
            return static_cast<std::string>("Jul");
        case Months::Aug:
            return static_cast<std::string>("Aug");
        case Months::Sep:
            return static_cast<std::string>("Sep");
        case Months::Oct:
            return static_cast<std::string>("Oct");
        case Months::Nov:
            return static_cast<std::string>("Nov");
        case Months::Dec:
            return static_cast<std::string>("Dec");
        default:
            return "NaN";
        }
    }


    Reviews StrToReviews(const std::string &reviews)
    {
        if (reviews[0] < '9' && reviews[0] > '0')
        {
            return Reviews::Nan;
        }

        switch (reviews[0])
        {
        case 'O':
            if (reviews[15] == 'P')
            {
                return Reviews::OverwhelminglyPositive;
            }
            else
            {
                return Reviews::OverwhelminglyNegative;
            }
            break;
        case 'V':
            if (reviews[5] == 'P')
            {
                return Reviews::VeryPositive;
            }
            else
            {
                return Reviews::VeryNegative;
            }
            break;
        case 'P':
            return Reviews::Positive;
            break;
        case 'N':
            return Reviews::Negative;
            break;
        case 'M':
            if(reviews[1] == 'i')
                return Reviews::Mixed;
            else
            {   
                if(reviews[7] == 'P')
                {
                    return Reviews::MostlyPositive;
                }
                else
                {
                    return Reviews::MostlyNegative;
                }
            }
            break;
        default:
            return Reviews::Nan;
        }
    }
    std::string ReviewsToStr(Reviews reviews)
    {
        switch (reviews)
        {
        case Reviews::OverwhelminglyPositive:
            return static_cast<std::string>("Overwhelmingly Positive");
        case Reviews::VeryPositive:
            return static_cast<std::string>("Very Positive");
        case Reviews::Positive:
            return static_cast<std::string>("Positive");
        case Reviews::Mixed:
            return static_cast<std::string>("Mixed");
        case Reviews::Negative:
            return static_cast<std::string>("Negative");
        case Reviews::VeryNegative:
            return static_cast<std::string>("Very Negative");
        case Reviews::OverwhelminglyNegative:
            return static_cast<std::string>("Overwhelmingly Negative");
        case Reviews::MostlyPositive:
            return static_cast<std::string>("Mostly Positive");
        case Reviews::MostlyNegative:
            return static_cast<std::string>("Mostly Negative");
        default:
            return static_cast<std::string>("NaN");
        }
    }

    Game::Game(const std::string &AppId, const std::string &Name, const std::string &Developer, const std::string &Publisher, const std::string &Release_date, const std::string &Tags, const std::string &Price, const std::string &Reviews)
        : name(Name), developer(Developer), publisher(Publisher), release_date(Release_date), tags(Tags), price(Price), reviews(StrToReviews(Reviews))
    {
        appid = std::stoi(AppId);
    }
}