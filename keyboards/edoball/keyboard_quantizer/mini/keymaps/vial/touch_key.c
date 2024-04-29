// Copyright 2024 EdoBall
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "pointing_device.h"
#include "virtser.h"
#include "eeconfig.h"
#include "vial.h"

#include "report_parser.h"
#include "cli.h"
#include "os_key_override.h"
//
#include "touch_key.h"
#include "print.h"

touch_key_t touch_key[4];
// char estr[256]; // a string for sending error messages

bool touch_key_setup_pin(uint8_t output_pin, uint8_t input_pin) {
    setPinOutput(output_pin);
    setPinInput(input_pin);
    return true;
}

bool touch_key_init(void) {
    // added by EdoBall
    touch_key[0].out_pin = TKOP1;
    touch_key[0].in_pin = TKIP1;
    touch_key[0].status = 0;
    touch_key[0].time = 0;
    touch_key[1].out_pin = TKOP2;
    touch_key[1].in_pin = TKIP2;
    touch_key[1].status = 0;
    touch_key[1].time = 0;
    touch_key[2].out_pin = TKOP3;
    touch_key[2].in_pin = TKIP3;
    touch_key[2].status = 0;
    touch_key[2].time = 0;
    touch_key[3].out_pin = TKOP4;
    touch_key[3].in_pin = TKIP4;
    touch_key[3].status = 0;
    touch_key[3].time = 0;

    uint8_t i;
    for (i = 0; i < 4; i++) {
        touch_key_setup_pin(touch_key[i].out_pin, touch_key[i].in_pin);
    }
    // end
    return true;
}

// Added by EdoBall
uint8_t get_a_touch_key_status(uint8_t out_pin, uint8_t in_pin) {
    uint16_t i, j, on_c;
    uint16_t on_delay_c, off_delay_c;
    uint8_t key_status;

    writePinLow(out_pin);
    on_c = 0;
    for (i = 0 ; i < 5; i++) {
    //for (i = 0 ; i < 40; i++) {
	    writePinHigh(out_pin);
	    on_delay_c = 512;
	    for (j = 0; j < 512; j++) {
	        if (readPin(in_pin) == 1) {
		    on_delay_c = j;
		    break;
            }
	    wait_us(1);
	    }
	    writePinLow(out_pin);
	    off_delay_c = 512;
	    for (j = 0; j < 512; j++) {
	        if (readPin(in_pin) == 0) {
		    off_delay_c = j;
		    break;
            }
	        wait_us(1);
	    }
        //sprintf(estr, "n%df%d:", on_delay_c, off_delay_c);
        // SEND_STRING(estr);
        if (on_delay_c > ON_THD && off_delay_c > OFF_THD && on_delay_c < 512 && off_delay_c < 512) {
	        // The key may be touched.
	        on_c++;
            // sprintf(estr, "n%df%d:", on_delay_c, off_delay_c);
            // SEND_sING(estr);
        }
    }
    if (on_c >= 3) {
	// The key is touched.
    key_status = 1;
  } else {
	if (on_c == 0) {
	    // The key may be released.
        key_status = 0;
    } else { //
	  key_status = 2; // undecidable
	}
  }
    return key_status;
}

uint8_t get_touch_key_status(uint8_t kn) {
    uint16_t ignore_time = 43; // 43ms (magic number)

    switch (get_a_touch_key_status(touch_key[kn].out_pin, touch_key[kn].in_pin)) {
    case 0: // The key is released.
        // sprintf(estr, "0");
        if (abs(timer_read() - touch_key[kn].time) > ignore_time) {
            touch_key[kn].status = (touch_key[kn].status << 1) & 0b11111110;
        }
        // touch_key[kn].status = (touch_key[kn].status << 1) & 0b11111110;
        break;
    case 1: // The key is pressed.
        // sprintf(estr, "1");
        touch_key[kn].status = (touch_key[kn].status << 1) | 0b00000001; // The touch key is on.
        touch_key[kn].time = timer_read();
        break;
    case 2: // The key's status is undecidable.
        // sprintf(estr, "2");
        break;
    default:
        // sprintf(estr, "d");
        break;
    }
    // SEND_STRING(estr);
    return(touch_key[kn].status);
}

uint8_t get_touch_keys_status(void) {
    uint8_t i;

    for (i = 0; i < 4; i++) {
        get_touch_key_status(i);
    }
    return(i);
}
