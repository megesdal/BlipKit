#include "test.h"

int main (int argc, char const * argv [])
{
	BKInt res;
	BKTrack * track = INVALID_PTR;

	// check for allocation

	res = BKTrackAlloc (& track, BK_SQUARE);

	if (res != 0) {
		fprintf (stderr, "Track allocation failed (%d)\n", res);
		return 99;
	}

	if (track == INVALID_PTR || track == NULL) {
		fprintf (stderr, "Invalid pointer (%p)\n", track);
		return 99;
	}

	BKDispose (track);

	// check for init values

	res = BKTrackAlloc (& track, -345876);

	if (track -> waveform != 0) {
		fprintf (stderr, "Invalid initial value (%d)\n", res);
		return 99;
	}

	// check for allocation

	BKContext * ctx = INVALID_PTR;

	res = BKContextAlloc (& ctx, 2, 44100);

	if (res != 0) {
		fprintf (stderr, "Context allocation failed (%d)\n", res);
		return 99;
	}

	res = BKTrackAttach (track, ctx);

	if (res != 0) {
		fprintf (stderr, "Not attached (%d)\n", res);
		return 99;
	}

	BKTrackDetach (track);

	if (track -> unit.ctx != NULL) {
		fprintf (stderr, "Not detached\n");
		return 99;
	}

	BKDispose (track);
	BKDispose (ctx);

	return 0;
}