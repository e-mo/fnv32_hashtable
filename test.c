#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "fnv32_ht.h"

void single_bucket() {
	char *foo = strdup("foo");
	char *bar = strdup("bar");
	char *meow = strdup("meow");

	// Single bucket hashtable so all entries are in single linked list
	fnv32_ht *ht = fnv32_ht_new(1);
	assert(ht != NULL);

	// Insert
	assert(fnv32_ht_ins(ht, "foo", foo) == 0);
	assert(fnv32_ht_ins(ht, "bar", bar) == 0);
	assert(fnv32_ht_ins(ht, "meow", meow) == 0);

	// Delete
	// Delete fail: "fail" not found
	assert(fnv32_ht_del(ht, "fail") == -1);
	// Delete success: "foo" found and deleted
	assert(fnv32_ht_del(ht, "foo") == 0); 
	// Get fail: confirmation that "foo" entry no longer exists
	assert(fnv32_ht_get(ht, "foo") == NULL);

	// Remove
	char *rm = fnv32_ht_rm(ht, "meow");
	// Remove success: rm is not NULL and contains string "bar"
	assert(rm != NULL && !strcmp(rm, meow));
	// Get fail: confirmaiton that "bar" entry no longer exists
	assert(fnv32_ht_get(ht, "meow") == NULL);
	free(rm);

	// Get
	char *get = fnv32_ht_get(ht, "fail");
	// Get fail: "fail" not found
	assert(get == NULL);
	get = fnv32_ht_get(ht, "bar");
	// Get success: "meow" found and retrieved
	assert(get != NULL && !strcmp(get, bar));

	// Free
	fnv32_ht_free(ht);
}

void dictionary(int buckets) {
	// Use much smaller than optimal number to ensure
	// many collisions
	fnv32_ht *ht = fnv32_ht_new(buckets);
	assert(ht != NULL);

	char buf[100];
	FILE *dict = fopen("dictionary.txt", "r");
	assert(dict != NULL);
	while (fgets(buf, 100, dict) != NULL) {
		buf[strlen(buf)-1] = '\0';
		assert(fnv32_ht_ins(ht, buf, strdup(buf)) == 0);
	}
	fseek(dict, 0, SEEK_SET);

	int delete = 1;
	int count = 0;
	while (fgets(buf, 100, dict) != NULL) {
		// Toggle between delete and remove on every other entry
		if (count%2 == 0) {
			buf[strlen(buf)-1] = '\0';
			if (delete) {
				assert(fnv32_ht_del(ht, buf) == 0);
				assert(fnv32_ht_del(ht, buf) == -1);
				delete = 0;
			} else {
				char *rm = fnv32_ht_rm(ht, buf);
				assert(!strcmp(rm, buf));
				free(rm);
				rm = fnv32_ht_rm(ht, buf);
				assert(rm == NULL);
				delete = 1;
			}
		}
		count++;
	}

	fclose(dict);
	fnv32_ht_free(ht);
}		

int main() {
	clock_t start, end;
	double cpu_time;

	printf("Test 1: single bucket: starting - ");
	fflush(stdout);
	start = clock();
	single_bucket();   // Only collisions, 3 entries
	end = clock();
	cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Passed!\n");
	printf(" Duration: %f seconds\n", cpu_time);

	printf("Test 2: ~40,000 entries, many collisions (5,021 buckets): starting - ");
	fflush(stdout);
	start = clock();
	dictionary(5021);  // Many collisions, ~40,000 entries
	end = clock();
	cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Passed!\n");
	printf(" Duration: %f seconds\n", cpu_time);

	printf("Test 3: ~40,000 entries, minimal collisions (1,150,139 buckets): starting - ");
	fflush(stdout);
	start = clock();
	dictionary(1150139); // Few collisions, ~40,000 entries
	end = clock();
	cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
	printf("Passed!\n");
	printf(" Duration: %f seconds\n", cpu_time);

	return 0;
}
