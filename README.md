# fnv32_hashtable
###A simple <str, int> hash table using the FNV-1a hashing algorithm.

HASHING ALGORITHM:    FNV-1a  
KEY TYPE:             string (char *)  
VALUE TYPE:           int (uint32_t)  
COLLISION RESOLUTION: separate chaining  
 
###INTERFACE 
typedef struct fnv32_ht fnv32_ht;
    - Incomplete type for holding **fnv32_ht *** returned by fnv32_ht_new()

###Example
```c
#include "fnv32_ht.h"

int main() {
  fnv32_ht *ht = fnv32_ht_new(50);    // Create new hashtable with 50 buckets
  fnv32_ht_ins(ht, "e-mo", 33);       // Insert entry with key "e-mo" and value 33
  int32_t val = fnv32_ht_get("e-mo"); // Retrieve value associated with key "e-mo" (33)
  fnv32_ht_del("e-mo");               // Remove entry associated with key "e-mo"
  fnv32_ht_free(ht);                  // Free hashtable
}
```

*More info on FNV-1a at http://www.isthe.com/chongo/tech/comp/fnv/
