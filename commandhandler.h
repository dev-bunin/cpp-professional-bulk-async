#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <string>
#include <queue>
#include <chrono>
#include <memory>
#include <iostream>

class LoggerInterface;

class QueueInterface {
public:
	virtual ~QueueInterface()  = default;

	virtual bool isFinished() = 0;
	virtual void addCommand(const std::string &command) = 0;
	virtual bool check() = 0;
	virtual bool finished() = 0;
};

class QueueBase : public QueueInterface {
	using Time = std::chrono::time_point<std::chrono::system_clock>;
	using Loggers = std::vector<std::weak_ptr<LoggerInterface>>;
public:
	QueueBase();
	virtual bool isFinished() override;
	virtual void addCommand(const std::string &command) override;
	void clear();

	virtual std::string getCommands();
	Time getStratTime();

protected:
	std::vector<std::string> m_commands;
	Time m_startTime;

	void saveToFile(const std::string &result);
};

class DynamicQueue : public QueueBase {
public:
	DynamicQueue();

	bool isFinished() override;
	void addCommand(const std::string &command) override;
	bool check() override;
	bool finished() override;

private:
	int m_deep = 0;
};

class LimitedQueue : public QueueBase {
public:
	LimitedQueue(size_t maxCommandCount);

	void addCommand(const std::string &command) override;
	bool check() override;
	bool finished() override;

private:
	size_t m_maxCommandCount;
};

class Handler {
public:
	Handler();
	Handler(size_t blockSize);
	~Handler();

	void input(const std::string &data);
	void addLogger(std::weak_ptr<LoggerInterface> logger);

private:
	void addCommand(const std::string &command);
	void doCommand();

	enum CommandType {
		None,
		Queue,
		Limited
	};

	std::vector<std::weak_ptr<LoggerInterface>> m_loggers;
	std::unique_ptr<QueueBase> m_command;
	CommandType m_type = CommandType::None;
	size_t m_blockSize;
};

void parseCommand(const int maxCommandCount, std::istream &istream);

#endif // COMMANDHANDLER_H
