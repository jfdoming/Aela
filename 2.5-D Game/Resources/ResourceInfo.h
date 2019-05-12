/*
* Name: ResourceInfo
* Author: Robert Ciborowski
* Date: 18/03/2018
* Description: Contains information about the resources that are specific to this game.
*/

#pragma once

// This is necessary because a lot of this game's custom loaders also need to obtain other resources
// (which requires knowing what the resource root is). Keep in mind that a ResourceLoader does
// not have access to the ResourceManager, so it must be able to know the resource root in some other
// way.
#define RESOURCE_ROOT (std::string) "../../../"
#define TILED_MAP_LOCATION (std::string) "res/tiled maps/"
#define SAVE_LOCATION (std::string) "/Documents/Neo-Zero/saves/"
#define BATTLE_LOCATION (std::string) "res/battles/"
#define MAP_BEING_EDITED (std::string) "stage_1"
#define MAP_BEING_EDITED_WORLD_ID 1