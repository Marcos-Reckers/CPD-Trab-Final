from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool as bool_t


cdef extern from "../CPP/database.hpp" namespace "database":
    vector[vector[string]] append(const string &path, int appID, const string &name, const string &developer, const string &publisher, const string &releaseDate, const string &popularTags, const string &gameDetails, const string &languages, const string &genre, const string &price, const string &review)
    
    string convert(const string &, int)
    
    vector[vector[string]] search(const vector[string] &, const vector[string] &, const vector[string] &, const vector[string] &, const vector[string] &, const vector[string] &, const vector[string] &, const string &, int, int, int, int, bool_t)

    bool_t validate()

    vector[string] getField(const string &)

def Append(Path: str, AppID: int, Name: str, Developer: str, Publisher: str, ReleaseDate: str, PopularTags: str, GameDetails: str, Languages: str, Genre: str, Price: str, Review: str) -> list[list[str]]:
    ret = append(bytes(Path, 'utf-8'), AppID, bytes(Name, 'utf-8'), bytes(Developer, 'utf-8'), bytes(Publisher, 'utf-8'), bytes(ReleaseDate, 'utf-8'), bytes(PopularTags, 'utf-8'), bytes(GameDetails, 'utf-8'), bytes(Languages, 'utf-8'), bytes(Genre, 'utf-8'), bytes(Price, 'utf-8'), bytes(Review, 'utf-8'))
    return [[x.decode('utf-8') for x in y] for y in ret]

def Convert(DB_Path: str, Size: int) -> str:
    return convert(bytes(DB_Path, 'utf-8'), Size).decode('utf-8')

def Search(genres: list[str], languagues: list[str], tags: list[str], dates: list[str], developers: list[str], publishers: list[str], reviews: list[str], name: str, minPrice: int, maxPrice: int, decade: int, appID: int, searchType: bool) -> list[list[str]]:
    gen = [bytes(x, 'utf-8') for x in genres]
    lang = [bytes(x, 'utf-8') for x in languagues]
    tag = [bytes(x, 'utf-8') for x in tags]
    date = [bytes(x, 'utf-8') for x in dates]
    dev = [bytes(x, 'utf-8') for x in developers]
    pub = [bytes(x, 'utf-8') for x in publishers]
    rev = [bytes(x, 'utf-8') for x in reviews]
    ret = search(gen, lang, tag, date, dev, pub, rev, bytes(name, 'utf-8'), minPrice, maxPrice, decade, appID, searchType)
    return [[x.decode('utf-8') for x in y] for y in ret]

def Validate() -> bool:
    return validate()

def GetField(field: str) -> list[str]:
    ret = getField(bytes(field, 'utf-8'))
    return [x.decode('utf-8') for x in ret]