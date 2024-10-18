#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "loggerinterface.h"

#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

class ConsoleLogger : public LoggerInterface
{
public:
	~ConsoleLogger();

	void write(QueueBase &command) override;
	void stop();
	bool isEmpty();

private:
	friend class DataKeeper;
	ConsoleLogger();

	std::mutex m_mutex;
	std::thread m_thread;
	std::condition_variable m_conditionVariable;
	bool m_doWork;

	std::queue<std::string> m_toWrite;

	void worker();
};

#endif // CONSOLELOGGER_H
