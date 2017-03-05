#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../tests/time_sample.h"
#include "../buffer.h"
#include "../rmalloc.h"

size_t qint_encode(char *ptr, uint32_t arr[], int len) {
  if (len <= 0 || len > 4) return 0;
  size_t ret = 1;
  char *leading = ptr;
  *leading = 0;
  ptr++;

  for (int i = 0; i < len; i++) {
    int n = 0;
    while (arr[i] && n < 4) {
      *ptr++ = (char)(arr[i] & 0xff);
      n++;
      ret++;
      arr[i] = arr[i] >> 8;
    }
    *leading |= (((n-1) & 0x03) << i * 2);
  }

  return ret;
}

size_t _qint_encode(char *leading, BufferWriter *bw, uint32_t i, int offset) {
  size_t ret = 0;
  int n = 0;
  while (i && n < 4) {
    ret += Buffer_Write(bw, (unsigned char *)(i & 0xff), 1);
    n++;
    i >>= 8;
  }
  *leading |= ((n-1) & 0x03)<<(offset*2);
  return ret;
}

size_t qint_encode1(BufferWriter *bw, uint32_t i);
size_t qint_encode2(BufferWriter *bw, uint32_t i1, uint32_t i2);
size_t qint_encode3(BufferWriter *bw, uint32_t i1, uint32_t i2, uint32_t i3);
size_t qint_encode4(BufferWriter *bw, uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4);


size_t qint_encode1(BufferWriter *bw, uint32_t i) {
  size_t ret = 1;
  char *leading = bw->pos;
  Buffer_Write(bw, "\0", 1);
  ret += _qint_encode(leading, bw, i, 0);
  return ret;
}

size_t qint_encode2(BufferWriter *bw, uint32_t i1, uint32_t i2) {
  size_t ret = 1;
  char *leading = bw->pos;
  Buffer_Write(bw, "\0", 1);
  ret += _qint_encode(leading, bw, i1, 0);
  ret += _qint_encode(leading, bw, i2, 1);
  return ret;
}

size_t qint_encode3(BufferWriter *bw, uint32_t i1, uint32_t i2, uint32_t i3) {
  size_t ret = 1;
  char *leading = bw->pos;
  Buffer_Write(bw, "\0", 1);
  ret += _qint_encode(leading, bw, i1, 0);
  ret += _qint_encode(leading, bw, i2, 1);
  ret += _qint_encode(leading, bw, i3, 2);
  return ret;
}

size_t qint_encode4(BufferWriter *bw, uint32_t i1, uint32_t i2, uint32_t i3, uint32_t i4) {
  size_t ret = 1;
  char *leading = bw->pos;
  Buffer_Write(bw, "\0", 1);
  ret += _qint_encode(leading, bw, i1, 0);
  ret += _qint_encode(leading, bw, i2, 1);
  ret += _qint_encode(leading, bw, i3, 2);
  ret += _qint_encode(leading, bw, i3, 3);
  return ret;
}


typedef struct  {
  int offset;
  uint32_t mask;
}qintField;

typedef struct {
  size_t size;
  qintField fields[4];
} qintConfig;


qintConfig configs[256] = {
{.fields = {{1, 0xff},{2, 0xff},{3, 0xff},{4, 0xff},}, .size = 5 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xff},{5, 0xff},}, .size = 6 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xff},{6, 0xff},}, .size = 7 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xff},{7, 0xff},}, .size = 8 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xff},{5, 0xff},}, .size = 6 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xff},{6, 0xff},}, .size = 7 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xff},{7, 0xff},}, .size = 8 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xff},{8, 0xff},}, .size = 9 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xff},{6, 0xff},}, .size = 7 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xff},{7, 0xff},}, .size = 8 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xff},{8, 0xff},}, .size = 9 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xff},{9, 0xff},}, .size = 10 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xff},{7, 0xff},}, .size = 8 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xff},{8, 0xff},}, .size = 9 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xff},{9, 0xff},}, .size = 10 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xff},{10, 0xff},}, .size = 11 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xffff},{5, 0xff},}, .size = 6 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xffff},{6, 0xff},}, .size = 7 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xffff},{7, 0xff},}, .size = 8 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xffff},{8, 0xff},}, .size = 9 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xffff},{6, 0xff},}, .size = 7 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xffff},{7, 0xff},}, .size = 8 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xffff},{8, 0xff},}, .size = 9 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xffff},{9, 0xff},}, .size = 10 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xffff},{7, 0xff},}, .size = 8 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xffff},{8, 0xff},}, .size = 9 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xffff},{9, 0xff},}, .size = 10 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xffff},{10, 0xff},}, .size = 11 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xffff},{8, 0xff},}, .size = 9 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xffff},{9, 0xff},}, .size = 10 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xffff},{10, 0xff},}, .size = 11 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xffff},{11, 0xff},}, .size = 12 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xffffff},{6, 0xff},}, .size = 7 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xffffff},{7, 0xff},}, .size = 8 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xffffff},{8, 0xff},}, .size = 9 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xffffff},{9, 0xff},}, .size = 10 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xffffff},{7, 0xff},}, .size = 8 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xffffff},{8, 0xff},}, .size = 9 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xffffff},{9, 0xff},}, .size = 10 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xffffff},{10, 0xff},}, .size = 11 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xffffff},{8, 0xff},}, .size = 9 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xffffff},{9, 0xff},}, .size = 10 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xffffff},{10, 0xff},}, .size = 11 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xffffff},{11, 0xff},}, .size = 12 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xffffff},{9, 0xff},}, .size = 10 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xffffff},{10, 0xff},}, .size = 11 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xffffff},{11, 0xff},}, .size = 12 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xffffff},{12, 0xff},}, .size = 13 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xffffffff},{7, 0xff},}, .size = 8 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xffffffff},{8, 0xff},}, .size = 9 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xffffffff},{9, 0xff},}, .size = 10 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xffffffff},{10, 0xff},}, .size = 11 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xffffffff},{8, 0xff},}, .size = 9 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xffffffff},{9, 0xff},}, .size = 10 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xffffffff},{10, 0xff},}, .size = 11 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xffffffff},{11, 0xff},}, .size = 12 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xffffffff},{9, 0xff},}, .size = 10 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xffffffff},{10, 0xff},}, .size = 11 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xffffffff},{11, 0xff},}, .size = 12 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xffffffff},{12, 0xff},}, .size = 13 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xffffffff},{10, 0xff},}, .size = 11 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xffffffff},{11, 0xff},}, .size = 12 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xffffffff},{12, 0xff},}, .size = 13 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xffffffff},{13, 0xff},}, .size = 14 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xff},{4, 0xffff},}, .size = 6 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xff},{5, 0xffff},}, .size = 7 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xff},{6, 0xffff},}, .size = 8 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xff},{7, 0xffff},}, .size = 9 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xff},{5, 0xffff},}, .size = 7 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xff},{6, 0xffff},}, .size = 8 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xff},{7, 0xffff},}, .size = 9 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xff},{8, 0xffff},}, .size = 10 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xff},{6, 0xffff},}, .size = 8 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xff},{7, 0xffff},}, .size = 9 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xff},{8, 0xffff},}, .size = 10 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xff},{9, 0xffff},}, .size = 11 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xff},{7, 0xffff},}, .size = 9 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xff},{8, 0xffff},}, .size = 10 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xff},{9, 0xffff},}, .size = 11 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xff},{10, 0xffff},}, .size = 12 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xffff},{5, 0xffff},}, .size = 7 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xffff},{6, 0xffff},}, .size = 8 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xffff},{7, 0xffff},}, .size = 9 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xffff},{8, 0xffff},}, .size = 10 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xffff},{6, 0xffff},}, .size = 8 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xffff},{7, 0xffff},}, .size = 9 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xffff},{8, 0xffff},}, .size = 10 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xffff},{9, 0xffff},}, .size = 11 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xffff},{7, 0xffff},}, .size = 9 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xffff},{8, 0xffff},}, .size = 10 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xffff},{9, 0xffff},}, .size = 11 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xffff},{10, 0xffff},}, .size = 12 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xffff},{8, 0xffff},}, .size = 10 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xffff},{9, 0xffff},}, .size = 11 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xffff},{10, 0xffff},}, .size = 12 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xffff},{11, 0xffff},}, .size = 13 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xffffff},{6, 0xffff},}, .size = 8 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xffffff},{7, 0xffff},}, .size = 9 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xffffff},{8, 0xffff},}, .size = 10 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xffffff},{9, 0xffff},}, .size = 11 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xffffff},{7, 0xffff},}, .size = 9 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xffffff},{8, 0xffff},}, .size = 10 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xffffff},{9, 0xffff},}, .size = 11 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xffffff},{10, 0xffff},}, .size = 12 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xffffff},{8, 0xffff},}, .size = 10 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xffffff},{9, 0xffff},}, .size = 11 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xffffff},{10, 0xffff},}, .size = 12 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xffffff},{11, 0xffff},}, .size = 13 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xffffff},{9, 0xffff},}, .size = 11 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xffffff},{10, 0xffff},}, .size = 12 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xffffff},{11, 0xffff},}, .size = 13 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xffffff},{12, 0xffff},}, .size = 14 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xffffffff},{7, 0xffff},}, .size = 9 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xffffffff},{8, 0xffff},}, .size = 10 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xffffffff},{9, 0xffff},}, .size = 11 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xffffffff},{10, 0xffff},}, .size = 12 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xffffffff},{8, 0xffff},}, .size = 10 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xffffffff},{9, 0xffff},}, .size = 11 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xffffffff},{10, 0xffff},}, .size = 12 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xffffffff},{11, 0xffff},}, .size = 13 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xffffffff},{9, 0xffff},}, .size = 11 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xffffffff},{10, 0xffff},}, .size = 12 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xffffffff},{11, 0xffff},}, .size = 13 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xffffffff},{12, 0xffff},}, .size = 14 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xffffffff},{10, 0xffff},}, .size = 12 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xffffffff},{11, 0xffff},}, .size = 13 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xffffffff},{12, 0xffff},}, .size = 14 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xffffffff},{13, 0xffff},}, .size = 15 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xff},{4, 0xffffff},}, .size = 7 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xff},{5, 0xffffff},}, .size = 8 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xff},{6, 0xffffff},}, .size = 9 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xff},{7, 0xffffff},}, .size = 10 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xff},{5, 0xffffff},}, .size = 8 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xff},{6, 0xffffff},}, .size = 9 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xff},{7, 0xffffff},}, .size = 10 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xff},{8, 0xffffff},}, .size = 11 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xff},{6, 0xffffff},}, .size = 9 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xff},{7, 0xffffff},}, .size = 10 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xff},{8, 0xffffff},}, .size = 11 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xff},{9, 0xffffff},}, .size = 12 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xff},{7, 0xffffff},}, .size = 10 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xff},{8, 0xffffff},}, .size = 11 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xff},{9, 0xffffff},}, .size = 12 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xff},{10, 0xffffff},}, .size = 13 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xffff},{5, 0xffffff},}, .size = 8 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xffff},{6, 0xffffff},}, .size = 9 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xffff},{7, 0xffffff},}, .size = 10 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xffff},{8, 0xffffff},}, .size = 11 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xffff},{6, 0xffffff},}, .size = 9 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xffff},{7, 0xffffff},}, .size = 10 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xffff},{8, 0xffffff},}, .size = 11 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xffff},{9, 0xffffff},}, .size = 12 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xffff},{7, 0xffffff},}, .size = 10 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xffff},{8, 0xffffff},}, .size = 11 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xffff},{9, 0xffffff},}, .size = 12 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xffff},{10, 0xffffff},}, .size = 13 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xffff},{8, 0xffffff},}, .size = 11 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xffff},{9, 0xffffff},}, .size = 12 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xffff},{10, 0xffffff},}, .size = 13 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xffff},{11, 0xffffff},}, .size = 14 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xffffff},{6, 0xffffff},}, .size = 9 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xffffff},{7, 0xffffff},}, .size = 10 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xffffff},{8, 0xffffff},}, .size = 11 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xffffff},{9, 0xffffff},}, .size = 12 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xffffff},{7, 0xffffff},}, .size = 10 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xffffff},{8, 0xffffff},}, .size = 11 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xffffff},{9, 0xffffff},}, .size = 12 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xffffff},{10, 0xffffff},}, .size = 13 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xffffff},{8, 0xffffff},}, .size = 11 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xffffff},{9, 0xffffff},}, .size = 12 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xffffff},{10, 0xffffff},}, .size = 13 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xffffff},{11, 0xffffff},}, .size = 14 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xffffff},{9, 0xffffff},}, .size = 12 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xffffff},{10, 0xffffff},}, .size = 13 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xffffff},{11, 0xffffff},}, .size = 14 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xffffff},{12, 0xffffff},}, .size = 15 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xffffffff},{7, 0xffffff},}, .size = 10 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xffffffff},{8, 0xffffff},}, .size = 11 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xffffffff},{9, 0xffffff},}, .size = 12 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xffffffff},{10, 0xffffff},}, .size = 13 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xffffffff},{8, 0xffffff},}, .size = 11 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xffffffff},{9, 0xffffff},}, .size = 12 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xffffffff},{10, 0xffffff},}, .size = 13 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xffffffff},{11, 0xffffff},}, .size = 14 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xffffffff},{9, 0xffffff},}, .size = 12 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xffffffff},{10, 0xffffff},}, .size = 13 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xffffffff},{11, 0xffffff},}, .size = 14 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xffffffff},{12, 0xffffff},}, .size = 15 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xffffffff},{10, 0xffffff},}, .size = 13 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xffffffff},{11, 0xffffff},}, .size = 14 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xffffffff},{12, 0xffffff},}, .size = 15 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xffffffff},{13, 0xffffff},}, .size = 16 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xff},{4, 0xffffffff},}, .size = 8 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xff},{5, 0xffffffff},}, .size = 9 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xff},{6, 0xffffffff},}, .size = 10 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xff},{7, 0xffffffff},}, .size = 11 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xff},{5, 0xffffffff},}, .size = 9 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xff},{6, 0xffffffff},}, .size = 10 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xff},{7, 0xffffffff},}, .size = 11 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xff},{8, 0xffffffff},}, .size = 12 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xff},{6, 0xffffffff},}, .size = 10 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xff},{7, 0xffffffff},}, .size = 11 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xff},{8, 0xffffffff},}, .size = 12 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xff},{9, 0xffffffff},}, .size = 13 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xff},{7, 0xffffffff},}, .size = 11 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xff},{8, 0xffffffff},}, .size = 12 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xff},{9, 0xffffffff},}, .size = 13 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xff},{10, 0xffffffff},}, .size = 14 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xffff},{5, 0xffffffff},}, .size = 9 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xffff},{6, 0xffffffff},}, .size = 10 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xffff},{7, 0xffffffff},}, .size = 11 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xffff},{8, 0xffffffff},}, .size = 12 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xffff},{6, 0xffffffff},}, .size = 10 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xffff},{7, 0xffffffff},}, .size = 11 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xffff},{8, 0xffffffff},}, .size = 12 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xffff},{9, 0xffffffff},}, .size = 13 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xffff},{7, 0xffffffff},}, .size = 11 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xffff},{8, 0xffffffff},}, .size = 12 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xffff},{9, 0xffffffff},}, .size = 13 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xffff},{10, 0xffffffff},}, .size = 14 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xffff},{8, 0xffffffff},}, .size = 12 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xffff},{9, 0xffffffff},}, .size = 13 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xffff},{10, 0xffffffff},}, .size = 14 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xffff},{11, 0xffffffff},}, .size = 15 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xffffff},{6, 0xffffffff},}, .size = 10 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xffffff},{7, 0xffffffff},}, .size = 11 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xffffff},{8, 0xffffffff},}, .size = 12 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xffffff},{9, 0xffffffff},}, .size = 13 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xffffff},{7, 0xffffffff},}, .size = 11 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xffffff},{8, 0xffffffff},}, .size = 12 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xffffff},{9, 0xffffffff},}, .size = 13 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xffffff},{10, 0xffffffff},}, .size = 14 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xffffff},{8, 0xffffffff},}, .size = 12 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xffffff},{9, 0xffffffff},}, .size = 13 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xffffff},{10, 0xffffffff},}, .size = 14 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xffffff},{11, 0xffffffff},}, .size = 15 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xffffff},{9, 0xffffffff},}, .size = 13 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xffffff},{10, 0xffffffff},}, .size = 14 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xffffff},{11, 0xffffffff},}, .size = 15 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xffffff},{12, 0xffffffff},}, .size = 16 },
{.fields = {{1, 0xff},{2, 0xff},{3, 0xffffffff},{7, 0xffffffff},}, .size = 11 },
{.fields = {{1, 0xffff},{3, 0xff},{4, 0xffffffff},{8, 0xffffffff},}, .size = 12 },
{.fields = {{1, 0xffffff},{4, 0xff},{5, 0xffffffff},{9, 0xffffffff},}, .size = 13 },
{.fields = {{1, 0xffffffff},{5, 0xff},{6, 0xffffffff},{10, 0xffffffff},}, .size = 14 },
{.fields = {{1, 0xff},{2, 0xffff},{4, 0xffffffff},{8, 0xffffffff},}, .size = 12 },
{.fields = {{1, 0xffff},{3, 0xffff},{5, 0xffffffff},{9, 0xffffffff},}, .size = 13 },
{.fields = {{1, 0xffffff},{4, 0xffff},{6, 0xffffffff},{10, 0xffffffff},}, .size = 14 },
{.fields = {{1, 0xffffffff},{5, 0xffff},{7, 0xffffffff},{11, 0xffffffff},}, .size = 15 },
{.fields = {{1, 0xff},{2, 0xffffff},{5, 0xffffffff},{9, 0xffffffff},}, .size = 13 },
{.fields = {{1, 0xffff},{3, 0xffffff},{6, 0xffffffff},{10, 0xffffffff},}, .size = 14 },
{.fields = {{1, 0xffffff},{4, 0xffffff},{7, 0xffffffff},{11, 0xffffffff},}, .size = 15 },
{.fields = {{1, 0xffffffff},{5, 0xffffff},{8, 0xffffffff},{12, 0xffffffff},}, .size = 16 },
{.fields = {{1, 0xff},{2, 0xffffffff},{6, 0xffffffff},{10, 0xffffffff},}, .size = 14 },
{.fields = {{1, 0xffff},{3, 0xffffffff},{7, 0xffffffff},{11, 0xffffffff},}, .size = 15 },
{.fields = {{1, 0xffffff},{4, 0xffffffff},{8, 0xffffffff},{12, 0xffffffff},}, .size = 16 },
{.fields = {{1, 0xffffffff},{5, 0xffffffff},{9, 0xffffffff},{13, 0xffffffff},}, .size = 17 },
};

typedef unsigned char *qint;
uint32_t qint_decode(qint p, int offset, uint32_t mask) {
  return  *(uint32_t*)(p+offset) & mask;
}

#define qint_member(p, i) (*(uint32_t*)(p+configs[*p].fields[i].offset) & configs[*p].fields[i].mask)

void printConfig(unsigned char c) {

  int off = 1;
  int masks[4] = {0xff, 0xffff, 0xffffff, 0xffffffff};

  printf("{.fields = {");
  
  
  for (int i = 0; i < 8; i+=2) {
    printf("{%d, 0x%x},", off, masks[(c >> i)&0x03]);
    off += ((c>>i)&0x03) + 1;
  }
  
  printf("}, .size = %d }, \n", off);
}
int main(int argc, char **argv){
  
  RM_PatchAlloc();
  
  // for (uint16_t i = 0; i < 256; i++) {
  //   printConfig(i);
  // }
  // return 0;
  Buffer *b = NewBuffer(1024);
  BufferWriter bw = NewBufferWriter(b);
  size_t sz = 0;
  //uint32_t arr[4] = {1000, 100, 300,4 };
  int x = 0;
  while (x < 1000) {
    sz += qint_encode4(&bw, 1000, 100, 300, 4);
    x++;
  }

  printf("sz: %zd, x: %d\n",sz,  x);
  
  unsigned char *buf = b->data;
  qintConfig config = configs[*(uint8_t*)buf];
  uint64_t total;
  TimeSample ts;
  
  TimeSampler_Start(&ts);
    for (int i = 0; i < 100; i++) {
      for (int n = 0 ; n < 4; n++) {
        printf("%d\n", qint_member(buf, n));

      }
      TimeSampler_Tick(&ts);
      buf+=configs[*buf].size;
    }
  TimeSampler_End(&ts);
  printf("Total: %zd, %fns/iter", TimeSampler_DurationNS(&ts), (double)TimeSampler_DurationNS(&ts)/(double)ts.num);
  printf("%d\n", total);
  //   //printf("%d %x\n",config.fields[i].offset, config.fields[i].mask);
  //   printf("%d\n", );
  // }
  printf("\n%d\n", qint_decode(buf, 1, 0xffff));
  return 0;
}