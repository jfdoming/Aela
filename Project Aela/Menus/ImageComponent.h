/*
* Class: ImageComponent
* Author: Robert Ciborowski
* Date: 16/08/2017
* Description: A class which represents an image.
*/

#pragma once
#include "Component.h"
#include "../2D/Texture/Texture.h"

namespace Aela {
	class ImageComponent : public Component {
		public:
			ImageComponent();
			ImageComponent(Texture* texture);
			ImageComponent(Texture* texture, Rect<int>* dimensions);
			ImageComponent(Texture* texture, Rect<int>* dimensions, ColourRGBA* tint);
			virtual ~ImageComponent();

			virtual void update();
			virtual void render(Renderer* renderer);

			// These are getters and setters.
			void setTexture(Texture* texture);
			Texture* getTexture();
			void setTint(ColourRGBA* tint);
			ColourRGBA* getTint();

		protected:
			Texture* texture = nullptr;
			ColourRGBA tint;
	};
}