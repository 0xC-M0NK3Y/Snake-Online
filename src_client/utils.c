#include <stdint.h>

int to_parse(uint8_t *buffer, int buffer_size) {
    if (buffer_size < 7)
        return 0;
    return ((int)*(uint32_t *)buffer <= buffer_size)?1:0;
}
