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
			virtual ~ImageComponent();

			virtual void update();
			virtual void render(Renderer* renderer);

			void setTexture(Texture* texture);
			Texture* getTexture();

		protected:
			Texture* texture = nullptr;
	};
}