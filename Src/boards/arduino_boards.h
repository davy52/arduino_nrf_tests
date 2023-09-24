/*
 * @file arduino_boards.h
 * @author  davy52 (dkliminskip@gmail.com)
 * @brief file for selecting arduino board config
 * @version 0.1
 * @date 2023-09-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */


#ifndef ARDUINO_BOARDS_H
#define ARDUINO_BOARDS_H

#if ARDUINO_BOARD == arduino_nano
    #include "arduino_nano_cfg.h"

#endif

// include generic functions after board config
// #include ...

#endif /* ARDUINO_BOARDS_H */
