#include <ncurses.h>
#include <iostream>
#include <filesystem>
#include <vector>

struct FileSystemItem
{
    std::filesystem::directory_entry entry;
    std::string path;
};

class FileExplorer
{
public:
    FileExplorer()
    {
        initscr();
        cbreak();
        noecho();
        curs_set(0);
        scrollok(stdscr, TRUE);
        keypad(stdscr, TRUE);
    }

    void handleKeyboardInput()
    {
        switch (m_input)
        {
        case KEY_DOWN:
            handleSelectedItem(KEY_DOWN);
            break;
        case KEY_UP:
            handleSelectedItem(KEY_UP);
            break;
        case 10:
            if (selected > 0 && selected <= static_cast<int>(directoryContents.size()))
            {
                currentItem = directoryContents[selected - 1]; // adjust for /..
                handlePopulateContent();
            }
            else if (selected == 0)
            {
                auto parentPath = currentItem.entry.path().parent_path();
                if (!parentPath.empty())
                {
                    currentItem = {std::filesystem::directory_entry(parentPath), parentPath.string()};
                    handlePopulateContent();
                }
            }
            selected = 0;
            break;

        default:
            break;
        }
    }

    void handleSelectedItem(int key)
    {
        if (key == KEY_DOWN)
        {
            selected = (selected + 1) % (directoryContents.size() + 1);
        }
        else if (key == KEY_UP)
        {
            selected = (selected - 1 + directoryContents.size() + 1) % (directoryContents.size() + 1);
        }
    }

    void handlePopulateContent()
    {
        directoryContents.clear();
        maxOptionLength = 0;

        if (std::filesystem::exists(currentItem.entry.path()) && std::filesystem::is_directory(currentItem.entry.path()))
        {
            for (const auto &entry : std::filesystem::directory_iterator(currentItem.entry.path()))
            {
                std::string name = entry.path().filename().string();
                directoryContents.push_back({entry, name});
                maxOptionLength = std::max<int>(maxOptionLength, name.length());

                if ((std::filesystem::is_directory(entry.path())) || (name[0] == '.'))
                {
                    maxOptionLength++;
                }
            }
        }
    }

    void displayContent()
    {
        int index{0};
        mvprintw(index++, 0, "/..");
        for (const auto &option : directoryContents)
        {
            if (std::filesystem::is_directory(option.entry.path()))
            {
                mvprintw(index, 0, "/%s", option.path.c_str());
            }
            else
            {
                mvprintw(index, 0, "%s", option.path.c_str());
            }

            index++;
        }

        if (selected >= 0 && selected < directoryContents.size() + 1)
        {
            mvchgat(selected, 0, maxOptionLength, A_STANDOUT, 1, NULL);
        }
    }

    int getInput()
    {
        return m_input;
    }

    void setInput(int input)
    {
        m_input = input;
    }

    ~FileExplorer()
    {
        endwin();
    }

private:
    FileSystemItem currentItem{std::filesystem::directory_entry(std::filesystem::current_path()), std::filesystem::current_path()};
    int maxOptionLength{};
    int m_input{};
    int selected{};
    std::vector<FileSystemItem> directoryContents{};
};

int main()
{
    FileExplorer fileExplorer{};

    fileExplorer.handlePopulateContent();

    while (fileExplorer.getInput() != 'q')
    {
        clear();
        fileExplorer.displayContent();

        refresh();

        fileExplorer.setInput(getch());

        fileExplorer.handleKeyboardInput();
    }
}