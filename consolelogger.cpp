#include "consolelogger.h"
#include "commandhandler.h"

using namespace std;

ConsoleLogger::~ConsoleLogger()
{
	if (m_thread.joinable()) {
		m_thread.join();
	}
}

void ConsoleLogger::write(QueueBase &command)
{
	unique_lock lg(m_mutex);
	m_toWrite.push(command.getCommands());
	m_conditionVariable.notify_one();
}

void ConsoleLogger::stop()
{
	m_doWork = false;
	m_conditionVariable.notify_all();
}

bool ConsoleLogger::isEmpty()
{
	unique_lock lg(m_mutex);
	return m_toWrite.empty();
}

ConsoleLogger::ConsoleLogger()
	: m_thread(&ConsoleLogger::worker, this)
{
	m_doWork = true;
}

void ConsoleLogger::worker()
{
	while(m_doWork) {
		unique_lock lg(m_mutex);
		m_conditionVariable.wait(lg, [this]() {
			return !m_toWrite.empty() || !m_doWork;
		});

		if (m_toWrite.empty()) {
			continue;
		}
		auto text = m_toWrite.front();
		m_toWrite.pop();

		lg.unlock();

		std::cout << text << '\n';
	}
}
