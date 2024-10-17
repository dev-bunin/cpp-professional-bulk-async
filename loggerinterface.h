#ifndef LOGGERINTERFACE_H
#define LOGGERINTERFACE_H

class QueueBase;

class LoggerInterface {
public:
	virtual void write(QueueBase &command) = 0;
};

#endif // LOGGERINTERFACE_H
