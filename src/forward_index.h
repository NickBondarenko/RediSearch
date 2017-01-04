#ifndef __FORWARD_INDEX_H__
#define __FORWARD_INDEX_H__
#include "document.h"
#include "tokenize.h"
#include "types.h"
#include "util/khash.h"
#include "varint.h"

typedef struct {
  const char *term;
  size_t len;
  t_docId docId;
  float freq;
  float docScore;
  u_char flags;
  VarintVectorWriter *vw;
  int stringFreeable;
} ForwardIndexEntry;

KHASH_MAP_INIT_INT(32, ForwardIndexEntry *)

// the quantizationn factor used to encode normalized (0..1) frquencies in the index
#define FREQ_QUANTIZE_FACTOR 0xFFFF

typedef struct {
  khash_t(32) * hits;

  t_docId docId;
  float totalFreq;
  float maxFreq;
  float docScore;
  int uniqueTokens;
  Stemmer *stemmer;
} ForwardIndex;

typedef struct {
  ForwardIndex *idx;
  khiter_t k;
} ForwardIndexIterator;

int forwardIndexTokenFunc(void *ctx, Token t);

void ForwardIndexFree(ForwardIndex *idx);
ForwardIndex *NewForwardIndex(Document doc);
ForwardIndexIterator ForwardIndex_Iterate(ForwardIndex *i);
ForwardIndexEntry *ForwardIndexIterator_Next(ForwardIndexIterator *iter);
void ForwardIndex_NormalizeFreq(ForwardIndex *, ForwardIndexEntry *);

#endif