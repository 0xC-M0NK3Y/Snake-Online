#ifndef UTILS_H
# define UTILS_H

int to_parse(uint8_t *buffer, int buffer_size);
void free_buffers(client_t *clients, size_t clients_len);
int send_finish_packets(client_t *client, size_t clients_len, char *winner);

#endif /* utils.h */