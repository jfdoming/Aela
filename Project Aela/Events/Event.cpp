#include "Event.h"

using namespace Aela;

Event::Event(int _type) {
	type = _type;
}

Event::~Event() {

}

int Event::getType() {
	return type;
}