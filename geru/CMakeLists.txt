cmake_minimum_required(VERSION 3.6)
project(geru)

cmake_policy(SET CMP0071 NEW)


set(SOURCE_FILES
        src/main.cpp
        src/BezierFrameGeometry.hpp
        src/BezierFrameGeometry.cpp
        src/CuboidWireframeGeometry.hpp
        src/CuboidWireframeGeometry.cpp
        src/ParticleModel.hpp
        src/ParticleModel.cpp
        src/QmlRaycast.hpp
        src/QmlRaycast.cpp
        src/CubeWalls.hpp
        src/CubeWalls.cpp
        src/simulation.hpp
        src/simulation.cpp

)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Qml Quick Charts 3DCore 3DRender 3DInput 3DLogic 3DExtras 3DAnimation REQUIRED)

qt5_add_resources(RESOURCES qrc/main.qrc)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADERS} ${RESOURCES})

qt5_use_modules(${PROJECT_NAME} Qml Quick Charts 3DCore 3DRender 3DInput 3DLogic 3DExtras 3DAnimation)
