#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ui.h"


/* [part 1]
   Implement a small ncurses application in C which
   reads from the a text file `test.txt` and outputs
   that text to a window line by line.

   Be sure that the output window is embedded in
   stdscr and that it has a border. Please use
   macro definitions for any constants, there should
   be no magic numbers. */

#define FILE_NAME "test.txt"

int main() {
  WINDOW *w_master = NULL;
  ui_init_scr();
  ui_init_master(&w_master);

  FILE *fp = fopen(FILE_NAME, "r");
  if (fp == NULL) {
    mvwprintw(w_master, 1, 2, "Error opening file");
    wgetch(w_master);
    return 1;
  }

continue_reading_lines:
  if (ui_read_in_lines(w_master, fp) < 0) {
    wgetch(w_master);
    ui_clear_master(w_master);
    goto continue_reading_lines;
  }

  // wait for input
  wgetch(w_master);

  // Clean up
  fclose(fp);
  delwin(w_master);
  endwin();

  return EXIT_SUCCESS;
}
