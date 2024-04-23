#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <panel.h>

#include "ui.h"
#include "pages.h"

#define FILE_NAME "test.txt"

int main(void) {
  WINDOW *w_master = NULL;
  FILE *fp = NULL;
  on_exit(ui_exit_handler, &(struct hitlist){ .wm = w_master, .fp = fp });

  ui_init_scr();
  ui_init_master(&w_master);

  fp = fopen(FILE_NAME, "r");
  if (fp == NULL) return EXIT_FAILURE;

  size_t MASTER_ROWS = ui_master_get_rows();
  size_t MASTER_COLS = ui_master_get_cols();
  Page *pg_curr = page_load(fp, 0, MASTER_ROWS, MASTER_COLS);

  ui_clear_master(w_master, pg_curr); // initial render

size_t line_no = 0;
_main_event:
{
  ui_master_update(w_master, pg_curr, line_no);

  int key = wgetch(w_master);
  switch (key) {
  case KEY_ESC: exit(EXIT_SUCCESS);
  case 'm': case '\n':
    ui_select_line(pg_curr, line_no); goto _main_event;
  case KEY_DOWN: case 'j':
    if (line_no + 1 < pg_curr->page_end) { line_no++; goto _main_event; }
    turn_page_next(fp, &pg_curr, MASTER_ROWS, MASTER_COLS);
    line_no = 0; break;
  case KEY_UP: case 'k':
    if (line_no > 0) { line_no--; goto _main_event; }
    if (pg_curr->page_no == 0) goto _main_event;
    turn_page_prev(&pg_curr);
    line_no = MASTER_ROWS - 1; break;
  case KEY_RIGHT: case 'l':
    turn_page_next(fp, &pg_curr, MASTER_ROWS, MASTER_COLS); break;
  case KEY_LEFT: case 'h':
    turn_page_prev(&pg_curr); break;
  }

  // FALLTHROUGH CODE GOES HERE
  ui_clear_master(w_master, pg_curr); // initial render
  goto _main_event;
}

  return EXIT_SUCCESS;
}
