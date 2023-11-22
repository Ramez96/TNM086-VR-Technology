CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
CMAKE_POLICY(VERSION 3.10)

# This feature is cumbersome but necessary for compatibility between
# versions of CMake
IF (POLICY CMP0072)
  CMAKE_POLICY(SET CMP0072 NEW)
ENDIF (POLICY CMP0072)

PROJECT(solution)

SET (APP_INCLUDE_DIRS CACHE INTERNAL "The list of include folders to search for the app.")
SET (APP_LIBS CACHE INTERNAL "The list of libraries that the app should link against.")

# Gramods dependencies
FIND_PACKAGE(Eigen3 REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
FIND_PACKAGE(SDL2 REQUIRED)
FIND_PACKAGE(tinyxml2 REQUIRED)
FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(FreeImage REQUIRED)
IF (WIN32)
  # Until ThinLinc is upgraded, OpenVR is only on Windows
  FIND_PACKAGE(OpenVR REQUIRED)
ENDIF()

# Build against Gramods
FIND_PACKAGE(Gramods REQUIRED gmCore gmMisc gmTrack gmGraphics gmNetwork)
LIST (APPEND APP_LIBS gmCore gmMisc gmTrack gmGraphics gmNetwork)

# Build against OpenSceneGraph
FIND_PACKAGE(OpenSceneGraph REQUIRED osgGA osgViewer osgDB osgUtil)
LIST (APPEND APP_INCLUDE_DIRS ${OPENSCENEGRAPH_INCLUDE_DIRS})
LIST (APPEND APP_LIBS ${OPENSCENEGRAPH_LIBRARIES})


ADD_EXECUTABLE(main main.cpp MyApp.cpp MyApp.hh)

TARGET_INCLUDE_DIRECTORIES(main PUBLIC ${APP_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(main ${APP_LIBS})
SET_PROPERTY(TARGET main PROPERTY CXX_STANDARD 17)
