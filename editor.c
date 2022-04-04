#include "editor.h"
#include "util.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

void editor_insert(struct editor *ed, char *data, size_t len, size_t at) {
	if (!ed->writable)
    		die(1, "editor_delete", "buffer is opened as read-only");

	if (at > ed->len)
    		die(1, "editor_insert", "insert out of bounds: %lu", at);

	size_t new_len = ed->len + len; 
	bool buf_too_small = new_len > ed->cap;

	// Allocate new buffer if old buffer too small, else use old buffer
	char *buf = buf_too_small ? malloc(new_len) : ed->data;

	memmove(buf, ed->data, at);
	memmove(buf + at + len, ed->data + at, ed->len - at);
	memcpy(buf + at, data, len);

	if (buf_too_small)
    		free(ed->data);

	ed->data = buf;
	ed->len = new_len;
	ed->cap = buf_too_small ? new_len : ed->cap;
}

void editor_delete(struct editor *ed, size_t len, size_t at) {
	if (!ed->writable)
    		die(1, "editor_delete", "buffer is opened as read-only");

	if (at + len > ed->len)
    		die(1, "editor_delete", "delete out of bounds: %lu -> %lu", at, at + len);

	memmove(ed->data + at, ed->data + at + len, ed->len - at - len);

	ed->len -= len;
}

void editor_flush(struct editor *ed) {
	if (!ed->writable)
    		die(1, "editor_flush", "buffer is opened as read-only");

	// Goto start of file and write data
	if (lseek(ed->file, 0, SEEK_SET) == -1)
    		die_errno("editor_flush");

	ssize_t nwrite = write(ed->file, ed->data, ed->len);
	if (nwrite == -1)
    		die_errno("editor_flush");

	int res;

	// Truncate file to new length and sync 
	res = ftruncate(ed->file, ed->len);
	if (res == -1)
    		die_errno("editor_flush");

	res = fsync(ed->file);
	if (res == -1)
    		die_errno("editor_flush");
}

struct editor editor_open(char *path) {
	struct editor ed;

	// try open file as read/write
	ed.file = open(path, O_RDWR);
	ed.writable = true;

	if (ed.file == -1) {
		// try re-opening file as read-only
		if (errno == EACCES) {
    			ed.file = open(path, O_RDONLY);
			ed.writable = false;
		}

		if (ed.file == -1)
    			die_errno("editor_open");
	}

	struct stat st;
	fstat(ed.file, &st);
	size_t file_len = st.st_size;

	ed.data = malloc(file_len);
	ssize_t nread = read(ed.file, ed.data, file_len);

	if (nread == -1)
    		die_errno("editor_open");

	ed.len = file_len;
	ed.cap = file_len;

	return ed;
}

void editor_close(struct editor *ed) {
	ed->len = 0;
	ed->cap = 0;

	free(ed->data);

	if (close(ed->file) == -1)
    		die_errno("editor_close");

	ed->writable = false;
}
