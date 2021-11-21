# fnv32_hashtable
### A simple <str, void *> hash table interface utilizing the FNV-1a hashing algorithm.
```
HASHING ALGORITHM:    FNV-1a*  
KEY TYPE:             string (char *)  
VALUE TYPE:           dynamic (void *)  
COLLISION RESOLUTION: separate chaining
```
*More info on FNV-1a at http://www.isthe.com/chongo/tech/comp/fnv/
 
### INTERFACE 
```c
typedef struct fnv32_ht fnv32_ht;
// Incomplete type for holding (fnv32_ht *) returned by fnv32_ht_new()
```
```c
fnv32_ht *fnv32_ht_new(uint32_t size);
// Returns (fnv32_ht *) to newly allocated fnv32_ht struct containing table of given size
// Returns NULL on failure to allocate
```
```c
int fnv32_ht_ins(fnv32_ht *ht, char *key, void *val);
// Returns 0 on successful insertion of new table entry <key, val>
// Returns -1 on failure to allocate
```
```c
int fnv32_ht_del(fnv32_ht *ht, char *key);
// Returns 0 on successful deletion of entry associated with given key
// Returns -1 on failure to find entry associated with given key
```
```c
void * fnv32_ht_get(fnv32_ht *ht, char *key);
// Returns (void *) value of entry associated with given key
// Return -1 on failiure to find entry associated with given key
```
```c
void fnv32_ht_free(fnv32_ht *ht);
// Frees given fnv32_ht and every associated fnv32_ht_entry
```
### Example
```c

#include <stdlib.h>
#include "fnv32_ht.h"

int main() {
  fnv32_ht *ht = fnv32_ht_new(50);    // Create new hashtable with 50 index table
  void *test_val = malloc(10);		  // Create test data
  fnv32_ht_ins(ht, "e-mo", 33);       // Insert entry with key "e-mo" and test_val pointer
  void *val = fnv32_ht_get("e-mo");   // Retrieve value associated with key "e-mo" (33)
  fnv32_ht_del("e-mo");               // Remove entry associated with key "e-mo"
  fnv32_ht_free(ht);                  // Free hashtable
}
```

### Pitfalls

## Double free
There are two situations which can read to a double call of free() on the same pointer:
1. Data at a single memory location is associated with multiple keys. All values should be unique.
2. You call free() on memory associated with a key, and then call fnv32_ht_free() or fnv32_ht_del() on the entry. Only use fnv32_ht functions to free assicated memory once passed into hashtable. 

