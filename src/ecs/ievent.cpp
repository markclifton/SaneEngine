#include "se/ecs/ievent.h"

EventHandler::EventHandler(EventManager* mgr)
	: mgr_(mgr)
{
	mgr_->registerHandler(this);
}

EventHandler::~EventHandler()
{
	mgr_->unregisterHandler(this);
}

bool EventHandler::handleEvent(IEvent* e)
{
	return false;
}

void EventManager::submit(IEvent e)
{
	std::lock_guard<std::mutex> lock(handlerMutex_);
	for (auto& handler : handlers_)
	{
		handler->handleEvent(&e);
	}
}

void EventManager::registerHandler(EventHandler* handler)
{
	if (handler != nullptr)
	{
		std::lock_guard<std::mutex> lock(handlerMutex_);
		handlers_.push_back(handler);
	}
}

void EventManager::unregisterHandler(EventHandler* handler)
{
	std::lock_guard<std::mutex> lock(handlerMutex_);
	for (int i = 0; i < handlers_.size(); i++)
	{
		if (handlers_[i] == handler)
		{
			handlers_[i] = handlers_.back();
			handlers_.pop_back();
			return;
		}
	}
}
