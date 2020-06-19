#ifndef BBP_TIMINGS_HPP
#define BBP_TIMINGS_HPP

namespace TroubleBrewing::Timings
{
	inline constexpr int firstDayTimeLengthSeconds = 350;
	inline constexpr int dayTimeLengthSeconds = 250;
	inline constexpr int nominationSeconds = 25;
	inline constexpr int postNightActionDelaySeconds = 10;
	inline constexpr int explainNominationTimeSeconds = 15;
	inline constexpr int defendTimeSeconds = 15;
	inline constexpr int enterPlayerTimeoutSeconds = 60;
	inline constexpr int voteTimeSeconds = 20;
}

#endif //BBP_TIMINGS_HPP
