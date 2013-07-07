
#ifndef HIDWiiClassic_h
#define HIDWiiClassic_h

/*
 * Bit masks for the buttons of the Wii Classic controller
 */
#define WII_CLASSIC_A 1
#define WII_CLASSIC_B 2
#define WII_CLASSIC_X 4
#define WII_CLASSIC_Y 8
#define WII_CLASSIC_L 16
#define WII_CLASSIC_R 32
#define WII_CLASSIC_ZL 64
#define WII_CLASSIC_ZR 128
#define WII_CLASSIC_LEFT 256
#define WII_CLASSIC_RIGHT 512
#define WII_CLASSIC_UP 1024
#define WII_CLASSIC_DOWN 2048
#define WII_CLASSIC_SELECT 4096
#define WII_CLASSIC_START 8192
#define WII_CLASSIC_HOME 16384

#define SET_WII_CLASSIC_BUTTON(keys, button) ((keys).buttons |= (button))

typedef struct _HIDWiiClassicKeys
{
    uint16_t buttons;
    int8_t left_x;
    int8_t left_y;
    int8_t right_x;
    int8_t right_y;
} HIDWiiClassicKeys;

class HIDWiiClassic_
{
public:
   HIDWiiClassic_();
   void begin(void);
   void end(void);
   void write(HIDWiiClassicKeys &keys);
};
extern HIDWiiClassic_ HIDWiiClassic;

#endif /* if defined(HIDWiiClassic_h) */
