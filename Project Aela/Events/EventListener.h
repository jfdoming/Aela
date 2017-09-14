#pragma once

#include "Event.h"

#include <functional>

namespace Aela {
	typedef std::function<void(Event*)> EventListener;
}

#define bindListener(f, v) std::bind(&f, v, std::placeholders::_1)
