#ifndef NOTES_TO_FREQ_H
#define NOTES_TO_FREQ_H

#include <stdint.h>

#define BASE_A4 440

typedef enum note_t {
    Note_C = 0,
    Note_Cs = 1,
    Note_D = 2,
    Note_Ds = 3,
    Note_E = 4,
    Note_F = 5,
    Note_Fs = 6,
    Note_G = 7,
    Note_Gs = 8,
    Note_A = 9,
    Note_As = 10,
    Note_H = 11
} note_t;

typedef enum octave_t {
    Octave_0 = 0,
    Octave_1 = 1,
    Octave_2 = 2,
    Octave_3 = 3,
    Octave_4 = 4,
    Octave_5 = 5,
    Octave_6 = 6,
    Octave_7 = 7,
    Octave_8 = 8
} octave_t;

uint16_t noteToFrequency(note_t note, octave_t octave);

#endif // NOTES_TO_FREQ_H
