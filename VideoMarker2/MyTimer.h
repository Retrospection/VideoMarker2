#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <functional>



template <typename R = void, typename... Args>
class CMyTimer
{
public:
	CMyTimer(size_t internal, std::function<R(Args...)> fn)
		:m_nInternal(internal), m_Func(fn), m_bRunning(true)
	{}
	~CMyTimer(){}
	void StartTimer()
	{
		m_bRunning = true;
		m_LaskTimePoint = std::chrono::high_resolution_clock::now();
		m_Thread = std::thread(std::bind(&CMyTimer::_Impl, this));
		m_Thread.detach();
	}
	void KillTimer()
	{
		m_bRunning = false;
	}

	void SetInterval(size_t interval)
	{
		m_nInternal = interval;
	}

private:

	void _Impl()
	{
		while (m_bRunning)
		{
			if (CalculateTimeSpanInMilliseconds(m_LaskTimePoint, std::chrono::high_resolution_clock::now()) < m_nInternal)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				continue;
			}
			m_Func();
			m_LaskTimePoint = std::chrono::high_resolution_clock::now();
		}
	}


	long long CalculateTimeSpanInMilliseconds(const std::chrono::high_resolution_clock::time_point& start, const std::chrono::high_resolution_clock::time_point& end)
	{
		std::chrono::milliseconds span = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		return span.count();
	}

	std::thread m_Thread;
	std::function<R(Args...)> m_Func;
	size_t m_nInternal;
	bool m_bRunning;
	std::chrono::high_resolution_clock::time_point m_LaskTimePoint;
};