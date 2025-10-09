#include <ncurses.h>
#include <iostream>
#include <filesystem>
#include <vector>

struct FileSystemItem
{
    std::filesystem::__cxx11::directory_entry entry;
    std::string path;
};

void get_directory_contents(std::vector<FileSystemItem> &options, FileSystemItem item, int &maxOptionLength, int &selected)
{

    if (options.size() > 0)
    {
        options.clear();
    }

    if (std::filesystem::exists(item.entry.path()) && std::filesystem::is_directory(item.entry.path()))
    {
        for (const auto &entry : std::filesystem::directory_iterator(item.entry.path()))
        {
            options.push_back({entry, entry.path().filename().string()});
            if (entry.path().filename().string().length() > maxOptionLength)
            {
                maxOptionLength = entry.path().filename().string().length();
            }
        }
    }
}

void displayDirectories(std::vector<FileSystemItem> &options, int &index)
{
    mvprintw(index, 0, "/..");
    for (const auto &option : options)
    {
        if (std::filesystem::is_directory(option.entry.path()))
        {
            mvprintw(index + 1, 0, "/%s", option.path.c_str());
        }
        else
        {
            mvprintw(index + 1, 0, "%s", option.path.c_str());
        }

        index++;
    }
}

int main()
{
    int maxOptionLength = 0;

    FileSystemItem defaultItem{std::filesystem::directory_entry(std::filesystem::current_path()), std::filesystem::current_path()};

    std::vector<FileSystemItem> options{};

    // //Set up nCurses screen
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE); // Lets us hijack the arrow keys and keypad

    // //Open interactive Menu
    int input = 0;    // To store User Input
    int selected = 0; // To store the menu 'state'

    get_directory_contents(options, defaultItem, maxOptionLength, selected);

    while (input != 'q')
    {            // Add an input that quits
        clear(); // Try to figure out why this should go here

        int index{0};

        displayDirectories(options, index);

        if (selected >= 0 && selected < options.size() + 1)
        {
            mvchgat(selected, 0, maxOptionLength, A_STANDOUT, 1, NULL);
        }
        // Print frame buffer to screen
        refresh();

        // Get user input
        input = getch();
        // Change selected option; note:(0,0) is top left
        if (input == KEY_DOWN && selected < options.size())
        {
            selected++;
        }

        if (input == KEY_UP && selected > 0)
            selected--;

        // If ENTER, display option (keyboard enter is 10; numeric pad is KEY_ENTER)
        if (input == 10)
        {
            if(selected == 0)
            {
                FileSystemItem parentItem{std::filesystem::directory_entry(options[selected].entry.path().parent_path().parent_path()), options[selected].entry.path().parent_path().parent_path()};
                get_directory_contents(options, {parentItem.entry, parentItem.path}, maxOptionLength, selected);
            } else
            {
                get_directory_contents(options, {options[selected - 1].entry, options[selected - 1].path}, maxOptionLength, selected);
            }
           
            selected = 0;
        }
    }

    endwin();
}