#ifndef NONCOPYABLE_H
#define NONCOPYABLE_H

#include "se/utils/dlldefines.h"

class NonCopyable
{
public:
	NonCopyable() = default;
	~NonCopyable() = default;
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif