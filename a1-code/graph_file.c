#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deltas.h"

int main(int argc, char *argv[]) {
	if (argc < 4) {
		printf("usage: %s <format> <filename> <height>\n", argv[0]);
		printf(" <format> is one of\n");
		printf(" text : text ints are in the given filename\n");
		printf(" int  : binary ints are in the given filename\n");
		printf(" 4bit : 4bit binary ints are in the given filename\n");
		return 1;
	}

	char *format = argv[1];
	char *fname = argv[2];
	int height = atoi(argv[3]);

	if (height > 0) {
		int data_len = -1;
		int *data_vals = NULL;

		if (strcmp("text", format) == 0) {
			printf("Reading text format\n");
			data_vals = read_text_deltas(fname, &data_len);
		}
		else if (strcmp("int", format) == 0) {
			printf("Reading binary int format\n");
			data_vals = read_int_deltas(fname, &data_len);
		}
		else {
			printf("Unknown format '%s'\n", format);
			return 1;
		}

		if (data_vals && data_len > 0) {		  //If there is data
			print_graph(data_vals, data_len, height);		  //print the graph
			free(data_vals);		  // free the int arr
		}
	}

	return 0;
}
