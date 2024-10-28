#include <dirent.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lib/buttons.h"
#include "lib/colors.h"
#include "lib/device.h"
#include "lib/display.h"
#include "lib/fonts/fonts.h"
#include "lib/log.h"

#define VIEWER_FOLDER "viewer/"
#define MAX_ENTRIES 8
#define MAX_FILE_NAME 100
#define MAX_TEXT_SIZE 400

// Colors — Feel free to change these to fit your preference
#define BACKGROUND_COLOR WHITE
#define FONT_COLOR BLACK
#define SELECTED_BG_COLOR BYU_BLUE
#define SELECTED_FONT_COLOR BYU_LIGHT_SAND

// Makes sure to deinitialize everything before program close
void intHandler(int dummy) {
    log_info("Exiting...");
    display_exit();
    exit(0);
}

/*
 * Takes in a folder, reads the contents of the folder, filtering out any files that do not end with
 * .log or .bmp. This function should check to make sure the folder exists. It fills in the entries
 * array with all of the entries in the folder, up to 8 (MAX_ENTRIES), copying the strings from
 * d_name to the entries array. The function returns the number of entries it put into the entries
 * array.
 */
int get_entries(char *folder, char entries[][MAX_FILE_NAME]) { 
    DIR *dp;
    dp = opendir(folder);

    if(dp == NULL) {
        //Folder does not exist
        printf("Cannot find folder/n");
        exit(1);
    }

    struct dirent * entry;
    int files = 0;

    while ((entry = readdir(dp)) != NULL && (files < MAX_FILE_NAME)) {
        int len = strlen(entry->d_name);
        char file_type[3];
        file_type[0] = entry->d_name[len-3];
        file_type[1] = entry->d_name[len-2];
        file_type[2] = entry->d_name[len-1];

        if(strncmp(file_type,"bmp",3) == 0 || strncmp(file_type,"log", 3) == 0){
            strcpy(entries[files], entry->d_name);
            files++;
        }
    }

    
    closedir(dp);    
    
    return files; 
}

/*
 * Draws the menu of the screen. It uses the entries array to create the menu, with the num_entries
 * specifying how many entries are in the entries array. The selected parameter is the item in the
 * menu that is selected and should be highlighted. Use BACKGROUND_COLOR, FONT_COLOR,
 * SELECTED_BG_COLOR, and SELECTED_FONT_COLOR to help specify the colors of the background, font,
 * select bar color, and selected text color.
 */
void draw_menu(char entries[][MAX_FILE_NAME], int num_entries, int selected) {
   display_clear(BACKGROUND_COLOR);

   for(int i = 0; i < num_entries; i++) {
    char *file_name = entries[i];
    display_draw_string(8, i*16 + 4, file_name, &Font8, BACKGROUND_COLOR, FONT_COLOR);
    if(i == selected){
        display_draw_rectangle(0, i*16, 128, i*16+16, SELECTED_BG_COLOR, true, 1);
        display_draw_string(8, i*16 + 4,file_name, &Font8, SELECTED_BG_COLOR, SELECTED_FONT_COLOR);
    }
   }
   
}

/*
 * Displays an image or a log file. This function detects the type of file that should be draw. If
 * it is a bmp file, then it calls display_draw_image. If it is a log file, it opens the file, reads
 * 100 characters (MAX_TEXT_SIZE), and displays the text using display_draw_string. Combine folder
 * and file_name to get the complete file path.
 */
void draw_file(char *folder, char *file_name) {

}

int main(void) {

    signal(SIGINT, intHandler);

    log_info("Starting...");

    delay_ms(1000);


    // Use this to fill in with entries from the directory
    char entries[MAX_ENTRIES][MAX_FILE_NAME];

    // TODO: Initialize the hardware
    display_init();
    buttons_init();

    // TODO: Get directory contents using get_entries function
    int size = get_entries(VIEWER_FOLDER,entries);
    

    // TODO: Draw menu using draw_menu function
    int selected_entry = 0;
    draw_menu(entries,size,selected_entry);
    
    

    while (true) {
        delay_ms(200);

        // TODO: Put button logic here

        // Example of how to interact with a button
        if (button_up() == 0) {
            if(selected_entry != 0) {
                selected_entry--;
                draw_menu(entries,size,selected_entry);
            }

        }
        if (button_down() == 0) {
            if(selected_entry < MAX_ENTRIES-1) {
                selected_entry++;
                draw_menu(entries,size,selected_entry);
            }

        }
        if (button_center() == 0) {

        }
    }
    return 0;
}
