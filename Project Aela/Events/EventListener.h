#pragma once

#include "Event.h"

#include <functional>

namespace Aela {
	typedef std::function<void(Event*)> EventListener;
}

#define bindListener(f, v) ({                   \
    auto fn = &f;                               \
    std::bind(fn, v, std::placeholders::_1);    \
})
