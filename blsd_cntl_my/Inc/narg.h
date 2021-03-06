#ifndef NARG_H
#define NARG_H

#define NARGS(...) NARG_IMPL(__VA_ARGS__, \
  32,31,30,29,28,27,26,25, \
  24,23,22,21,20,19,18,17, \
  16,15,14,13,12,11,10, 9, \
  8, 7, 6, 5, 4, 3, 2, 1)
#define NARG_IMPL(...) ARG_CNT_(__VA_ARGS__)
#define ARG_CNT_( \
  _32,_31,_30,_29,_28,_27,_26,_25, \
  _24,_23,_22,_21,_20,_19,_18,_17, \
  _16,_15,_14,_13,_12,_11,_10,_9, \
  _8, _7, _6, _5, _4, _3, _2, _1, \
   N, ...) N
/*
#define RARGS \
  32,31,30,29,28,27,26,25, \
  24,23,22,21,20,19,18,17, \
  16,15,14,13,12,11,10, 9, \
  8, 7, 6, 5, 4, 3, 2, 1
*/

#endif /*NARG_H*/
