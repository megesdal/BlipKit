#ifndef _BK_INTERPOLATION_H_
#define _BK_INTERPOLATION_H_

#include "BKBase.h"

#define BK_STATE_STEP_FRAC_SHIFT 8
#define BK_STATE_MAX_STEPS (1 << (32 - BK_STATE_STEP_FRAC_SHIFT - 1))

typedef struct BKSlideState    BKSlideState;
typedef struct BKIntervalState BKIntervalState;

struct BKSlideState
{
	BKInt  endValue;
	BKInt  steps;
	BKInt  value;
	BKInt  stepDelta;
	BKInt  roundBias;
	BKUInt valueShift;
	BKInt  step;
};

struct BKIntervalState
{
	BKInt  delta;
	BKInt  steps;
	BKInt  value;
	BKInt  stepDelta;
	BKInt  roundBias;
	BKUInt valueShift;
	BKUInt phase;
	BKInt  step;
};

/**
 * Initialize a slide state struct
 *
 * This struct is used to interpolate from value to another. Due to the usage of
 * variable length fixed point numbers even very small values can be used.
 *
 * `maxValue` defines the absolute positive or negative value which will be used
 * This value is used to determine the fraction length to interpolate the values
 */
extern void BKSlideStateInit (BKSlideState * state, BKInt maxValue);

/**
 * Set the `endValue` which should be slided to after `steps` steps
 *
 * When changing the values before the current slide has finished, the new slide
 * starts from the current interpolated value.
 *
 * `steps` should be a value between 0 and BK_STATE_MAX_STEPS. If it is 0 then
 * `endValue` is set immediately without sliding.
 *
 * The absolute value of `endValue` should not exceed `maxValue` given at
 * initialization or else calculation errors can occur.
 */
extern void BKSlideStateSetValueAndSteps (BKSlideState * state, BKInt endValue, BKInt steps);

/**
 * Set only the `endValue` which should be slided to
 *
 * This is the same as calling `BKSlideStateSetValueAndSteps` without changing
 * the `steps`.
 */
extern void BKSlideStateSetValue (BKSlideState * state, BKInt endValue);

/**
 * Set only the `steps` to reach the `endValue`
 *
 * This is the same as calling `BKSlideStateSetValueAndSteps` without changing
 * the `endValue`.
 */
extern void BKSlideStateSetSteps (BKSlideState * state, BKInt steps);

/**
 * Halt slide
 *
 * Following calls to `BKSlideStateStep` will not change the state value
 * anymore.
 *
 * If `setEndValue` is 1 the state value is set immediately to `endValue`
 * otherwise the interpolated value keeps its current value.
 */
extern void BKSlideStateHalt (BKSlideState * state, BKInt setEndValue);

/**
 * Make slide step
 *
 * Every call slides the value by one step until the end value is reached.
 */
extern void BKSlideStateStep (BKSlideState * state);

/**
 * Get the current interpolated value
 */
extern BKInt BKSlideStateGetValue (BKSlideState const * state);

/**
 * Initialize a interval state struct
 *
 * This struct is used to create a periodic oscillation between a maximum value
 * and its negative counterpart starting from 0.
 *
 * `maxValue` defines the absolute `delta` value used
 * This value is used to determine the fraction length to interpolate the values
 */
extern void BKIntervalStateInit (BKIntervalState * state, BKInt maxValue);

/**
 * Set the maximum and mimimum `delta` value and `steps` for each phase
 *
 * The oscillation has 4 phases and each phase has a duration of `steps` steps:
 *
 * 1. Beginning from 0 the value raises to the maxiumum `delta` value
 * 2. The value lowers to 0
 * 3. The value lowers to the negative `delta` value
 * 4. The value raises back to 0
 *
 * This phases are repeated until the values are changed. When changing the
 * values the current value and phase offset is recalculated for the new values.
 */
extern void BKIntervalStateSetDeltaAndSteps (BKIntervalState * state, BKInt delta, BKInt steps);

/**
 * Set only the maximum and mimimum `delta` value
 *
 * This is the same as calling `BKIntervalStateSetDeltaAndSteps` without
 * changing the `steps`.
 */
extern void BKIntervalStateSetDelta (BKIntervalState * state, BKInt delta);

/**
 * Set only `steps` for each phase
 *
 * This is the same as calling `BKIntervalStateSetDeltaAndSteps` without
 * changing the `delta` value.
 */
extern void BKIntervalStateSetSteps (BKIntervalState * state, BKInt steps);

/**
 * Make oscillation step
 *
 * Every call sets the next oscillation step
 */
extern void BKIntervalStateStep (BKIntervalState * state);

/**
 * Get the current oscillated value
 */
extern BKInt BKIntervalStateGetValue (BKIntervalState const * state);

#endif /* ! _BK_INTERPOLATION_H_ */