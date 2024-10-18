#include "filelogger.h"
#include "commandhandler.h"

#include <fstream>

#define THREAD_COUNT 2

using namespace std;

FileLogger::~FileLogger()
{
	for (auto &thread : m_threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
}

void FileLogger::write(QueueBase &command)
{
	using namespace chrono;
	unique_lock lg(m_mutex);
	Entry entry;
	entry.text = command.getCommands();
	entry.time = to_string(duration_cast<seconds>(
						  command.getStratTime().time_since_epoch()).count());
	m_toWrite.push(entry);
	m_conditionVariable.notify_one();
}

void FileLogger::stop()
{
	m_doWork = false;
	m_conditionVariable.notify_all();
}

bool FileLogger::isEmpty()
{
	unique_lock lg(m_mutex);
	return m_toWrite.empty();
}

FileLogger::FileLogger()
{
	m_doWork = true;
	for (int i = 0; i < THREAD_COUNT; ++i) {
		m_threads.emplace_back(&FileLogger::worker, this, i);
	}
}

void FileLogger::worker(int id)
{
	string strId = to_string(id);
	while(m_doWork) {
		unique_lock lg(m_mutex);
		m_conditionVariable.wait(lg, [this]() {
			return !m_toWrite.empty() || !m_doWork;
		});

		if (m_toWrite.empty()) {
			continue;
		}
		auto entry = m_toWrite.front();
		m_toWrite.pop();

		lg.unlock();

		using namespace chrono;
		std::ofstream outFile;
		string fileName(strId + "_" +entry.time + ".log");
		outFile.open(fileName);
		if (outFile.is_open()) {
			outFile << entry.text;
		}
		outFile.close();
	}
}
