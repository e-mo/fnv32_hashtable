#ifndef FNV32_HT_H
#define FNV32_HT_H
// A FNV-1a 32bit hash table for string keys and int vals.
// Uses separate chaining to handle collisions.

#include <stdint.h>

typedef struct fnv32_ht fnv32_ht;

fnv32_ht *fnv32_ht_new(uint32_t size);
int8_t fnv32_ht_ins(fnv32_ht *ht, char *key, uint32_t val);
int8_t fnv32_ht_del(fnv32_ht *ht, char *key);
int32_t fnv32_ht_get(fnv32_ht *ht, char *key);
void fnv32_ht_free(fnv32_ht *ht);

#endif
