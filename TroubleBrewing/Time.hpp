#ifndef BBP_TIME_HPP
#define BBP_TIME_HPP

#include <iostream>

namespace TroubleBrewing
{

class Time
{
	bool isDay;
	int dayOrNightCount;

public:
	explicit Time() : isDay{false}, dayOrNightCount{0}{};

	Time (bool _isDay, int _dayOrNightCount) : isDay{_isDay}, dayOrNightCount{_dayOrNightCount}{};

	inline bool IsDay() const { return isDay; }
	inline bool DayOrNightCount() const { return dayOrNightCount; }

	inline Time& Increment()
	{
		if (isDay)
			isDay = false;
		else {
			isDay = true;
			++dayOrNightCount;
		}

		return *this;
	}
};

inline bool operator==(Time a, Time b)
{
	return a.IsDay() == b.IsDay() && a.DayOrNightCount() == b.DayOrNightCount();
}

inline Time& operator++(Time& a)
{
	return a.Increment();
}

inline bool operator<(Time a, Time b)
{
	return (a.DayOrNightCount() < b.DayOrNightCount()) || (a.IsDay() && !b.IsDay());
}

inline bool operator<=(Time a, Time b)
{
	return (a < b) || (a == b);
}

inline bool operator>(Time a, Time b)
{
	return !(a <= b);
}

inline bool operator>=(Time a, Time b)
{
	return !(a < b);
}

inline std::ostream& operator<<(std::ostream& stream, Time a)
{
	stream << (a.IsDay() ? "Day " : "Night ") << a.DayOrNightCount();
}

}

#endif //BBP_TIME_HPP
