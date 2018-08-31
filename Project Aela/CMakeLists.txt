cmake_minimum_required(VERSION 3.12)
project(Aela)

set(CMAKE_CXX_STANDARD 14)

include_directories(E:/Programming/Libraries/glm-0.9.7.1)
include_directories(E:/Programming/Libraries/glew-2.1.0/include)
include_directories("E:/Programming/Libraries/freetype 2.7.1/freetype-windows-binaries-master/include")
include_directories(E:/Programming/Libraries/SDL2-devel-2.0.9-mingw/x86_64-w64-mingw32/include/SDL2)
include_directories(E:/Programming/Libraries/libpng)
include_directories(E:/Programming/Libraries/lua-5.3.4-mingw/include)
include_directories("E:/Programming/Libraries/OpenAL 1.1 SDK/include")

include_directories(.)
include_directories(2D/Animation)
include_directories(2D/Renderer)
include_directories("2D/Simple 2D Framebuffer")
include_directories(2D/Text)
include_directories(2D/Texture)
include_directories(2D/Transformable)
include_directories(3D/Animation)
include_directories(3D/Billboards)
include_directories("3D/Bounding Box")
include_directories(3D/Camera)
include_directories(3D/Light)
include_directories(3D/Maps)
include_directories(3D/Materials)
include_directories(3D/Models)
include_directories(3D/Particles)
include_directories(3D/Renderer)
include_directories(3D/Skybox)
include_directories(3D/Transformable)
include_directories(Animation)
include_directories(Audio)
include_directories("Control Manager")
include_directories(Entities)
include_directories("Error Handler")
include_directories(Events)
include_directories("Framerate Calculator")
include_directories(Lua)
include_directories(Lua/LuaBridge)
include_directories(Lua/LuaBridge/detail)
include_directories(Menus)
include_directories("Old Garbage")
include_directories(Physics)
include_directories(Renderer)
include_directories("Resource Management")
include_directories(Scenes)
include_directories(Time)
include_directories(Time/Stopwatch)
include_directories(Transformable)
include_directories("User Environment")
include_directories(Utilities)
include_directories(Utilities/Colour)
include_directories(Utilities/Paths)
include_directories(Utilities/Pool)
include_directories(Utilities/Rect)
include_directories("Utilities/Texture Splitter")
include_directories(Utilities/Transformations)
include_directories(Window)

add_library(Aela
        2D/Animation/AnimationTrack2D.h
        2D/Animation/KeyFrame2D.cpp
        2D/Animation/KeyFrame2D.h
        2D/Renderer/Basic2DGLRenderer.cpp
        2D/Renderer/Basic2DGLRenderer.h
        2D/Renderer/Basic2DRenderer.cpp
        2D/Renderer/Basic2DRenderer.h
        "2D/Simple 2D Framebuffer/Simple2DFramebuffer.cpp"
        "2D/Simple 2D Framebuffer/Simple2DFramebuffer.h"
        "2D/Simple 2D Framebuffer/Simple2DGLFramebuffer.cpp"
        "2D/Simple 2D Framebuffer/Simple2DGLFramebuffer.h"
        2D/Fonts/Font.cpp
        2D/Fonts/Font.h
        2D/Fonts/Fontloader.cpp
        2D/Fonts/FontLoader.h
        2D/Texture/GLImage.h
        2D/Texture/GLSpriteSheetLoader.cpp
        2D/Texture/GLSpriteSheetLoader.h
        2D/Texture/GLTexture.h
        2D/Texture/GLTextureLoader.cpp
        2D/Texture/GLTextureLoader.h
        2D/Texture/Image.h
        2D/Texture/Texture.h
        2D/Transformable/Transformable2D.cpp
        2D/Transformable/Transformable2D.h
        3D/Animation/AnimationTrack3D.h
        3D/Animation/AnimationTrackMaterial.h
        3D/Animation/AnimationTrackModel.h
        3D/Animation/KeyedAnimator3D.cpp
        3D/Animation/KeyedAnimator3D.h
        3D/Animation/KeyFrame3D.cpp
        3D/Animation/KeyFrame3D.h
        3D/Animation/KeyFrameMaterial.cpp
        3D/Animation/KeyFrameMaterial.h
        3D/Animation/KeyFrameModel.cpp
        3D/Animation/KeyFrameModel.h
        3D/Billboards/BillboardEntity.h
        "3D/Bounding Box/BoundingBox3D.cpp"
        "3D/Bounding Box/BoundingBox3D.h"
        3D/Camera/Camera3D.cpp
        3D/Camera/Camera3D.h
        3D/Light/LightEntity.cpp
        3D/Light/LightEntity.h
        3D/Maps/Map3D.cpp
        3D/Maps/Map3D.h
        3D/Maps/Map3DExporter.cpp
        3D/Maps/Map3DExporter.h
        3D/Maps/Map3DLoader.cpp
        3D/Maps/Map3DLoader.h
        3D/Materials/GLMaterialLoader.cpp
        3D/Materials/GLMaterialLoader.h
        3D/Materials/Material.cpp
        3D/Materials/Material.h
        3D/Models/Model.cpp
        3D/Models/Model.h
        3D/Models/ModelEntity.cpp
        3D/Models/ModelEntity.h
        3D/Models/OBJLoader.cpp
        3D/Models/OBJLoader.h
        3D/Models/SubModel.cpp
        3D/Models/SubModel.h
        3D/Particles/GLParticleLoader.cpp
        3D/Particles/GLParticleLoader.h
        3D/Particles/Particle.cpp
        3D/Particles/Particle.h
        3D/Particles/ParticleEmitter.cpp
        3D/Particles/ParticleEmitter.h
        3D/Particles/PlanarParticleEmitter.cpp
        3D/Particles/PlanarParticleEmitter.h
        3D/Particles/PointParticleEmitter.cpp
        3D/Particles/PointParticleEmitter.h
        3D/Renderer/Basic3DGLModelRenderer.cpp
        3D/Renderer/Basic3DGLModelRenderer.h
        3D/Renderer/Basic3DGLRenderer.cpp
        3D/Renderer/Basic3DGLRenderer.h
        3D/Renderer/Basic3DGLShadowRenderer.cpp
        3D/Renderer/Basic3DGLShadowRenderer.h
        3D/Renderer/Basic3DGLSkyboxRenderer.cpp
        3D/Renderer/Basic3DGLSkyboxRenderer.h
        3D/Skybox/GLSkyboxLoader.cpp
        3D/Skybox/GLSkyboxLoader.h
        3D/Skybox/Skybox.cpp
        3D/Skybox/Skybox.h
        3D/Skybox/SkyboxEntity.cpp
        3D/Skybox/SkyboxEntity.h
        3D/Transformable/Transformable3D.cpp
        3D/Transformable/Transformable3D.h
        Animation/AnimationLooper.cpp
        Animation/AnimationLooper.h
        Animation/AnimationTrack.h
        Animation/Animator.cpp
        Animation/Animator.h
        Animation/KeyFrame.cpp
        Animation/KeyFrame.h
        Audio/AudioClip.cpp
        Audio/AudioClip.h
        Audio/AudioListener.cpp
        Audio/AudioListener.h
        Audio/AudioPlayer.cpp
        Audio/AudioPlayer.h
        Audio/WAVEClipLoader.cpp
        Audio/WAVEClipLoader.h
        "Control Manager/ControlManager.cpp"
        "Control Manager/ControlManager.h"
        Entities/Entity.cpp
        Entities/Entity.h
        "Error Handler/Error Handler/AelaErrorHandler.cpp"
        "Error Handler/Error Handler/AelaErrorHandler.h"
        "Error Handler/Simple Error/AelaSimpleError.cpp"
        "Error Handler/Simple Error/AelaSimpleError.h"
        "Error Handler/ErrorHandling.cpp"
        "Error Handler/ErrorHandling.h"
        Events/Event.cpp
        Events/Event.h
        Events/EventConstants.cpp
        Events/EventConstants.h
        Events/EventDispatcher.cpp
        Events/EventDispatcher.h
        Events/EventHandler.cpp
        Events/EventHandler.h
        Events/EventListener.h
        Events/EventListenerList.cpp
        Events/EventListenerList.h
        Events/KeyEvent.cpp
        Events/KeyEvent.h
        Events/MouseEvent.cpp
        Events/MouseEvent.h
        Events/WindowResizeEvent.cpp
        Events/WindowResizeEvent.h
        "Framerate Calculator/FramerateCalculator.cpp"
        "Framerate Calculator/FramerateCalculator.h"
        Lua/LuaBridge/detail/CFunctions.h
        Lua/LuaBridge/detail/ClassInfo.h
        Lua/LuaBridge/detail/Constructor.h
        Lua/LuaBridge/detail/dump.h
        Lua/LuaBridge/detail/FuncTraits.h
        Lua/LuaBridge/detail/Iterator.h
        Lua/LuaBridge/detail/LuaException.h
        Lua/LuaBridge/detail/LuaHelpers.h
        Lua/LuaBridge/detail/LuaRef.h
        Lua/LuaBridge/detail/Namespace.h
        Lua/LuaBridge/detail/Stack.h
        Lua/LuaBridge/detail/TypeList.h
        Lua/LuaBridge/detail/TypeTraits.h
        Lua/LuaBridge/detail/Userdata.h
        Lua/LuaBridge/LuaBridge.h
        Lua/LuaBridge/RefCountedObject.h
        Lua/LuaBridge/RefCountedPtr.h
        Lua/Exposable.h
        Lua/LuaManager.cpp
        Lua/LuaManager.h
        Lua/LuaScript.cpp
        Lua/LuaScript.h
        Lua/Mod.cpp
        Lua/Mod.h
        Lua/ModEnvironment.cpp
        Lua/ModEnvironment.h
        Lua/ModManager.cpp
        Lua/ModManager.h
        Menus/AbstractLayoutManager.cpp
        Menus/AbstractLayoutManager.h
        Menus/Button.cpp
        Menus/Button.h
        Menus/Component.cpp
        Menus/Component.h
        Menus/Container.cpp
        Menus/Container.h
        Menus/ImageComponent.cpp
        Menus/ImageComponent.h
        Menus/Label.cpp
        Menus/Label.h
        Menus/LAF.cpp
        Menus/LAF.h
        Menus/LayoutManager.h
        Menus/Menu.cpp
        Menus/Menu.h
        Menus/RectComponent.cpp
        Menus/RectComponent.h
        Menus/stdafx.h
        Menus/SubMenu.cpp
        Menus/SubMenu.h
        Menus/TextInput.cpp
        Menus/TextInput.h
        "Old Garbage/shader.cpp"
        "Old Garbage/shader.hpp"
        "Old Garbage/texture.cpp"
        "Old Garbage/texture.hpp"
        Physics/PhysicsManager.cpp
        Physics/PhysicsManager.h
        Renderer/GLRenderer.cpp
        Renderer/GLRenderer.h
        Renderer/Renderer.cpp
        Renderer/Renderer.h
        "Resource Management/Resource.cpp"
        "Resource Management/Resource.h"
        "Resource Management/ResourceGroup.cpp"
        "Resource Management/ResourceGroup.h"
        "Resource Management/ResourceLoader.cpp"
        "Resource Management/ResourceLoader.h"
        "Resource Management/ResourceManager.cpp"
        "Resource Management/ResourceManager.h"
        "Resource Management/ResourceMap.cpp"
        "Resource Management/ResourceMap.h"
        "Resource Management/ResourcePaths.h"
        "Resource Management/ResourceQuery.cpp"
        "Resource Management/ResourceQuery.h"
        "Resource Management/stdafx.h"
        Scenes/Scene.cpp
        Scenes/Scene.h
        Scenes/SceneManager.cpp
        Scenes/SceneManager.h
        Scenes/stdafx.h
        Time/Stopwatch/Stopwatch.cpp
        Time/Stopwatch/Stopwatch.h
        Time/Timer/Timer.cpp
        Time/Timer/Timer.h
        Time/Clock.h
        Transformable/Transformable.cpp
        Transformable/Transformable.h
        "User Environment/UserEnvironment.cpp"
        "User Environment/UserEnvironment.h"
        Utilities/Colour/ColourRGB.h
        Utilities/Colour/ColourRGBA.h
        Utilities/Paths/PathSegment.h
        Utilities/Paths/PathSegment2D.h
        Utilities/Paths/PathSegment3D.h
        Utilities/Pool/Pool.h
        Utilities/Rect/Cuboid.h
        Utilities/Rect/Rect.h
        "Utilities/Texture Splitter/GLTextureSplitter.cpp"
        "Utilities/Texture Splitter/GLTextureSplitter.h"
        Utilities/Transformations/PointRotation3D.cpp
        Utilities/Transformations/PointRotation3D.h
        Utilities/enumut.h
        Utilities/flut.h
        Utilities/glmut.h
        Utilities/strut.h
        Window/Window.cpp
        Window/Window.h
        Aela_Engine.cpp
        Aela_Engine.h
        stdafx.h
        targetver.h)

# GLFW
find_package(OpenGL REQUIRED)

target_link_libraries(Aela PRIVATE "E:/Programming/Libraries/libpng-1.6.35-mingw/lib/libpng.dll.a")
target_link_libraries(Aela PRIVATE "E:/Programming/Libraries/freetype 2.7.1/freetype-windows-binaries-master/win64/freetype271.lib")
target_link_libraries(Aela PRIVATE "E:/Programming/Libraries/OpenAL 1.1 SDK/libs/Win64/OpenAl32.lib")
target_link_libraries(Aela PUBLIC "E:/Programming/Libraries/lua-5.3.4-mingw/liblua53.a")

# the statements below must be in the below very specific order, otherwise they won't compile
target_link_libraries(Aela PRIVATE "E:/Programming/Libraries/glew-2.1.0-mingw/lib/libglew32.a")
target_link_libraries(Aela PRIVATE ${OPENGL_gl_LIBRARY})
target_link_libraries(Aela PRIVATE "E:/Programming/Libraries/SDL2-devel-2.0.9-mingw/x86_64-w64-mingw32/lib/libSDL2main.a")
target_link_libraries(Aela PRIVATE "E:/Programming/Libraries/SDL2-devel-2.0.9-mingw/x86_64-w64-mingw32/lib/libSDL2.a")

# required to link SDL2, see here: https://stackoverflow.com/questions/27408853/cmake-failing-to-statically-link-sdl2
target_link_libraries(Aela PRIVATE m)
target_link_libraries(Aela PRIVATE dinput8)
target_link_libraries(Aela PRIVATE dxguid)
target_link_libraries(Aela PRIVATE dxerr8)
target_link_libraries(Aela PRIVATE user32)
target_link_libraries(Aela PRIVATE gdi32)
target_link_libraries(Aela PRIVATE winmm)
target_link_libraries(Aela PRIVATE imm32)
target_link_libraries(Aela PRIVATE ole32)
target_link_libraries(Aela PRIVATE oleaut32)
target_link_libraries(Aela PRIVATE shell32)
target_link_libraries(Aela PRIVATE version)
target_link_libraries(Aela PRIVATE uuid)
