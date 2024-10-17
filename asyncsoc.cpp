#include "asyncsoc.h"
#include "commandhandler.h"

using namespace std;

std::map<AsyncSoc::handle, std::unique_ptr<Handler>> AsyncSoc::m_handlers;
std::mutex AsyncSoc::m_handlertsMutex;

AsyncSoc::handle AsyncSoc::connect(std::size_t block_size)
{
	static int id = 0;
	lock_guard<mutex> lg(m_handlertsMutex);
	m_handlers.emplace(++id, make_unique<Handler>(block_size));
	return id;
}

void AsyncSoc::receive(handle handle, const char *data, std::size_t size)
{
	HandlersIt it;
	{
		// Под мьютексом только поиск
		lock_guard<mutex> lg(m_handlertsMutex);
		it = m_handlers.find(handle);
	}

	if (it == m_handlers.end()) {
		return;
	}
	it->second->input(string(data, size));
}

void AsyncSoc::disconnect(handle handle)
{
	HandlersIt it;
	{
		// Под мьютексом только поиск
		lock_guard<mutex> lg(m_handlertsMutex);
		it = m_handlers.find(handle);
	}

	if (it == m_handlers.end()) {
		return;
	}
	m_handlers.erase(it);
}
