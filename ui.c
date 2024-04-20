#include "ui.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

static Line lines[MAX_LINES];
static int SCREEN_COLS, SCREEN_ROWS;
static int MASTER_COLS, MASTER_ROWS;

void ui_exit_handler(int code, void *args) {
  struct hitlist *hl = (struct hitlist *) args;

  if (hl->wm != NULL) delwin(hl->wm);
  if (hl->fp != NULL) fclose(hl->fp);
  endwin();

  printf("EXIT: %d\n", code);
}

static void ui_init_clr(void) {
  start_color();
  init_pair(1, COLOR_BLACK, COLOR_WHITE);
  init_pair(2, COLOR_WHITE, COLOR_BLACK);
}

void ui_init_scr(void) {
  initscr();
  if (stdscr == NULL) PANIC(EXIT_ALLOC_FAILED);
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(FALSE);

  SCREEN_COLS = getmaxx(stdscr);
  SCREEN_ROWS = getmaxy(stdscr);

  ui_init_clr();
}

void ui_init_master(WINDOW **w) {
  MASTER_COLS = SCREEN_COLS - 2 * BORDER_WIDTH;
  MASTER_ROWS = SCREEN_ROWS - 2 * BORDER_WIDTH;
  *w = newwin(MASTER_ROWS, MASTER_COLS, 1, 1);
  if (*w == NULL) PANIC(EXIT_ALLOC_FAILED);
  scrollok(*w, TRUE);
  keypad(*w, TRUE);
  set_escdelay(0);
  box(*w, 0, 0);
}

#define FOUND_BOTTOM(r) ((r) >= MASTER_ROWS - BORDER_WIDTH)
int ui_read_in_lines(WINDOW *w, FILE *fp) {
  char line_txt[MAX_LINE_LENGTH];
  int row = 1;
  while (fgets(line_txt, sizeof(line_txt), fp)) {
    line_txt[strcspn(line_txt, "\n")] = '\0';
    lines[row-1] = (Line) { .txt = line_txt, .row = row };
    mvwprintw(w, row, 2 * BORDER_WIDTH, "%s", line_txt); 
    row++;
    if (FOUND_BOTTOM(row)) return -1;
  }
  return row;
}

void ui_clear_master(WINDOW *w) { wclear(w); box(w, 0, 0); }

Line ui_get_line(int row) { return lines[row - 1]; }

void _ui_hl_line(WINDOW *w, int row, char *fmt, char *ln, int pair) {
  int to = MASTER_COLS - (int) strlen(ln) - 4 * BORDER_WIDTH;
  wattrset(w, COLOR_PAIR(pair));
  mvwprintw(w, row, 2 * BORDER_WIDTH, fmt, ln, to, "");
}
