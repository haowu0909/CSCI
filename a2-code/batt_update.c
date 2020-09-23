#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "batt.h"

#define VOLTAGE_FLAG 0
#define PERCENT_FLAG 1 // mode: 0 for volts, 1 for percent


void setBit(int *display, int bits, int onoff);

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

extern short BATT_VOLTAGE_PORT;
// Tied to the battery, provides a measure of voltage in units of
// 0.001 volts (milli volts). The sensor can sense a wide range of
// voltages including negatives but the batteries being measured are
// Full when 3.8V (3800 sensor value) or above is read and Empty when
// 3.0V (3000 sensor value) or lower is read.

extern unsigned char BATT_STATUS_PORT;
// Lowest order bit indicates whether display should be in Volts (0)
// or Percent (1). C code should only read this port. Tied to a user
// button which will toggle the bit. Other bits in this char may be
// setBit to indicate the status of other parts of the meter and should
// be ignored: ONLY THE LOW ORDER BIT MATTERS.

extern int BATT_DISPLAY_PORT;
// Controls battery meter display. Readable and writable. C code
// should mostly write this port with a sequence of bits which will
// light up specific elements of the LCD panel.


int batt_update() {
	batt_t batt = { .volts = -100, .percent = -1, .mode = -1 };
	int result = set_batt_from_ports(&batt);
	if (result == 0) {
		int display = 0;
		result = set_display_from_batt(batt, &display);
		BATT_DISPLAY_PORT = display;
	}
	return result;
}


int set_batt_from_ports(batt_t *batt) {
	// if BATT_VOLTAGE_PORT is negative, then battery has been wired wrong;
	// no fields of 'batt' are changed and 1 is returned to indicate an
	// error.
	if (BATT_VOLTAGE_PORT < 0) {
		return 1;
	}

	if ((BATT_STATUS_PORT & 0x01) == VOLTAGE_FLAG) {
		batt->mode = VOLTAGE_FLAG;
	} else if ((BATT_STATUS_PORT & 0x01) == PERCENT_FLAG) {
		batt->mode = PERCENT_FLAG;
	}

	batt->volts = BATT_VOLTAGE_PORT;

	if (BATT_VOLTAGE_PORT < 3000) {
		batt->percent = 0;
	} else if (BATT_VOLTAGE_PORT > 3800) {
		batt->percent = 100;
	} else {
		batt->percent = (BATT_VOLTAGE_PORT - 3000) / 8;
	}
	return 0;
}


int set_display_from_batt(batt_t batt, int *display) {
	*display = 0;

	//Bits 0-6 control the rightmost digit
	//Bits 7-13 control the middle digit
	//Bits 14-20 control the left digit

	//Bits 21 controls whether the decimal place is on (for Volts) or off (for percent)
	//Bit 22 controls whether the 'V' Voltage indicator is shown
	//Bit 23 controls whether the '%' Percent indicator is shown
	if (batt.mode == VOLTAGE_FLAG) {
		//Bits 21 controls whether the decimal place is on (for Volts) or off (for percent)
		setBit(display, 21, 1);	  //. on
		setBit(display, 22, 1);	  //V on
		setBit(display, 23, 0);  	//% off

		//.volts   = 3942
		int volts = (batt.volts + 5) / 10;	//3765 ==> 377
		int left_digit = (volts) % 10;	//4
		int middle_digit = (volts / 10) % 10;	//9
		int right_digit = (volts / 100) % 10;	//3

		*display = (*display) | masks[left_digit];
		*display = (*display) | (masks[middle_digit] << 7);
		*display = (*display) | (masks[right_digit] << 14);

	} else if (batt.mode == PERCENT_FLAG) {
		setBit(display, 21, 0);	//. off
		setBit(display, 22, 0);	//V off
		setBit(display, 23, 1);	//% on

		//.percent = 100
		int left_digit = (batt.percent) % 10;	//0
		int middle_digit = (batt.percent / 10) % 10;	//0
		int right_digit = (batt.percent / 100) % 10;	//1

		*display = (*display) | masks[left_digit];
		if (middle_digit || right_digit) {//070 => 70
			*display = (*display) | (masks[middle_digit] << 7);
		}
		if (right_digit) {
			*display = (*display) | (masks[right_digit] << 14);
		}
	}

	//Bits 24 and 28 control the 'bars' in the visual level indicator
	if (batt.percent >= 5 && batt.percent <= 29) {
		setBit(display, 28, 1);
	} else if (batt.percent >= 30 && batt.percent <= 49) {
		setBit(display, 27, 1);
		setBit(display, 28, 1);
	} else if (batt.percent >= 50 && batt.percent <= 69) {
		setBit(display, 26, 1);
		setBit(display, 27, 1);
		setBit(display, 28, 1);
	} else if (batt.percent >= 70 && batt.percent <= 89) {
		setBit(display, 25, 1);
		setBit(display, 26, 1);
		setBit(display, 27, 1);
		setBit(display, 28, 1);
	} else if (batt.percent >= 90 && batt.percent <= 100) {
		setBit(display, 24, 1);
		setBit(display, 25, 1);
		setBit(display, 26, 1);
		setBit(display, 27, 1);
		setBit(display, 28, 1);
	}

	return 0;
}

void setBit(int *display, int bits, int onoff) {
	if (onoff) {
		*display = (*display) | (1 << bits);	//on
	} else {
		*display = (*display) & (~(1 << bits));	//off
	}
}
