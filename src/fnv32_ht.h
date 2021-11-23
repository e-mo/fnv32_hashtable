// A FNV-1a 32bit hash table for string keys and dynamic (void *) vals.
// Uses separate chaining to handle collisions.
// Very fast and relizble if there are enough buckets
// to avoid excessive collisions and memory is handled properly.
//
// Documentation: https://github.com/e-mo/fnv32_hashtable/ 
//
// Maintainer: Evan Morse
// emorse8686@gmail.com
// github.com/e-mo

#ifndef FNV32_HT_H
#define FNV32_HT_H

typedef struct fnv32_ht fnv32_ht;

fnv32_ht *fnv32_ht_new(unsigned int size);
int fnv32_ht_ins(fnv32_ht *ht, char *key, void *val);
int fnv32_ht_del(fnv32_ht *ht, char *key);
void * fnv32_ht_rm(fnv32_ht *ht, char *key);
void * fnv32_ht_get(fnv32_ht *ht, char *key);
void fnv32_ht_free(fnv32_ht *ht);

#endif
