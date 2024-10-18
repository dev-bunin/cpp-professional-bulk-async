#ifndef ASYNCSOC_H
#define ASYNCSOC_H

#include <map>
#include <memory>
#include <mutex>

class FileLogger;
class ConsoleLogger;
class Handler;
class async;

class DataKeeper
{
	std::weak_ptr<ConsoleLogger> getConsoleLogger();
	std::weak_ptr<FileLogger> getFileLogger();

private:
	// чтоб никто левый не создал
	friend class async;
	DataKeeper();
	~DataKeeper();

	std::shared_ptr<ConsoleLogger> m_consoleLogger;
	std::shared_ptr<FileLogger> m_fileLogger;
};


class async {
public:
	using handle = std::size_t;

	static handle connect(std::size_t block_size);
	static void receive(handle handle, const char *data, std::size_t size);
	static void disconnect(handle handle);

private:
	using HandlersMap = std::map<handle, std::unique_ptr<Handler>>;
	using HandlersIt = HandlersMap::iterator;
	// Для автоматического управления временем жизни логгеров
	static DataKeeper m_keeper;
	static HandlersMap m_handlers;
	static std::mutex m_handlertsMutex;
};

#endif // ASYNCSOC_H
