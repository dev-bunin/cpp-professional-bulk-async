#ifndef ASYNCSOC_H
#define ASYNCSOC_H

#include <map>
#include <memory>
#include <mutex>

class Handler;

class AsyncSoc
{
public:
	using handle = std::size_t;

	static handle connect(std::size_t block_size);

	static void receive(handle handle, const char *data, std::size_t size);

	static void disconnect(handle handle);

private:
	using HandlersMap = std::map<handle, std::unique_ptr<Handler>>;
	using HandlersIt = HandlersMap::iterator;

	static HandlersMap m_handlers;
	static std::mutex m_handlertsMutex;
};

#endif // ASYNCSOC_H
