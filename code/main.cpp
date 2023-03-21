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

    switch (args[0][1])
    {
    case 'h':
    {
        return MENU::Help(args);
    }
    case 'c':
    {
        return MENU::Convert(args);
    }
    case 'g':
    {
        return MENU::Get(args);
    }
    case 's':
    {
        return MENU::Set(args);
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