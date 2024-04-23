#ifndef PAGES_H_
#define PAGES_H_
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Line {
  char *txt;
  size_t abs_line_no;
  size_t line_max;
  bool selected;
} Line;

typedef struct Page {
  Line *lines;
  size_t page_no;
  size_t page_end;
  size_t lines_max;
  struct Page *prev;
  struct Page *next;
} Page;

Page *page_load(FILE *, size_t, size_t, size_t);
void turn_page_prev(Page **);
void turn_page_next(FILE *, Page **, size_t, size_t);

#endif // PAGES_H_
