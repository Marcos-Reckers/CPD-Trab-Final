from libcpp.vector cimport vector
from libcpp.string cimport string

path = "code/classes/.hpp"

cdef extern from path namespace "database":
    string convert(const string &, int)
    
    vector[vector[string]] search(  const vector[string] &,
                                    const vector[string] &, 
                                    const vector[string] &,
                                    const vector[string] &,
                                    const vector[string] &,
                                    const vector[string] &,
                                    const vector[string] &,
                                    const string &,
                                    int, int, int, int, bool)

    bool validate()

    vector[string] getField(const string &)

def Convert(DB_Path: str, Size: int) -> str:
    return convert(DB_Path, Size)

def Search( genres: list[str], 
            languagues: list[str], 
            tags: list[str], 
            dates: list[str], 
            developers: list[str], 
            publishers: list[str], 
            reviews: list[str], 
            name: string, 
            minPrice: int, maxPrice: int, decade: int, appID: int, searchType: bool) -> list[list[str]]:
    return search(genres, languagues, tags, dates, developers, publishers, reviews, name, minPrice, maxPrice, decade, appID, searchType)

def Validate() -> bool:
    return validate()

def GetField(field: str) -> list[str]:
    return getField(field)