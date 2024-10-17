#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "loggerinterface.h"

#include <mutex>
#include <thread>

class ConsoleLogger : public LoggerInterface
{
public:
	ConsoleLogger() = default;

	void write(QueueBase &command) override;

private:
	std::mutex m_mutex;

	std::thread m_thread;
};

#endif // CONSOLELOGGER_H
