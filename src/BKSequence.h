/*
 * Copyright (c) 2012-2015 Simon Schoenenberger
 * http://blipkit.audio
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef _BK_SEQUENCE_H_
#define _BK_SEQUENCE_H_

#include "BKBase.h"

typedef struct BKSequence      BKSequence;
typedef struct BKSequenceState BKSequenceState;
typedef struct BKSequenceFuncs BKSequenceFuncs;
typedef struct BKSequencePhase BKSequencePhase;

/**
 * Sequence phases
 */
enum
{
	// sequence state is muted (default)
	BK_SEQUENCE_PHASE_MUTE,

	// sequence state is in attack phase
	BK_SEQUENCE_PHASE_ATTACK,

	// sequence state is in release phase
	BK_SEQUENCE_PHASE_RELEASE,
};

/**
 * Step return values
 */
enum
{
	// no step
	BK_SEQUENCE_RETURN_NONE = 0,

	// normal step
	BK_SEQUENCE_RETURN_STEP = 1 << 0,

	// sustain sequence was restarted
	BK_SEQUENCE_RETURN_REPEAT = 1 << 1,

	// sequence has finished
	BK_SEQUENCE_RETURN_FINISH = 1 << 2,

	// mask for active sequence
	BK_SEQUENCE_RETURN_ACTIVE_MASK = BK_SEQUENCE_RETURN_STEP | BK_SEQUENCE_RETURN_REPEAT,
};

/**
 * Step levels
 */
enum
{
	// envelope step
	BK_SEQUENCE_STEP_MAX,

	// sequence step
	BK_SEQUENCE_STEP_DIVIDED,
};

/**
 * Defines function prototypes of sequence types
 */
struct BKSequenceFuncs
{
	// create a sequence or envelope
	BKInt (* create) (BKSequence ** outSequence, BKSequenceFuncs const * funcs, void const * values, BKUInt length, BKUInt sustainOffset, BKUInt sustainLength);

	// advance state by one step
	// `level` indicates the step level
	BKEnum (* step) (BKSequenceState * state, BKEnum level);

	// set current value of sequence
	// e.g. before attacking duty cycle envelope to slide from current value
	BKInt (* setValue) (BKSequenceState * state, BKInt value);

	// set phase of sequence state
	BKInt (* setPhase) (BKSequenceState * state, BKEnum phase);

	// copy sequence
	BKInt (* copy) (BKSequence ** outCopy, BKSequence const * sequence);
};

/**
 * Defines phase used in envelope sequence
 */
struct BKSequencePhase
{
	BKUInt steps;
	BKInt  value;
};

/**
 * Defines a sequence
 * This can be a simple array of values or an envelope
 */
struct BKSequence
{
	BKSequenceFuncs const * funcs;
	BKSequenceState       * stateList;
	BKInt                   length;
	BKInt                   sustainOffset;
	BKInt                   sustainLength;
	BKInt                   fracShift;
	//BKInt                   defaultValue;
	BKEnum                  state;
	void                  * values;
};

/**
 * Holds the state of a sequence
 */
struct BKSequenceState
{
	BKSequence      * sequence;
	BKSequenceState * prevState;
	BKSequenceState * nextState;
	BKEnum            phase;
	BKInt             steps;
	BKInt             delta;
	BKInt             offset;
	BKInt             value;
	BKInt             shiftedValue;
	BKInt             endValue;
};

/**
 * Sequence prototype functions
 */
extern BKSequenceFuncs const BKSequenceFuncsSimple;
extern BKSequenceFuncs const BKSequenceFuncsEnvelope;

/**
 * Create a sequence or envelope
 */
extern BKInt BKSequenceCreate (BKSequence ** outSequence, BKSequenceFuncs const * funcs, void const * values, BKUInt length, BKUInt sustainOffset, BKUInt sustainLength);

/**
 * Create copy of sequence
 */
extern BKInt BKSequenceCopy (BKSequence ** outSequence, BKSequence * sequence);

/**
 * Dispose sequence
 */
extern void BKSequenceDispose (BKSequence * sequence);

/**
 * Set new sequence
 */
extern BKInt BKSequenceStateSetSequence (BKSequenceState * state, BKSequence * sequence);

/**
 * Set phase
 */
extern BKInt BKSequenceStateSetPhase (BKSequenceState * state, BKEnum phase);

/**
 * Call step function
 */
extern BKInt BKSequenceStateStep (BKSequenceState * state, BKEnum level);

/**
 * Call setValue function
 */
extern BKInt BKSequenceStateSetValue (BKSequenceState * state, BKInt value);

#endif /* ! _BK_SEQUENCE_H_ */
