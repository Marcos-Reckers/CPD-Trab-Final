// #include "database.hpp"

// void test()
// {
//     std::ifstream file("data/Steam.db.pat");
//     std::ifstream strings("data/Steam.db.pat.str");
//     if (!file.is_open() || !strings.is_open())
//     {
//         std::cout << "Error opening file" << std::endl;
//         return;
//     }

//     Trees::Patricia patricia;
//     patricia.readFromFile(file, strings);

//     file.close();
//     strings.close();

//     auto a = patricia.SearchPrefix("0");
//     for(auto item : a)
//     {
//         std::cout << item << std::endl;
//     }

//     std::ofstream file2("data/Steam.db.pat2");

//     file2 << patricia << std::endl;
// }

// int main()
// {
    // std::cout << database::convert("C:/Users/kersz/Documents/ufrgs/Semestre_3/CPDFinal/code/DB_Steam.csv", -1) << std::endl;

    // database::append("C:/Users/kersz/Documents/ufrgs/Semestre_3/CPDFinal/code/NewGames.csv", 0, "", "", "", "", "", "", "", "", "", "");
    // database::append("", 2230903, "IANSSTESTGAME", "TEST DEV", "TEST PUB", "2027", "TEST TAG", "TEST GAMEDET", "English", "Action", "Free", "Mostly Positive");

    // database::append("C:/Users/kersz/Documents/ufrgs/Semestre_3/CPDFinal/code/NewGames.csv", 0, "", "", "", "", "", "", "", "", "", "");

    // auto ret = database::search({}, {}, {}, {}, {}, {}, {"Overwhelmingly Positive"}, "", -1, -1, -1, -1, false);
    // for(auto item : ret)
    // {
    //     for(auto item2 : item)
    //     {
    //         std::cout << item2 << " ; ";
    //     }
    //     std::cout << std::endl << std::endl;
    // }

    // return 0;
// }