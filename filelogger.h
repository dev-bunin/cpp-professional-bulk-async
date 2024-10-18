#ifndef FILELOGGER_H
#define FILELOGGER_H

#include "loggerinterface.h"

#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

class FileLogger : public LoggerInterface
{
	struct Entry {
		std::string text;
		std::string time;
	};

public:
	~FileLogger();

	void write(QueueBase &command) override;
	void stop();
	bool isEmpty();

private:
	// Чтоб не мог создать кто угодно
	friend class DataKeeper;
	FileLogger();

	std::mutex m_mutex;
	std::vector<std::thread> m_threads;
	std::condition_variable m_conditionVariable;
	bool m_doWork;

	std::queue<Entry> m_toWrite;

	void worker(int id);
};

#endif // FILELOGGER_H
