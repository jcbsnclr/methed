#include "editor.h"
#include "util.h"

int main(int argc, char *argv[]) {
	(void)argc;
	(void)argv;

	struct editor ed = editor_open("test.txt");

	editor_insert(&ed, "Cowabunga", 9, 3);
	editor_delete(&ed, 7, 16);

	editor_flush(&ed);

	editor_close(&ed);

	return 0;
}
