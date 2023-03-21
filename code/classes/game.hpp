#pragma once
#include "subclasses.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <array>

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
        std::string getName() { return name; };
        std::string getDeveloper() { return developer; };
        std::string getPublisher() { return publisher; };
        ReleaseDate getReleaseDate() { return release_date; };
        std::string getTags() { return tags; };
        int getPrice() { return price.getPrice(); };
        Reviews getReviews() { return reviews; };
        int writeToFile(std::ofstream &file);
        bool readFromFile(std::ifstream &file, int Index);
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