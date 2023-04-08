#pragma once
#include "subclasses.hpp"

namespace DB
{
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
        Game(int AppId, const std::string &Name, const std::string &Developer, const std::string &Publisher, const ReleaseDate &Release_date, const std::string &Tags, const GamePrice &Price, const Reviews &Reviews);
        Game();
        ~Game() = default;
        const int getAppid() const { return appid; };
        std::string getName() const { return name; };
        std::string getDeveloper() const { return developer; };
        std::string getPublisher() const { return publisher; };
        ReleaseDate getReleaseDate() const { return release_date; };
        std::string getTags() const { return tags; };
        GamePrice getPrice() const { return price; };
        int getPriceInt() const { return price.getPrice(); };
        Reviews getReviews() const { return reviews; };
        int writeToFile(std::fstream &file) const;
        bool readFromFile(std::ifstream &file, int Index);
        friend std::ostream &operator<<(std::ostream &os, const Game &game)
        {
            os << "AppID: " << game.appid << " Name: " << game.name
               << " Developer: " << game.developer << " Publisher: " << game.publisher
               << " Release Date: " << game.release_date << " Tags: " << game.tags
               << " Price: " << game.price << " Reviews: " << ReviewsToStr(game.reviews);
            return os;
        };
        std::vector<std::string> DataOut() const
        {
            std::vector<std::string> data;
            data.push_back(std::to_string(this->getAppid()));
            data.push_back(this->getName());
            data.push_back(this->getDeveloper());
            data.push_back(this->getPublisher());
            data.push_back(this->getReleaseDate().toStr());
            data.push_back(this->getTags());
            data.push_back(this->getPrice().toStr());
            data.push_back(ReviewsToStr(this->getReviews()));
            return data;
        }
    };
}