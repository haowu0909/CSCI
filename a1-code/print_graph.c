
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int number(int num);
void print_graph(int *data, int len, int max_height);
void print_graph(int *data, int len, int max_height) {
	int x_steps = 5; //lines with a + every 5 steps
	int range = 0;
	int min = data[0], max = data[0];
	double units_per_height = 0;

	//Finding the Maximum and Minimum
	for (int i = 0; i < len; i++) {
		if (data[i] < min) {
			min = data[i];
		} else if (data[i] > max) {
			max = data[i];
		}
	}
	range = max - min;
	units_per_height = (double)range / max_height;

	//print initial statistics
	printf("length: %d\n", len);
	printf("min: %d\n", min);
	printf("max: %d\n", max);
	printf("range: %d\n", range);
	printf("max_height: %d\n", max_height);
	printf("units_per_height: %.2f\n", units_per_height);

	//  print top axe
	for (int i = 0; i < x_steps; i++) {
		printf(" ");
	}
	for (int i = 0; i < len; i++) {
		if (i % x_steps == 0) {
			printf("+");
		} else {
			printf("-");
		}
	}
	printf("\n");

	//doublr-nested loop for printing graph body
	for (int row = max_height; row >= 0; row--) { //print from top (max_height)
		int height_on_row = (double) row * (max - min) / max_height + min;//calculating the height of current row
		printf("%3d |", height_on_row);
		for (int i = 0; i < len; i++) { // check if elements reach height of current row. if so, print X
			if (data[i] >= height_on_row) {
				printf("X");
			} else {
				printf(" ");
			}
		}
		printf("\n");
	}

//   print bottom axe
	for (int i = 0; i < x_steps; i++) {
		printf(" ");
	}
	for (int i = 0; i < len; i++) {
		if (i % x_steps == 0) {
			printf("+");
		} else {
			printf("-");
		}
	}
	printf("\n");

//   number for every 5, under bottom axe
	for (int i = 0; i < x_steps; i++) {
		printf(" ");
	}
	for (int i = 0; i < len;) {
		if (i % x_steps == 0) {
			printf("%d", i);
			i += number(i);	//Number of digits for calculating integers
		} else {
			printf(" ");
			i++;
		}
	}
	printf("\n");
}

//Number of digits for calculating integers
int number(int num) {
	int i = 0;
	do {
		num = num / 10;
		i++;
	} while (num != 0);
	return i;
}
