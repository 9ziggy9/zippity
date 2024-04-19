#include "ui.h"
#include <stdlib.h>
#include <string.h>

static char line[MAX_LINE_LENGTH];
static int SCREEN_COLS, SCREEN_ROWS;
static int MASTER_COLS, MASTER_ROWS;

void ui_init_scr(void) {
  initscr();
  if (stdscr == NULL) PANIC(EXIT_ALLOC_FAILED);
  noecho();
  curs_set(FALSE);
  SCREEN_COLS = getmaxx(stdscr);
  SCREEN_ROWS = getmaxy(stdscr);
}

void ui_init_master(WINDOW **w) {
  MASTER_COLS = SCREEN_COLS - 2 * BORDER_WIDTH;
  MASTER_ROWS = SCREEN_ROWS - 2 * BORDER_WIDTH;
  *w = newwin(MASTER_ROWS, MASTER_COLS, 1, 1);
  if (*w == NULL) PANIC(EXIT_ALLOC_FAILED);
  scrollok(*w, TRUE);
  box(*w, 0, 0);
}

#define FOUND_BOTTOM(r) ((r) >= MASTER_ROWS - BORDER_WIDTH)
int ui_read_in_lines(WINDOW *w, FILE *fp) {
  int row = 1;
  while (fgets(line, sizeof(line), fp)) {
    line[strcspn(line, "\n")] = 0;
    mvwprintw(w, row, 2, "%s", line);
    if (row % 2) ui_hl_line(w, row);
    row++;
    if (FOUND_BOTTOM(row)) return -1;
  }
  return row;
}

void ui_hl_line(WINDOW *w, int row) {
  wattron(w, A_REVERSE);
  mvwprintw(w, row, 2, "%*s", MASTER_COLS - 4, "");
  wattroff(w, A_REVERSE);
}

void ui_clear_master(WINDOW *w) { wclear(w); box(w, 0, 0); }
