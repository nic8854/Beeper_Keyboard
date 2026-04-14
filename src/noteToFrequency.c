#include "noteToFrequency.h"
#include <math.h>


uint16_t noteToFrequency(note_t note, octave_t octave) {
    if(note < Note_C || note > Note_H || octave < Octave_0 || octave > Octave_8) {
        return 0; // Invalid note or octave
    }
    int16_t semitoneDifference = (octave - Octave_4) * 12 + (note - Note_A);
    return (uint16_t)(BASE_A4 * pow(2, semitoneDifference / 12.0));
}