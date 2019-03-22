#ifndef IEVENT_H
#define IEVENT_H

#include <mutex>
#include <vector>

#include "se/utils/dlldefines.h"

class EventManager;

class MYLIB_EXPORT IEvent
{
};

class MYLIB_EXPORT EventHandler
{
	EventManager* mgr_;
public:
	EventHandler(EventManager* mgr);
	~EventHandler();

	bool handleEvent(IEvent* e);
};

class MYLIB_EXPORT EventManager
{
	std::mutex handlerMutex_;
	std::vector<EventHandler*> handlers_;
public:
	EventManager() = default;
	~EventManager() = default;

	void submit(IEvent e);
	void registerHandler(EventHandler* handler);
	void unregisterHandler(EventHandler* handler);
};

#endif