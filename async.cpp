#include "async.h"
#include "commandhandler.h"
#include "consolelogger.h"
#include "filelogger.h"

using namespace std;

std::map<async::handle, std::unique_ptr<Handler>> async::m_handlers;
std::mutex async::m_handlertsMutex;
DataKeeper async::m_keeper;

async::handle async::connect(std::size_t block_size)
{
	static int id = 0;
	unique_lock lg(m_handlertsMutex);
	m_handlers.emplace(++id, make_unique<Handler>(block_size));
	m_handlers[id]->addLogger(m_keeper.getConsoleLogger());
	m_handlers[id]->addLogger(m_keeper.getFileLogger());
	return id;
}

void async::receive(handle handle, const char *data, std::size_t size)
{
	HandlersIt it;
	{
		// Под мьютексом только поиск
		unique_lock lg(m_handlertsMutex);
		it = m_handlers.find(handle);
	}

	if (it == m_handlers.end()) {
		return;
	}
	it->second->input(string(data, size));
}

void async::disconnect(handle handle)
{
	HandlersIt it;
	{
		// Под мьютексом только поиск
		unique_lock lg(m_handlertsMutex);
		it = m_handlers.find(handle);
	}

	if (it == m_handlers.end()) {
		return;
	}
	m_handlers.erase(it);
}


std::weak_ptr<ConsoleLogger> DataKeeper::getConsoleLogger()
{
	return m_consoleLogger;
}

std::weak_ptr<FileLogger> DataKeeper::getFileLogger()
{
	return m_fileLogger;
}

DataKeeper::DataKeeper()
	: m_consoleLogger(new ConsoleLogger()),
	  m_fileLogger(new FileLogger())
{
}

DataKeeper::~DataKeeper()
{
	while (!m_consoleLogger->isEmpty()) {
		// Ждем, когда все допишем
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	while (!m_fileLogger->isEmpty()) {
		// Ждем, когда все допишем
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	m_consoleLogger->stop();
	m_fileLogger->stop();
}
