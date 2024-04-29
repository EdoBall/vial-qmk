#ifndef TOUCH_KEY_H
#define TOUCH_KEY_H
// Added By EdoBall
typedef struct {
    uint8_t out_pin;
    uint8_t in_pin;
    uint8_t status;
    uint16_t time;
} touch_key_t;

extern touch_key_t touch_key[4];
bool touch_key_init(void);
uint8_t get_touch_keys_status(void);

// Configulations
// Touch key pins
// touch key 1 output pin
#define TKOP1 GP8
// touch key 1 input pin
#define TKIP1 GP9
#define TKOP2 GP10
#define TKIP2 GP11
#define TKOP3 GP12
#define TKIP3 GP13
#define TKOP4 GP14
#define TKIP4 GP15

#define ON_THD 10
#define OFF_THD 10



#endif
