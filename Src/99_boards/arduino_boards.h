

#ifndef ARDUINO_BOARDS_H
#define ARDUINO_BOARDS_H

#ifdef arduino_nano
    #include "arduino_nano_cfg.h"
    #define BOARD_INCLUDED
#endif

#ifdef arduino_pro_mini_5v
    #include "arduino_pro_mini.h"
    #define BOARD_INCLUDED
#endif

#ifdef arduino_pro_mini_3v3
    #include "arduino_pro_mini.h"
    #define BOARD_INCLUDED
#endif

#ifndef BOARD_INCLUDED
    #error "Define your board before including this file"
#endif

#ifdef BOARD_INCLUDED
    #undef BOARD_INCLUDED
#endif

// include generic functions after board config
// #include ...

#endif /* ARDUINO_BOARDS_H */
