#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deltas.h"
#include <sys/types.h>
#include <sys/stat.h>


int* read_text_deltas(char *fname, int *len) {
	int *arr = NULL;
	*len = -1;

	FILE *fp = fopen(fname, "r"); //Open the file
	if (fp) {
		int number = 0;
		int last = 0;
		int count = 0;
		while (fscanf(fp, "%d", &number) != EOF) { //Read in the int until EOF
				count++;//calculate count
		}

		if (count > 0) {
			*len = count; //int length
			arr = (int*) malloc(sizeof(int) * count); //alloc arr for int

			if (arr) {
				count = 0;
				rewind(fp); //rewinf to file begin
				while (fscanf(fp, "%d", &number) != EOF) { //Read in the int until EOF
					last = last + number; //perform the delta computation of adding on the previous value to the recently read delta
					arr[count++] = last;
				}
			}
		}
		fclose(fp); //close the open file
	}
	else{     //if file open failures return NULL
		return NULL;
		fclose(fp);
	}
	return arr;
}

int* read_int_deltas(char *fname, int *len) {
	int *arr = NULL;
	*len = -1;

	struct stat sb;                                    // struct to hold
	int result = stat(fname, &sb); // unix system call to determine size of named file
	if (result == -1 || sb.st_size < sizeof(int)) { // if something went wrong or bail if file is too small
		return NULL;
	}
	int count = sb.st_size / sizeof(int); //int length

	FILE *fp = fopen(fname, "r+b"); //Open the file in binary format
	if (fp) {
		*len = count;
		arr = (int*) malloc(sizeof(int) * count); //alloc buffer for int
		if (arr) {
			count = 0;
			int number= 0;
			int last = 0;
			while (fread(&number, 1, sizeof(int), fp) == sizeof(int)) { //Read in the int until EOF
				last = last + number; //perform the delta computation of adding on the previous value to the recently read delta
				arr[count++] = last;
			}
		}
		fclose(fp); //close the open file
	}
	else{     //if file open failures return NULL
		return NULL;
		fclose(fp);
	}

	return arr;
}
