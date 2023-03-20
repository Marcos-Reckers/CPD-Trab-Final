#include "game.hpp"

namespace DB
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
        else
        {
            const auto temp = IO::customSplit(date, ' ');
            auto TBA = temp[0];
            for (auto &c : TBA)
                c = toupper(c);
            if (TBA == "TBA" || TBA == "COMING" || TBA == "SUMMER" || TBA == "FALL" || TBA == "WINTER" || TBA == "SPRING" || TBA == "SOON" || TBA == "COMMING" || TBA == "EM" || TBA == "EVENTUALLY" || TBA == "WHEN" || TBA == "THIS")
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
            if (reviews[1] == 'i')
                return Reviews::Mixed;
            else
            {
                if (reviews[7] == 'P')
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
    Game::Game()
        : appid(0), name(""), developer(""), publisher(""), release_date("a"), tags(""), price(""), reviews(Reviews::Nan)
    {
    }

    Game::Game(const std::string &AppId, const std::string &Name, const std::string &Developer, const std::string &Publisher, const std::string &Release_date, const std::string &Tags, const std::string &Price, const std::string &Reviews)
        : name(Name), developer(Developer), publisher(Publisher), release_date(Release_date), tags(Tags), price(Price), reviews(StrToReviews(Reviews))
    {
        appid = std::stoi(AppId.substr(0, AppId.find("/")));
    }

    Game::Game(int AppId, const std::string &Name, const std::string &Developer, const std::string &Publisher, const ReleaseDate &Release_date, const std::string &Tags, const GamePrice &Price, const Reviews &Reviews)
        : appid(AppId), name(Name), developer(Developer), publisher(Publisher), release_date(Release_date), tags(Tags), price(Price), reviews(Reviews)
    {
    }

    const int GameSize = 1300;
    const int GameFields = 8;
    int Game::writeToFile(std::ofstream &file)
    {
        // The data to be written
        std::array<const char *, GameFields> data({reinterpret_cast<char *>(&(this->appid)),
                                                   this->name.c_str(),
                                                   this->developer.c_str(),
                                                   this->publisher.c_str(),
                                                   reinterpret_cast<char *>(&(this->reviews)),
                                                   this->tags.c_str(),
                                                   reinterpret_cast<char *>(&(this->price)),
                                                   reinterpret_cast<char *>(&(this->release_date))});
        // The sizes of the data
        std::array<int, GameFields> sizes({sizeof(this->appid),
                                           (int)name.size(),
                                           (int)developer.size(),
                                           (int)publisher.size(),
                                           sizeof(this->reviews),
                                           (int)tags.size(),
                                           sizeof(this->price),
                                           sizeof(this->release_date)});

        int size = 0; // The total size of the data
        for (int i = 0; i < GameFields; i++)
        {
            size += sizes[i];                                               // Add the size of the data
            size += sizeof(int);                                            // Add the size of the size
            file.write(reinterpret_cast<char *>(&(sizes[i])), sizeof(int)); // Write the size of the data
            file.write(data[i], sizes[i]);                                  // Write the data to the file
        }

        int bufferSize = GameSize - size; // No game should be larger than 1300 bytes (the largest in the file is 1174)
        for(int i = 0; i < bufferSize; i++)
        {
            file.write("", 1); // Write empty bytes to fill the buffer
        }

        // Return the number of bytes written
        return bufferSize + size;
    }

    bool Game::readFromFile(std::ifstream &file, int Index)
    {
        char buffer[GameSize];             // The buffer to read the data into
        if (!file.seekg(Index * GameSize)) // Seek to the correct position in the file (each game is 1300 bytes)
        {
            return false;
        }

        if (!file.read(buffer, GameSize)) // Read the data into the buffer
        {
            return false;
        }

        // 0 - AppId, 1 - Name, 2 - Developer, 3 - Publisher, 4 - Reviews, 5 - Tags, 6 - Price, 7 - Release Date
        std::array<char *, GameFields> data;
        std::array<int, GameFields> sizes;
        int offset = 0;

        for (int i = 0; i < GameFields; i++)
        {
            sizes[i] = *reinterpret_cast<int *>(buffer + offset);
            offset += sizeof(int);
            data[i] = buffer + offset;
            offset += sizes[i];
        }

        this->appid = *reinterpret_cast<int *>(data[0]);
        this->name = std::string(data[1], sizes[1]);
        this->developer = std::string(data[2], sizes[2]);
        this->publisher = std::string(data[3], sizes[3]);
        this->reviews = *reinterpret_cast<Reviews *>(data[4]);
        this->tags = std::string(data[5], sizes[5]);
        this->price = *reinterpret_cast<GamePrice *>(data[6]);
        this->release_date = *reinterpret_cast<ReleaseDate *>(data[7]);

        return true;
    }
}