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
//     // std::cout << database::convert("../DB_Steam.csv", -1) << std::endl;
//     // auto begin = std::chrono::high_resolution_clock::now();
//     test();
//     // auto end = std::chrono::high_resolution_clock::now();
//     // std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "ms" << std::endl;

//     auto games = database::search({}, {}, {}, {}, {}, {}, {}, "0", -1, -1, -1, -1, 0);

//     for (auto game : games)
//     {
//         for (auto item : game)
//         {
//             std::cout << item << " ";
//         }
//         std::cout << std::endl;
//     }

//     return 0;
// }