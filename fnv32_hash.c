// fnv32.c
// Simple implementation of FNV-1a hashing algorithm
// for hashing null-terminated strings with recommended
// 32 bit prime and offset basis.
//
// More info:
// http://www.isthe.com/chongo/tech/comp/fnv/

#include "fnv32_hash.h"

static const uint32_t FNV32_OFFSET_BASIS = 2166136261;
static const uint32_t FNV32_PRIME = 16777619;

uint32_t fnv32_hash_str(char *str) {
    unsigned char *c = (unsigned char *)str;
    uint32_t hash = FNV32_OFFSET_BASIS;
    while (*c) {
        hash *= FNV32_PRIME;
        hash ^= (uint32_t)*c++;
    }

    return hash;
}
