#include "classes/menu.hpp"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        return MENU::HelpMessage();
    }

    std::vector<std::string> args;

    for (int i = 1; i < argc; i++)
        args.emplace_back(argv[i]);

    if (args[0][1] == 'h')
    {
        return MENU::Help(args);
    }
    else if (args[0][1] == 'c')
    {
        return MENU::Convert(args);
    }

    if(!IO::databaseExists())
    {
        std::cout << "No database found. Please create one using the -c flag." << std::endl;
        return -1;
    }

    switch (args[0][1])
    {
    case 'g':
    {
        return MENU::Get(args);
    }
    case 's':
    {
        return MENU::Search(args);
    }
    case 'a':
    {
        return MENU::Append(args);
    }
    case 'l':
    {
        return MENU::Load(args);
    }
    default:
    {
        return MENU::HelpMessage();
    }
    }
}