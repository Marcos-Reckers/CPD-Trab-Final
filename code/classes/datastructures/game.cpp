#include "game.hpp"

namespace DB
{
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
    int Game::writeToFile(std::fstream &file) const
    {
        // The data to be written
        std::array<const char *, GameFields> data({reinterpret_cast<const char *>(&(this->appid)),
                                                   this->name.c_str(),
                                                   this->developer.c_str(),
                                                   this->publisher.c_str(),
                                                   reinterpret_cast<const char *>(&(this->reviews)),
                                                   this->tags.c_str(),
                                                   reinterpret_cast<const char *>(&(this->price)),
                                                   reinterpret_cast<const char *>(&(this->release_date))});
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