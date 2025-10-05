#include <ncurses.h>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>


void display_directory(std::vector<std::filesystem::__cxx11::directory_entry>& options, std::string path, int& count, int& maxOptionLength)
{   
    if(options.size() == 0)
    {
        options.clear();
    }
    
    // std::string path{std::filesystem::current_path()};

    if(std::filesystem::exists(path) && std::filesystem::is_directory(path))
    {
        for(const auto& entry : std::filesystem::directory_iterator(path))
        {
            options.push_back(entry);
            count++;
            if(entry.path().filename().string().length() > maxOptionLength)
            {
                maxOptionLength = entry.path().filename().string().length();
            }
        }
    }
}

int main(){
    // std::string path{std::filesystem::current_path()};
    std::string path{"/home/kris/Desktop/c++"};
	int count = 0;
	int maxOptionLength = 0;
	std::string menuOption;

    std::vector<std::filesystem::__cxx11::directory_entry> options;
    
    // if(std::filesystem::exists(path) && std::filesystem::is_directory(path))
    // {
    //     for(const auto& entry : std::filesystem::directory_iterator(path))
    //     {
    //         options.push_back(entry);
    //         count++;
    //         if(entry.path().filename().string().length() > maxOptionLength)
    //         {
    //             maxOptionLength = entry.path().filename().string().length();
    //         }
    //     }
    // }
    
	// //Set up nCurses screen
	initscr();
	cbreak();
	noecho();
	curs_set(0);
	keypad(stdscr, TRUE); //Lets us hijack the arrow keys and keypad

	// //Open interactive Menu
	int input = 0; //To store User Input
	int selected = 1; //To store the menu 'state'
	display_directory(options, path, count, maxOptionLength);
	
    while(input != 'q'){ //Add an input that quits
		clear(); //Try to figure out why this should go here
        
		//Write menu to frame buffer
		// mvprintw(1,12, "Main Menu");


		for(int i=0; i<count; i++){
			mvprintw(i+2, 15, "%s", options[i].path().filename().c_str());
		}
		if(selected > 0 && selected < (count+1))
        {
            mvchgat(selected+1, 15, maxOptionLength, A_STANDOUT, 1, NULL);
        }
			
		mvprintw(30, 100, "%i", input); //Debug: write input to frame buffer

		//Print frame buffer to screen
		refresh();

		//Get user input
		input = getch();
		//Change selected option; note:(0,0) is top left
		if(input == KEY_DOWN && selected < (count)) selected++;
		if(input == KEY_UP && selected > 1) selected--;
		
		//If ENTER, display option (keyboard enter is 10; numeric pad is KEY_ENTER)
		if(input == 10){
            display_directory(options, options[selected-1].path(), count, maxOptionLength);
            clear();
            for(int i=0; i<count; i++){
			    mvprintw(i+2, 15, "%s", options[i].path().filename().c_str());
		    }

            
            // if(std::filesystem::status(options[selected-1].path()).type() == std::filesystem::file_type::directory)
            // {
            //     options.clear();
            //     if(std::filesystem::exists(path) && std::filesystem::is_directory(path))
            //     {
            //         for(const auto& entry : std::filesystem::directory_iterator(path))
            //         {
            //             options.push_back(entry);
            //             count++;
            //             if(entry.path().filename().string().length() > maxOptionLength)
            //             {
            //                 maxOptionLength = entry.path().filename().string().length();
            //             }
            //         }
            //     }
            //     for(int i=0; i<count; i++)
            //     {
			//         mvprintw(i+2, 15, "%s", options[i].path().filename().c_str());
		    //     }
            //     // mvprintw(3, 20, "You have selected the directory: %s", options[selected-1].path().filename().c_str());
            // } else if(std::filesystem::status(options[selected-1].path()).type() == std::filesystem::file_type::regular)
            // {
            //     mvprintw(3, 20, "You have selected the file: %s", options[selected-1].path().filename().c_str());
            // }
			
			// mvprintw(5, 30, "Press any key to return...");
			// getch();
		}
	}

	endwin();
}