#include "commandhandler.h"
#include "consolelogger.h"

#include <memory>
#include <memory>
#include <iostream>
#include <fstream>

using namespace std;

QueueBase::QueueBase()
{
}

bool QueueBase::isFinished()
{
	return m_commands.empty();
}

void QueueBase::addCommand(const std::string &command)
{
	m_commands.push_back(command);
}

void QueueBase::clear()
{
	m_commands.clear();
}

string QueueBase::getCommands()
{
	bool firstIt = true;
	string result;
	for (auto it = m_commands.begin(); it != m_commands.end(); ++it) {
		if (!firstIt) {
			result += ", ";
		}
		firstIt = false;
		result += *it;
	}
	return result;
}

QueueBase::Time QueueBase::getStratTime()
{
	return m_startTime;
}

void QueueBase::saveToFile(const std::string &result)
{
	using namespace chrono;
	std::ofstream outFile;
	string fileName(to_string(duration_cast<seconds>
							  (m_startTime.time_since_epoch()).count()) + ".log");
	outFile.open(fileName);
	if (outFile.is_open()) {
		outFile << result;
	}
	outFile.close();
}

LimitedQueue::LimitedQueue(size_t maxCommandCount)
	: m_maxCommandCount(maxCommandCount)
{
}

void LimitedQueue::addCommand(const std::string &command)
{
	if (m_commands.empty()) {
		m_startTime = chrono::system_clock::now();
	}
	QueueBase::addCommand(command);
}

bool LimitedQueue::check()
{
	return true;
}

bool LimitedQueue::finished()
{
	return m_maxCommandCount <= m_commands.size();
}

DynamicQueue::DynamicQueue()
{
	// Фиксируем время при создании
	m_startTime = chrono::system_clock::now();
}

bool DynamicQueue::isFinished()
{
	return (m_deep == 0) && QueueBase::isFinished();
}

void DynamicQueue::addCommand(const std::string &command)
{
	if (command == "{") {
		// Вложенные блоки игнорируется, но надо отслеживать что все они завершены
		++m_deep;
		return;
	}
	if (command == "}") {
		--m_deep;

		return;
	}
	QueueBase::addCommand(command);
}

bool DynamicQueue::check()
{
	return m_deep == 0;
}

bool DynamicQueue::finished()
{
	return m_deep == 0;
}

void parseCommand(const int maxCommandCount, std::istream &istream)
{

	Handler handler(maxCommandCount);

	string line;
	while(getline(istream, line)) {
		if (line.empty()) {
			break;
		}
		handler.input(line);
	}
}

Handler::Handler(size_t blockSize)
	:m_blockSize(blockSize)
{
}

Handler::~Handler()
{
	if (m_command) {
		doCommand();
	}
}

void Handler::input(const std::string &data)
{
	if (data.empty()) {
		return;
	} else if (data == "{") {
		switch (m_type) {
		case CommandType::Limited:
			doCommand();
			//FALLTHROUGH

		case CommandType::None:
			m_type = CommandType::Queue;
			m_command.reset(new DynamicQueue());
			break;

		case CommandType::Queue:
			// Ничего не делаем
			break;
		}
	} else if (data == "}") {
		if (m_type != CommandType::Queue) {
			// Что то пошло не так.
			// ToDo Обработка ошибки
			return;
		}
	} else {
		if (!m_command) {
			m_type = CommandType::Limited;
			m_command.reset(new LimitedQueue(m_blockSize));
		}
	}

	addCommand(data);

	// Если очередь команд завершилась, удаляем
	if (m_command && m_command->isFinished()) {
		m_type = CommandType::None;
		m_command.reset();
	}
}

void Handler::addLogger(std::weak_ptr<LoggerInterface> logger)
{
	m_loggers.push_back(logger);
}

void Handler::addCommand(const std::string &command)
{
	m_command->addCommand(command);
	if (m_command->finished()) {
		doCommand();
	}
}

void Handler::doCommand()
{
	if (!m_command->check()) {
		return;
	}
	for (auto &logger : m_loggers) {
		logger.lock()->write(*m_command);
	}
	m_command->clear();
}
