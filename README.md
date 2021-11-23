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
void * fnv32_ht_rm(fnv32_ht *ht, char *key);
// Returns (void *) value of entry associated with given key
// Return NULL on failure to find entry associated with given key
// After returning value, table entry is freed.
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
  fnv32_ht *ht = fnv32_ht_new(50);    // Create new hashtable with 50 buckets
  void *test_val = malloc(10);        // Create data
  fnv32_ht_ins(ht, "e-mo", test_val); // Insert entry with key "e-mo" and test_val pointer
  void *val = fnv32_ht_get("e-mo");   // Retrieve value associated with key "e-mo" (33)
  fnv32_ht_del("e-mo");               // Remove entry associated with key "e-mo"
  fnv32_ht_free(ht);                  // Free hashtable
}
```
## Building library
A makefile is provided to simplify building as a library and installing.
Use `make help` for instructions.
  
## Pitfalls

### Double free
There are two situations which can lead to a double call of free() on the same data:
1. Data at a single memory location is associated with multiple keys.
```c
void *test_val = malloc(10);
fnv32_ht_ins(ht, "one", test_val);
fnv32_ht_ins(ht, "two", test_val);
fnv32_ht_free(ht); // This will result in a double free
```
To avoid this, make sure every entry carries unique data. If data must be inserted into multiple entries, you must either use a 
deep copy of the data, or manually NULL each entry before freeing using
`fnv32_ht_ins(fnv32_ht *ht, char *key)`. 
  
2. Calling free() on memory associated with a key, and then calling
`fnv32_ht_free(fnv32_ht *ht)` or `fnv32_ht_del(fnv32_ht *ht, char *key)` 
on the entry.
```c
void *test_val = malloc(10);
fnv32_ht_ins(ht, "test", test_val);
free(test_val);
fnv32_ht_free(ht); // This will result in a double free
```
```c
void *test_val = malloc(10);
fnv32_ht_ins(ht, "test", test_val);
free(test_val);
fnv32_ht_del("test"); // This will result in a double free
```
To avoid these pitfalls, treat the hashtable as if it has ultimate ownership over any data given to it. If data must be removed from the table and freed manually, use the `fnv32_ht_rm(fnv32_ht *ht, char *key)` function which returns the data and breaks the table's ownership of the data.  
