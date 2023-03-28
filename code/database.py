import DB as database


def Convert(DB_Path: str, Size: int) -> str:
    return database.Convert(DB_Path, Size)


def Search(genres: list[str], languagues: list[str], tags: list[str], dates: list[str], developers: list[str], publishers: list[str], reviews: list[str], name: str, minPrice: int, maxPrice: int, decade: int, appID: int, searchType: bool) -> list[list[str]]:
    return database.Search(genres, languagues, tags, dates, developers, publishers, reviews, name, minPrice, maxPrice, decade, appID, searchType)


def Validate() -> bool:
    return database.Validate()


def GetField(field: str) -> list[str]:
    return database.GetField(field)
