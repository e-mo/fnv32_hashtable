# fnv32_hashtable
### A simple <str, int> hash table using the FNV-1a hashing algorithm.
```
HASHING ALGORITHM:    FNV-1a*  
KEY TYPE:             string (char *)  
VALUE TYPE:           int (uint32_t)  
COLLISION RESOLUTION: separate chaining
```
||
---:|---:|---:|
meow | meow | meow |

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
int8_t fnv32_ht_ins(fnv32_ht *ht, char *key, uint32_t val);
// Returns 0 on successful insertion of new table entry <key, val>
// Returns -1 on failure to allocate
```
```c
int8_t fnv32_ht_del(fnv32_ht *ht, char *key);
// Returns 0 on successful deletion of entry associated with given key
// Returns -1 on failure to find entry associated with given key
```
```c
int32_t fnv32_ht_get(fnv32_ht *ht, char *key);
// Returns uint32_t value of entry associated with given key
// Return -1 on failiure to find entry associated with given key
```
```c
void fnv32_ht_free(fnv32_ht *ht);
// Frees given fnv32_ht and every associated fnv32_ht_entry
```
### Example
```c

#include "fnv32_ht.h"

int main() {
  fnv32_ht *ht = fnv32_ht_new(50);    // Create new hashtable with 50 index table
  fnv32_ht_ins(ht, "e-mo", 33);       // Insert entry with key "e-mo" and value 33
  int32_t val = fnv32_ht_get("e-mo"); // Retrieve value associated with key "e-mo" (33)
  fnv32_ht_del("e-mo");               // Remove entry associated with key "e-mo"
  fnv32_ht_free(ht);                  // Free hashtable
}
```

