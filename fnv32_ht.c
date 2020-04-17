// fnv32_ht.c
// A FNV-1a 32bit hash table for string keys and int vals.
// Uses separate chaining to handle collisions.

#include <stdlib.h>
#include <string.h>

#include "fnv32_hash.h"
#include "fnv32_ht.h"

typedef struct fnv32_ht_entry fnv32_ht_entry;
struct fnv32_ht_entry {
    char *key;
    int32_t val;
    fnv32_ht_entry *next;
};

struct fnv32_ht {
    uint32_t size;
    fnv32_ht_entry **table;
};

// STATIC 

static fnv32_ht_entry *ht_entry_new(char *, int32_t);
static void ht_entry_free(fnv32_ht_entry *);

static void ht_entry_chain_ins(fnv32_ht_entry *hte, fnv32_ht_entry *next);
static int8_t ht_entry_chain_del(fnv32_ht_entry *hte, char *key);
static int32_t ht_entry_chain_get(fnv32_ht_entry *hte, char *key);
static void ht_entry_chain_free(fnv32_ht_entry *hte);

static fnv32_ht_entry *ht_entry_new(char *key, int32_t val) {
    fnv32_ht_entry *new_entry;
    if ((new_entry = malloc(sizeof(fnv32_ht_entry))) == NULL) 
        return NULL;
    if ((new_entry->key = strdup(key)) == NULL)
        return NULL;

    new_entry->val = val;
    new_entry->next = NULL;
    return new_entry;
}

static void ht_entry_free(fnv32_ht_entry *ht_entry) {
    free(ht_entry->key);
    free(ht_entry);
}

static void ht_entry_chain_ins(fnv32_ht_entry *hte, fnv32_ht_entry *next) {
    if (!strcmp(hte->key, next->key)) {
        hte->val = next->val;
        ht_entry_free(next);
    } else if (hte->next != NULL) 
        ht_entry_chain_ins(hte->next, next);
    else 
        hte->next = next;
}

static int8_t ht_entry_chain_del(fnv32_ht_entry *hte, char *key) {
    if (!strcmp(hte->next->key, key)) {
        fnv32_ht_entry *next = hte->next;
        if (next->next != NULL)
            hte->next = next->next;
        free(next);
        return 0;
    } else if (hte->next->next != NULL)
        return ht_entry_chain_del(hte->next, key);
    return -1;
}

static int32_t ht_entry_chain_get(fnv32_ht_entry *hte, char *key) {
    if (!strcmp(hte->key, key))
        return hte->val;
    else if (hte->next != NULL)
        return ht_entry_chain_get(hte->next, key);
    return -1;
}

static void ht_entry_chain_free(fnv32_ht_entry *hte) {
    if (hte->next != NULL) 
        ht_entry_chain_free(hte->next);
    free(hte);
}

// INTERFACE

fnv32_ht *fnv32_ht_new(uint32_t size) {
    fnv32_ht *new_ht;
    if ((new_ht = malloc(sizeof(fnv32_ht))) == NULL) {
        // HANDLE HT ALLOC FAILURE
        return NULL;
    } 
    if ((new_ht->table = malloc(size * sizeof(fnv32_ht_entry))) == NULL) {
        // HANDLE TABLE ALLOC FAILURE
        return NULL;
    }

    for (int i = 0; i < size; i++)
        new_ht->table[i] = NULL;

    new_ht->size = size;
    return new_ht;
}

int8_t fnv32_ht_ins(fnv32_ht *ht, char *key, uint32_t val) {
    fnv32_ht_entry *new_entry;
    if ((new_entry = ht_entry_new(key, val)) == NULL)
        return -1;
    
    uint32_t hash = fnv32_hash_str(key); 
    uint32_t index =  hash % ht->size;
    
    if (ht->table[index] != NULL) {
        ht_entry_chain_ins(ht->table[index], new_entry);        
    } else {
        ht->table[index] = new_entry;
    }
    return 1;
}

int8_t fnv32_ht_del(fnv32_ht *ht, char *key) {
    uint32_t hash = fnv32_hash_str(key); 
    uint32_t index =  hash % ht->size;

    if (ht->table[index] != NULL) {
        if (!strcmp(ht->table[index]->key, key)) {
            fnv32_ht_entry *hte = ht->table[index];
            ht->table[index] = hte->next;
            free(hte);
            return 0;
        } else if (ht->table[index]->next != NULL)
            return ht_entry_chain_del(ht->table[index], key);
    } 
    return -1;
}


int32_t fnv32_ht_get(fnv32_ht *ht, char *key) {
    uint32_t hash = fnv32_hash_str(key); 
    uint32_t index =  hash % ht->size;

    if (ht->table[index] != NULL) {
        if (!strcmp(ht->table[index]->key, key))
            return ht->table[index]->val;
        else if (ht->table[index]->next != NULL)
            return ht_entry_chain_get(ht->table[index], key);
    } 
    return -1;
}

void fnv32_ht_free(fnv32_ht *ht) {
    for (int i = 0; i < ht->size; i++) {
        if (ht->table[i] != NULL) {
            if (ht->table[i]->next != NULL)
                ht_entry_chain_free(ht->table[i]);
            else 
                ht_entry_free(ht->table[i]);
        }
    }
    free(ht);
}
