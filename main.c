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
  FILE *fp = NULL;
  on_exit(ui_exit_handler, &(struct hitlist){ .wm = w_master, .fp = fp });

  ui_init_scr();
  ui_init_master(&w_master);

  fp = fopen(FILE_NAME, "r");
  if (fp == NULL) {
    mvwprintw(w_master, 1, 2, "Error opening file");
    wgetch(w_master);
    return 1;
  }

  // Remember org notes on interesting read loop
  ui_read_in_lines(w_master, fp);

  int selected_line = 1;
begin_main_event: {
    int key = wgetch(w_master);

    switch (key) {
    /* case KEY_ESC: exit(EXIT_SUCCESS); */
    case KEY_DOWN: selected_line++; break;
    case KEY_UP:
      if (selected_line <= 0) break;
      selected_line--;
      break;
    }

    char *line_txt = ui_get_line(selected_line).txt;
    ui_hl_line(w_master, selected_line, "%s", line_txt, 1);

    goto begin_main_event;
}

// unreachable
cotton_eye_joe: {
  fprintf(stderr, "where did you come from?");
  getch();
  fprintf(stderr, "where did you go?");
  getch();
  goto cotton_eye_joe;
}
  return EXIT_SUCCESS;
}
