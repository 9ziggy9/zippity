#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ui.h"

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
  int end_page = ui_read_page(w_master, fp);
  int line_no = 1;
begin_main_event: {
    ui_master_update(w_master, line_no);
    int key = wgetch(w_master);
    switch (key) {
    case KEY_ESC: exit(EXIT_SUCCESS);
    case 'm': ui_toggle_sel(line_no); break;
    case KEY_DOWN:
      if (line_no++ >= end_page) {
        ui_clear_master(w_master);
        end_page = ui_read_page(w_master, fp);
        line_no = 1;
      }
      break;
    case KEY_UP:
      if (line_no-- <= 1) {
        ui_clear_master(w_master);
        end_page = ui_read_page(w_master, fp);
        line_no = end_page;
      }
      break;
    }
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
