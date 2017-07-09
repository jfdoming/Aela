/*
* Class: Resource
* Author: Julian Dominguez-Schatz
* Date: 01/01/2017
* Description: Represents a resource used in the application.
*/

#pragma once

namespace Aela {
	class Resource {
		public:
			Resource();
			virtual ~Resource();

			Resource(const Resource&) = delete;
			void operator=(const Resource&) = delete;
	};
}