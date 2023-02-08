#ifndef UTILS_H
# define UTILS_H

int to_parse(uint8_t *buffer, int buffer_size);
void free_buffers(client_t *clients, size_t clients_len);

#endif /* utils.h */