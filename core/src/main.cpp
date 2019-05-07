
#include "core.h"

int main(int argc, const char** argv) {
	for (int i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}
	printf("Hello world!\n");
	return 0;
}