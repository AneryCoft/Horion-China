#pragma once

class TimerUtil {
public:
	TimerUtil() noexcept;
	~TimerUtil() noexcept = default;
	bool hasTimedElapsed(__int64 time, bool reset);
	void setDifference(__int64 difference);
	bool hasTimePassed(__int64 ms);
	__int64 getElapsedTime1();
	bool elapsed(__int64 ms);
	__int64 getElapsedTime();
	__int64 getDifference();
	__int64 getTime();
	void resetTime();
	void reset2();
	void reset1();
	
	static __int64 getCurrentMs();

private:
	__int64 lastMS;
	__int64 ms;
	__int64 prevMS;
	__int64 timeMS;
};
