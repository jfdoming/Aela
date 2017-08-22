#include "Resource.h"

using namespace Aela;

Resource::Resource() {
}

Resource::~Resource() {
}

void Aela::Resource::setSource(std::string source) {
	this->source = source;
}

std::string Aela::Resource::getSource() {
	return source;
}
