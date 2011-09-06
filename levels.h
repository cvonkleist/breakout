char *bricks_1[] = {
  "  RR  RR  ",
  "  GG  GG  ",
  "  BB  BB  ",
};
char *bricks_2[] = {
  " RRG  GRR ",
  " RGG  GGR ",
  " BBBBBBBB ",
};
char *bricks_3[] = {
  " RMCCCCMR ",
  "  RMCCMR  ",
  "   RMMR   ",
  "    RR    ",
  "YY      YY",
};
char *bricks_4[] = {
  "RRR YY BBB",
  "RRR YY BBB",
  "RRR YY BBB",
  "    YY    ",
  "    YY    ",
};
char *bricks_5[] = {
  " RRR  R  R", // cvk
  "R     R  R",
  "R R R R R ",
  "R R R RR  ",
  "R  R  R R ",
  "R     R  R",
  " RRR  R  R",
  "CCCCCCCCCC",
};
char **bricks[] = {bricks_1, bricks_2, bricks_3, bricks_4, bricks_5};
uint8_t line_count[] = {3, 3, 5, 5, 8};
#define LEVEL_COUNT 5
