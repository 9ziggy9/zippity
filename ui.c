#include "ui.h"
#include <stdlib.h>
#include <string.h>

static char line[MAX_LINE_LENGTH];
static int SCREEN_COLS, SCREEN_ROWS;
static int MASTER_COLS, MASTER_ROWS;

static void ui_init_clr(void) {
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_GREEN);
}

void ui_init_scr(void) {
  initscr();
  if (stdscr == NULL) PANIC(EXIT_ALLOC_FAILED);
  noecho();
  curs_set(FALSE);

  SCREEN_COLS = getmaxx(stdscr);
  SCREEN_ROWS = getmaxy(stdscr);

  ui_init_clr();
}

#define HL_ROW(W, ROW, FROM, FMT, LINE, PAIR)        \
  do {                                               \
    int TO = MASTER_COLS - (int) strlen(LINE)        \
           - 4 * BORDER_WIDTH;                       \
    wattron (W, COLOR_PAIR(PAIR));                   \
    mvwprintw(W, ROW, FROM, FMT"%*s", LINE, TO, ""); \
    wattroff(W, COLOR_PAIR(PAIR));                   \
  } while(0);                                        \

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
    if (row % 2) mvwprintw(w, row, 2, "%s", line);
    else         HL_ROW(w, row, 2, "%s", line, 1);
    row++;
    if (FOUND_BOTTOM(row)) return -1;
  }
  return row;
}

void ui_clear_master(WINDOW *w) { wclear(w); box(w, 0, 0); }
