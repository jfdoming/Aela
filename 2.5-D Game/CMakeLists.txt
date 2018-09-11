cmake_minimum_required(VERSION 3.10)
project(2_5_D_Game)

set(CMAKE_CXX_STANDARD 14)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
endif()

# libraries
include_directories(C:/Users/Robert/Documents/CLion/Libraries/glm-0.9.7.1)
include_directories(C:/Users/Robert/Documents/CLion/Libraries/glew-2.1.0/include)
include_directories("C:/Users/Robert/Documents/CLion/Libraries/freetype 2.7.1/freetype-windows-binaries-master/include")
include_directories(C:/Users/Robert/Documents/CLion/Libraries/SDL2-devel-2.0.9-mingw/x86_64-w64-mingw32/include/SDL2)
include_directories(C:/Users/Robert/Documents/CLion/Libraries/libpng-1.6.35/include)
include_directories(C:/Users/Robert/Documents/CLion/Libraries/lua-5.3.4-mingw/include)
include_directories("C:/Users/Robert/Documents/CLion/Libraries/OpenAL 1.1 SDK/include")

# in-house libraries
include_directories("../Project Aela")
include_directories("../Project Aela/Time")

include_directories(.)
include_directories("Aela Game")
include_directories(Bullet)
include_directories(Camera)
include_directories(Character)
include_directories(Chunks)
include_directories(Displays)
include_directories(Displays/Dialogue)
include_directories(Displays/Items)
include_directories(Displays/Tiles)
include_directories(Elevator)
include_directories(Enemies)
include_directories("Game Mode")
include_directories("Game Object Provider")
include_directories(Items)
include_directories(Location)
include_directories(Main)
include_directories(Movement)
include_directories(Particles)
include_directories(Player)
include_directories(Resources)
include_directories(Scripts)
include_directories("Scripts/Scripts to Move to LUA")
include_directories(Teleporter)
include_directories(Tiles)
include_directories(Utilities)
include_directories(Worlds)

add_executable(2_5_D_Game
        "Aela Game/AelaGame.cpp"
        "Aela Game/AelaGame.h"
        Bullet/Bullet.cpp
        Bullet/Bullet.h
        Camera/CameraController.cpp
        Camera/CameraController.h
        Camera/CameraMode.h
        Character/Character.cpp
        Character/Character.h
        Character/CharacterModelGenerator.cpp
        Character/CharacterModelGenerator.h
        Character/CharacterProvider.cpp
        Character/CharacterProvider.h
        Character/CharacterStep.h
        Chunks/Chunk.cpp
        Chunks/Chunk.h
        Displays/Dialogue/DialogueDisplay.cpp
        Displays/Dialogue/DialogueDisplay.h
        Displays/Dialogue/DialogueOption.h
        Displays/Hints/HintDisplay.cpp
        Displays/Hints/HintDisplay.h
        Displays/Hints/HintDisplayDuration.h
        Displays/Items/ItemInventoryDisplay.cpp
        Displays/Items/ItemInventoryDisplay.h
        Displays/Tiles/TileInventoryDisplay.cpp
        Displays/Tiles/TileInventoryDisplay.h
        Doors/Door.cpp
        Doors/Door.h
        Doors/DoorProvider.cpp
        Doors/DoorProvider.h
        Doors/Lock.cpp
        Doors/Lock.h
        Doors/TileAmountLock.cpp
        Doors/TileAmountLock.h
        Elevator/Elevator.cpp
        Elevator/Elevator.h
        Enemies/Enemy.cpp
        Enemies/Enemy.h
        Enemies/EnemyProvider.cpp
        Enemies/EnemyProvider.h
        Enemies/Turret.cpp
        Enemies/Turret.h
        "Game Mode/GameMode.h"
        "Game Object Provider/GameObjectProvider.cpp"
        "Game Object Provider/GameObjectProvider.h"
        Guns/TileSwitchGun.cpp
        Guns/TileSwitchGun.h
        Items/Item.cpp
        Items/Item.h
        Items/ItemAtlas.cpp
        Items/ItemAtlas.h
        Items/ItemInventory.cpp
        Items/ItemInventory.h
        Items/ItemType.cpp
        Items/ItemType.h
        Location/Location.cpp
        Location/Location.h
        Main/main.cpp
        Menus/FontSizes.h
        Movement/Movement.cpp
        Movement/Movement.h
        Movement/TeleportationAnimation.h
        Particles/BulletParticleEmitter.cpp
        Particles/BulletParticleEmitter.h
        Particles/CharacterDeathParticleEmitter.cpp
        Particles/CharacterDeathParticleEmitter.h
        Particles/CharacterTeleportParticleEmitter.cpp
        Particles/CharacterTeleportParticleEmitter.h
        Particles/LaserParticleEmitter.cpp
        Particles/LaserParticleEmitter.h
        Particles/TileSwitchParticleEmitter.cpp
        Particles/TileSwitchParticleEmitter.h
        Player/Player.cpp
        Player/Player.h
        Resources/GenericMaterialLoader.cpp
        Resources/GenericMaterialLoader.h
        Resources/ResourceInfo.h
        "Save States/GameSaver.cpp"
        "Save States/GameSaver.h"
        "Save States/Saveable.cpp"
        "Save States/Saveable.h"
        "Save States/SaveState.cpp"
        "Save States/SaveState.h"
        "Scripts/Scripts to Move to LUA/AnimationScript.cpp"
        "Scripts/Scripts to Move to LUA/AnimationScript.h"
        "Scripts/Scripts to Move to LUA/GameScript.cpp"
        "Scripts/Scripts to Move to LUA/GameScript.h"
        "Scripts/Scripts to Move to LUA/MapScript.cpp"
        "Scripts/Scripts to Move to LUA/MapScript.h"
        "Scripts/Scripts to Move to LUA/ResourceScript.cpp"
        "Scripts/Scripts to Move to LUA/ResourceScript.h"
        "Scripts/Scripts to Move to LUA/SceneScript.cpp"
        "Scripts/Scripts to Move to LUA/SceneScript.h"
        "Scripts/Scripts to Move to LUA/ScriptObjects.cpp"
        "Scripts/Scripts to Move to LUA/ScriptObjects.h"
        Scripts/ScriptManager.cpp
        Scripts/ScriptManager.h
        Teleporter/Teleporter.cpp
        Teleporter/Teleporter.h
        Tiles/Tile.cpp
        Tiles/Tile.h
        Tiles/TileAtlas.cpp
        Tiles/TileAtlas.h
        Tiles/TileAtlasLoader.cpp
        Tiles/TileAtlasLoader.h
        Tiles/TileBehaviourExecutor.cpp
        Tiles/TileBehaviourExecutor.h
        Tiles/TileGroup.cpp
        Tiles/TileGroup.h
        Tiles/TileInventory.cpp
        Tiles/TileInventory.h
        Tiles/TileLoader.cpp
        Tiles/TileLoader.h
        Tiles/TileType.cpp
        Tiles/TileType.h
        Utilities/GameConstants.h
        Utilities/MathConstants.h
        Worlds/MapRebuilder.cpp
        Worlds/MapRebuilder.h
        Worlds/World.cpp
        Worlds/World.h
        Worlds/WorldExporter.cpp
        Worlds/WorldExporter.h
        Worlds/WorldLoader.cpp
        Worlds/WorldLoader.h
        Worlds/WorldManager.cpp
        Worlds/WorldManager.h
        Tiles/TileBehaviourExecutor.cpp)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(2_5_D_Game "C:/Users/Robert/Documents/CLion/Projects/Project Aela/Project Aela/build/bin/debug/x64/libAela.a")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    target_link_libraries(2_5_D_Game "C:/Users/Robert/Documents/CLion/Projects/Project Aela/Project Aela/build/bin/release/x64/libAela.a")
else ()
    message(WARNING "Unknown build type!")
endif ()

# GLFW
find_package(OpenGL REQUIRED)

target_link_libraries(2_5_D_Game "C:/Users/Robert/Documents/CLion/Libraries/libpng-1.6.35/lib/libpng16.dll.a")
target_link_libraries(2_5_D_Game "C:/Users/Robert/Documents/CLion/Libraries/freetype 2.7.1/freetype-windows-binaries-master/win64/freetype271.lib")
target_link_libraries(2_5_D_Game "C:/Users/Robert/Documents/CLion/Libraries/OpenAL 1.1 SDK/libs/Win64/OpenAl32.lib")
target_link_libraries(2_5_D_Game "C:/Users/Robert/Documents/CLion/Libraries/lua-5.3.4-mingw/liblua53.a")

# the statements below must be in the below very specific order, otherwise they won't compile
target_link_libraries(2_5_D_Game "C:/Users/Robert/Documents/CLion/Libraries/glew-2.1.0/lib/libglew32.a")
target_link_libraries(2_5_D_Game ${OPENGL_gl_LIBRARY})
target_link_libraries(2_5_D_Game "C:/Users/Robert/Documents/CLion/Libraries/SDL2-devel-2.0.9-mingw/x86_64-w64-mingw32/lib/libSDL2main.a")
target_link_libraries(2_5_D_Game "C:/Users/Robert/Documents/CLion/Libraries/SDL2-devel-2.0.9-mingw/x86_64-w64-mingw32/lib/libSDL2.a")

# required to link SDL2, see here: https://stackoverflow.com/questions/27408853/cmake-failing-to-statically-link-sdl2
target_link_libraries(2_5_D_Game m)
target_link_libraries(2_5_D_Game dinput8)
target_link_libraries(2_5_D_Game dxguid)
target_link_libraries(2_5_D_Game dxerr8)
target_link_libraries(2_5_D_Game user32)
target_link_libraries(2_5_D_Game gdi32)
target_link_libraries(2_5_D_Game winmm)
target_link_libraries(2_5_D_Game imm32)
target_link_libraries(2_5_D_Game ole32)
target_link_libraries(2_5_D_Game oleaut32)
target_link_libraries(2_5_D_Game shell32)
target_link_libraries(2_5_D_Game version)
target_link_libraries(2_5_D_Game uuid)
