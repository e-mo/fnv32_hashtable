#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "fnv32_ht.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("wrong number of args\n");
        return -1;
    }
    clock_t start, end, diff;
    double time;
    double min = 0;
    double max = 0;
    double total = 0;
    double avg;

    char word[50];
    int num_buckets = atoi(argv[1]);
    int max_words = atoi(argv[2]);
    int line_offset = atoi(argv[3]);

    fnv32_ht *ht = fnv32_ht_new(num_buckets);

    printf("fnv32_ht speed test\n");
    printf("buckets: %d\n", num_buckets);
    printf("entries: %d\n", max_words);
    printf("offset:  %d\n", line_offset);

    FILE *dict = fopen("dictionary.txt", "r");
    int num_words = 0;
    for (int i = 0; fgets(word, sizeof(word), dict) !=NULL; i++) {
        if (!(i % line_offset) && i != 0) {
            start = clock();
            fnv32_ht_ins(ht, word, i);
            end = clock();
            diff = end - start;
            time = ((double)diff) / CLOCKS_PER_SEC;
            num_words++;

            total += time;
            if (time < min || min == 0)
                min = time;
            if (time > max || max == 0)
                max = time;
        }
        if (num_words >= max_words)
            break;
    }
    fclose(dict);
    avg = total / num_words;
    printf("INSERT->\n");
    printf("TOT: %f\n", total);
    printf("AVG: %f\n", avg);
    printf("MIN: %f\n", min);
    printf("MAX: %f\n", max);

    dict = fopen("dictionary.txt", "r");
    num_words = 0;
    total = 0;
    for (int i = 0; fgets(word, sizeof(word), dict) !=NULL; i++) {
        if (!(i % line_offset) && i != 0) {
            start = clock();
            fnv32_ht_get(ht, word);
            end = clock();
            diff = end - start;
            time = ((double)diff) / CLOCKS_PER_SEC;
            num_words++;

            total += time;
            if (time < min || min == 0)
                min = time;
            if (time > max || max == 0)
                max = time;
        }
        if (num_words >= max_words)
            break;
    }
    fclose(dict);
    avg = total / num_words;
    printf("GET->\n");
    printf("TOT: %f\n", total);
    printf("AVG: %f\n", avg);
    printf("MIN: %f\n", min);
    printf("MAX: %f\n", max);

    fnv32_ht_free(ht);

    return 0;
}
