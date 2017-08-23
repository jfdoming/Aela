/*
* Class: Resource
* Author: Julian Dominguez-Schatz
* Date: 01/01/2017
* Description: Represents a resource used in the application.
*/

#pragma once

#include <string>

namespace Aela {
	class Resource {
		public:
			Resource(std::string src);
			virtual ~Resource();

			// you shouldn't copy resources
			Resource(const Resource&) = delete;
			void operator=(const Resource&) = delete;

			std::string getSrc();
		private:
			std::string src;
	};
}