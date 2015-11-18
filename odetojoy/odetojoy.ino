#include <toneAC_RTTTL.h>

char song_buffer[500];
prog_char bubble[] PROGMEM = "Beethoven:d=4,o=6,b=100:a5,a5,a_5,c,c,a_5,a5,g5,f5,f5,g5,a5,a.5,8g5,2g5,a5,a5,a_5,c,c,a_5,a5,g5,f5,f5,g5,a5,g.5,8f5,2f5,g5,g5,a5,f5,g5,8a5,8a_5,a5,f5,g5,8a5,8a_5,a5,g5,f5,g5,c5,2a5,a5,a_5,c,c,a_5,a5,g5,f5,f5,g5,a5,g.5,8f5,2f5";
strcpy_P(song_buffer, (char*)pgm_read_word(&Beethoven));
toneAC_RTTTL(song_buffer, volume, true);
