#include "utils.h"

int32_t randint(int32_t min, int32_t max) {
    if (min == max) return min;
    if (min > max) {
        int32_t t = min; min = max; max = t;
    }

    uint64_t range = (uint64_t)max - (uint64_t)min + 1ULL;
    if (range == 0ULL) return min; 

    uint32_t r = eadk_random();
    int32_t offset = (int32_t)(r % range);
    return min + offset;
}