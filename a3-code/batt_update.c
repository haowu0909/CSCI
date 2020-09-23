/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "batt.h"


int batt_update() {
	batt_t batt ;
	int result = set_batt_from_ports(&batt);
	if (result == 1) {
		return 1;
	}
	result = set_display_from_batt(batt, &BATT_DISPLAY_PORT);
	return 0;
}


int set_batt_from_ports(batt_t *batt) {
	// if BATT_VOLTAGE_PORT is negative, then battery has been wired wrong;
	// no fields of 'batt' are changed and 1 is returned to indicate an
	// error.
	if (BATT_VOLTAGE_PORT < 0) {
		return 1;
	}


	batt->mode = (BATT_STATUS_PORT & 0x01);


	batt->volts = BATT_VOLTAGE_PORT;

	if (BATT_VOLTAGE_PORT < 3000) {
		batt->percent = 0;
	}
	else if (BATT_VOLTAGE_PORT > 3800) {
		batt->percent = 100;
	}
	else {
		batt->percent = (BATT_VOLTAGE_PORT - 3000) / 8;
	}
	return 0;
}


int set_display_from_batt(batt_t batt, int *display) {
	int masks[10] = { 0b0111111,	//0
			0b0000011,	//1
			0b1101101,	//2
			0b1100111,	//3
			0b1010011,	//4
			0b1110110,	//5
			0b1111110,	//6
			0b0100011,	//7
			0b1111111,	//8
			0b1110111	//9
			};
	*display = 0;


	if (batt.mode == 0) { //voltage mode
		//Bits 21 controls whether the decimal place is on (for Volts) or off (for percent)
		*display = (*display) | (1 << 21);  //. on
		*display = (*display) | (1 << 22);  //V on

		batt.volts += 5;
		int left_digit = batt.volts  / 1000;
		int middle_digit = (batt.volts - left_digit * 1000) / 100;
		int right_digit = (batt.volts - middle_digit * 100 - left_digit * 1000) / 10;

		*display = (*display) | (masks[left_digit] << 14);
		*display = (*display) | (masks[middle_digit] << 7);
		*display = (*display) | masks[right_digit];
0b
	} else if (batt.mode == 1) { //percent mode

		*display = (*display) | (1 << 23);//% on


		int left_digit = (batt.percent / 100) % 10;
		int middle_digit = (batt.percent / 10) % 10;
		int right_digit = (batt.percent) % 10;

		*display = (*display) | masks[right_digit];

		if (middle_digit || left_digit) {//070 => 70
			*display = (*display) | (masks[middle_digit] << 7);
		}
		if (left_digit) {
			*display = (*display) | (masks[left_digit] << 14);
		}
	}

	//Bits 24 and 28 control the 'bars' in the visual level indicator
	if (batt.percent >= 5 ) {
		*display = *display | 1 << 28;
	}
	if (batt.percent >= 30 ) {
		*display = *display | 1 << 27;

	}
	if (batt.percent >= 50 ) {
		*display = *display | (1 << 26);

	}
	if (batt.percent >= 70) {
		*display = *display | (1 << 25);

	}
	if (batt.percent >= 90 ) {
		*display = *display | (1 << 24);

	}

	return 0;
}
*/
