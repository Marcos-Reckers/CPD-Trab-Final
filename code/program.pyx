from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool as bool_t


cdef extern from "database.hpp" namespace "database":
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

    bool_t validate()

    vector[string] getField(const string &)

def Convert(DB_Path: bytes, Size: int) -> bytes:
    return convert(DB_Path, Size)

def Search( genres: list[bytes], 
            languagues: list[bytes], 
            tags: list[bytes], 
            dates: list[bytes], 
            developers: list[bytes], 
            publishers: list[bytes], 
            reviews: list[bytes], 
            name: bytes, 
            minPrice: int, maxPrice: int, decade: int, appID: int, searchType: bool) -> list[list[bytes]]:
    return search(genres, languagues, tags, dates, developers, publishers, reviews, name, minPrice, maxPrice, decade, appID, searchType)

def Validate() -> bool:
    return validate()

def GetField(field: bytes) -> list[bytes]:
    return getField(field)