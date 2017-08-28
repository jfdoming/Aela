#include "Transformable2D.h"

void Transformable2D::setTint(ColourRGBA* tint) {
	this->tint = *tint;
}

ColourRGBA* Transformable2D::getTint() {
	return &tint;
}

void Transformable2D::setProperty(Transformable2DProperty property, float value) {
	switch (property) {
		case Transformable2DProperty::TINT_R:
			tint.setR(value);
			break;
		case Transformable2DProperty::TINT_G:
			tint.setG(value);
			break;
		case Transformable2DProperty::TINT_B:
			tint.setB(value);
			break;
		case Transformable2DProperty::TINT_A:
			tint.setA(value);
			break;
	}
}

float Transformable2D::getProperty(Transformable2DProperty property) {
	return 0.0f;
}
