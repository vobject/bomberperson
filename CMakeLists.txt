CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(BomberPerson)

SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

IF (NOT CMAKE_BUILD_TYPE)
    MESSAGE(STATUS "No build type selected. Default to RELEASE")
    SET(CMAKE_BUILD_TYPE "RELEASE")
ENDIF()

OPTION(USE_OPENGL "Use OpenGL for rendering." ON)
OPTION(USE_OGRE "Use the Ogre3D library for rendering." ON)

# TODO: Check if USE_OPENNI has been set and set it for the compilation.

# TODO: Check compiler version to be at least g++4.7 or clang++3.1.

FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(SDL_image REQUIRED)
FIND_PACKAGE(SDL_gfx REQUIRED)
FIND_PACKAGE(SDL_ttf REQUIRED)
FIND_PACKAGE(SDL_mixer REQUIRED)
FIND_PACKAGE(X11 REQUIRED) # OpenGL should already have taken care of this...
# TODO: Check for OpenNI
#TODO: OpenNI Stuff: ADD_DEFINITIONS(-I/usr/include/ni)
#TODO: OpenNI Stuff: ADD_DEFINITIONS(-Dlinux)

INCLUDE_DIRECTORIES(
   ${SDL_INCLUDE_DIR}
   ${SDLIMAGE_INCLUDE_DIR}
   ${SDLGFX_INCLUDE_DIR}
   ${SDLTTF_INCLUDE_DIR}
   ${SDLMIXER_INCLUDE_DIR}
   ${X11_INCLUDE_DIR}
)

FILE(GLOB BOMBERPERSON_HEADERS src/BomberPersonApp.hpp
                               src/WindowFrame.hpp
                               src/Options.hpp
                               src/audio/*.hpp
                               src/game/*.hpp
                               src/input/*.hpp
                               src/nui/*.hpp
                               src/render/*.hpp
                               src/render/SimpleSdlRenderer/*.hpp
                               src/render/SdlRenderer/*.hpp
                               src/render/RaycastRenderer/*.hpp
                               src/utils/*.hpp)

FILE(GLOB BOMBERPERSON_SOURCES src/main.cpp
                               src/BomberPersonApp.cpp
                               src/WindowFrame.cpp
                               src/audio/*.cpp
                               src/game/*.cpp
                               src/input/*.cpp
                               src/nui/*.cpp
                               src/render/*.cpp
                               src/render/SimpleSdlRenderer/*.cpp
                               src/render/SdlRenderer/*.cpp
                               src/render/RaycastRenderer/*.cpp)

ADD_DEFINITIONS(
   -Wall
   -std=c++11
)

SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3")

IF (USE_OPENGL)
    MESSAGE(STATUS "Using OpenGL.")

    FIND_PACKAGE(OpenGL REQUIRED)
    INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})
    ADD_DEFINITIONS(-DUSE_OPENGL)

    FILE(GLOB GL_RENDERER_HEADERS src/render/SimpleGlRenderer/*.hpp)
    FILE(GLOB GL_RENDERER_SOURCES src/render/SimpleGlRenderer/*.cpp)

    SET(BOMBERPERSON_HEADERS ${BOMBERPERSON_HEADERS} ${GL_RENDERER_HEADERS})
    SET(BOMBERPERSON_SOURCES ${BOMBERPERSON_SOURCES} ${GL_RENDERER_SOURCES})

    SET(EXTRA_LIBS ${EXTRA_LIBS} ${OPENGL_LIBRARIES})
ELSE (USE_OPENGL)
    MESSAGE(STATUS "Not using OpenGL.")
ENDIF (USE_OPENGL)

IF (USE_OGRE)
    MESSAGE(STATUS "Using Ogre3D library.")

    FIND_PACKAGE(OGRE REQUIRED)
    FIND_PACKAGE(Boost REQUIRED) # boost is currently only used for Ogre3D.

    INCLUDE_DIRECTORIES(${OGRE_INCLUDE_DIRS})
    INCLUDE_DIRECTORIES(${BOOST_INCLUDE_DIRS})

    # Enable Ogre3D code in the source files.
    ADD_DEFINITIONS(-DUSE_OGRE)

    FILE(GLOB OGRE_RENDERER_HEADERS src/render/Ogre3DRenderer/*.hpp)
    FILE(GLOB OGRE_RENDERER_SOURCES src/render/Ogre3DRenderer/*.cpp)

    SET(BOMBERPERSON_HEADERS ${BOMBERPERSON_HEADERS} ${OGRE_RENDERER_HEADERS})
    SET(BOMBERPERSON_SOURCES ${BOMBERPERSON_SOURCES} ${OGRE_RENDERER_SOURCES})

    SET(EXTRA_LIBS ${EXTRA_LIBS} ${OGRE_LIBRARIES})
#    SET(EXTRA_LIBS ${EXTRA_LIBS} ${BOOST_LIBRARIES})
    SET(EXTRA_LIBS ${EXTRA_LIBS} boost_system) # HACK
ELSE (USE_OGRE)
    MESSAGE(STATUS "Not using Ogre3D library.")
ENDIF (USE_OGRE)

ADD_EXECUTABLE(BomberPerson ${BOMBERPERSON_HEADERS} ${BOMBERPERSON_SOURCES})

TARGET_LINK_LIBRARIES(
   ${PROJECT_NAME}
   ${SDL_LIBRARY}
   ${SDLIMAGE_LIBRARY}
   ${SDLGFX_LIBRARY}
   ${SDLTTF_LIBRARY}
   ${SDLMIXER_LIBRARY}
   ${X11_LIBRARIES}
   ${EXTRA_LIBS}
)
