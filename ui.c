#include "ui.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

static Line lines[MAX_LINES];
int SCREEN_COLS, SCREEN_ROWS;
int MASTER_COLS, MASTER_ROWS;

void ui_exit_handler(int code, void *args) {
  struct hitlist *hl = (struct hitlist *) args;

  if (hl->wm != NULL) delwin(hl->wm);
  if (hl->fp != NULL) fclose(hl->fp);
  endwin();

  printf("EXIT: %d\n", code);
}

static void ui_init_clr(void) {
  start_color();
  init_pair(COLORS_D,   COLOR_WHITE, COLOR_BLACK);
  init_pair(COLORS_HL,  COLOR_BLACK, COLOR_WHITE);
  init_pair(COLORS_SEL, COLOR_BLACK, COLOR_GREEN);
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

int ui_read_page(WINDOW *w, FILE *fp) {
  static long prev_page_start = 0;
  int row = 0; // relative to screen view
  char line_txt[MAX_LINE_LENGTH];

  if (ftell(fp) < prev_page_start) fseek(fp, prev_page_start, SEEK_SET);

  while ((row + 1) < MASTER_ROWS - BORDER_WIDTH) {
    fgets(line_txt, sizeof(line_txt), fp);
    if (feof(fp)) {
      rewind(fp);
      prev_page_start = ftell(fp);
      return row;
    }
    line_txt[strcspn(line_txt, "\n")] = '\0';
    lines[row] = (Line) { .txt = line_txt, .row = row, .selected = false, };
    row++;
    mvwprintw(w, row, 2 * BORDER_WIDTH, "%s", line_txt); 
  }
  prev_page_start = ftell(fp);
  return row;
}

void ui_clear_master(WINDOW *w) { wclear(w); box(w, 0, 0); }

void ui_toggle_sel(int line_no) {
  lines[line_no].selected = !(lines[line_no].selected);
}

#define UI_EOL (MASTER_COLS - 2 * BORDER_WIDTH)
void ui_master_update(WINDOW *w, int line_no) {
  static int last = -1;

  if (last >= 0) mvwchgat(w, last, 1, UI_EOL, A_NORMAL, COLORS_D, NULL);
  last = line_no;
  mvwchgat(w, line_no, 1, UI_EOL, A_BOLD, COLORS_HL, NULL);

  for (int l = 0; l < MAX_LINES; l++) {
    if (lines[l].selected) mvwchgat(w, l, 1, UI_EOL, A_BOLD, COLORS_SEL, NULL);
  }

  wrefresh(w);
}
