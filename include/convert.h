#ifndef __CONVERT_H
#define __CONVERT_H

union int_to_vals {
  unsigned int ival;
  int iival;
  unsigned short sval[2];
  short isval[2];
  unsigned char cval[4];
};

#endif
