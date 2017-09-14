/*
* Class: ImageComponent
* Author: Robert Ciborowski
* Date: 16/08/2017
* Description: A class which represents an image.
*/

#pragma once
#include "Component.h"
#include "../2D/Texture/Texture.h"
#include "../2D/Transformable/Transformable2D.h"

namespace Aela {
	class ImageComponent : public Component, public Transformable2D {
		public:
			ImageComponent();
			ImageComponent(Texture* texture);
			ImageComponent(Texture* texture, Rect<int>* dimensions);
			ImageComponent(Texture* texture, Rect<int>* dimensions, ColourRGBA* tint);
			virtual ~ImageComponent();

			// These are getters and setters.
			void setTexture(Texture* texture);
			Texture* getTexture();

		protected:
			Texture* texture = nullptr;

			virtual void updateComponent();
			virtual void renderComponent(Renderer& renderer);
	};
}