#pragma once
#include "efpch.h"

#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>
#include <iostream>
#include <thread>

struct ProfileResult {
	std::string Name;
	long long Start, End;
	uint32_t ThreadID;
};

struct InstrumentationSession {
	std::string Name;
};

class Instrumentor {
private:
	InstrumentationSession* m_CurrentSession;
	std::ofstream m_OutputStream;
	int m_ProfileCount;
public:
	Instrumentor()
		: m_CurrentSession(nullptr), m_ProfileCount(0)
	{}

	void BeginSession(const std::string& name, const std::string& filepath = "results.json")
	{
		m_OutputStream.open(filepath);
		EF_ASSERT(m_OutputStream.good() && "failed to open profiling results file");
		WriteHeader();
		m_CurrentSession = new InstrumentationSession{ name };
	}

	void EndSession() {
		WriteFooter();
		m_OutputStream.close();
		delete m_CurrentSession;
		m_CurrentSession = nullptr;
		m_ProfileCount = 0;
	}

	void WriteProfile(const ProfileResult& result) {
		if (m_ProfileCount++ > 0)
			m_OutputStream << ",";

		std::string name = result.Name;
		std::replace(std::begin(name), std::end(name), '"', '\'');

		m_OutputStream << "{";
		m_OutputStream << "\"cat\":\"function\",";
		m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
		m_OutputStream << "\"name\":\"" << name << "\",";
		m_OutputStream << "\"ph\":\"X\",";
		m_OutputStream << "\"pid\":0,";
		m_OutputStream << "\"tid\":" << result.ThreadID << ",";
		m_OutputStream << "\"ts\":" << result.Start;
		m_OutputStream << "}";

		m_OutputStream.flush();
	}

	void WriteHeader() {
		m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
		m_OutputStream.flush();
	}

	void WriteFooter() {
		m_OutputStream << "]}";
		m_OutputStream.flush();
	}

	static Instrumentor& Get() {
		static Instrumentor* instance = new Instrumentor();
		return *instance;
	}
};

class InstrumentationTimer {
public:
	InstrumentationTimer(const char* name)
		: m_Name(name), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~InstrumentationTimer() {
		if (!m_Stopped)
			Stop();
	}

	void Stop() {
		auto endTimpoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(m_StartTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(endTimpoint).time_since_epoch().count();

		//std::cout << m_Name << ": " << end - start << "ms\n" << std::endl;

		uint32_t threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
		Instrumentor::Get().WriteProfile({ m_Name, start, end, threadID });

		m_Stopped = true;
	}

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
	const char* m_Name;
	bool m_Stopped;
};

#if PROFILING
#define PROFILE_SCOPE(name) InstrumentationTimer timer##__LINE__(name)
#define PROFILE_FUNCTION() PROFILE_SCOPE(__FUNCSIG__)
#else
#define PROFILE_SCOPE(name)
#define PROFILE_FUNCTION()
#endif