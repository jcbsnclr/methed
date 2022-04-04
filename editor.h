#ifndef editor_h_INCLUDED
#define editor_h_INCLUDED

#include <stddef.h>
#include <stdbool.h>

struct editor {
	int file;
	char *data;
	size_t len;
	size_t cap;
	bool writable;
};

struct editor editor_open(char *path);
void editor_close(struct editor *ed);

void editor_insert(struct editor *ed, char *data, size_t len, size_t at);
void editor_delete(struct editor *ed, size_t len, size_t at);

void editor_flush(struct editor *ed);

#endif // editor_h_INCLUDED

