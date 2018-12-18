cmake_minimum_required(VERSION 3.6)
project(kata)

cmake_policy(SET CMP0071 NEW)

file(GLOB_RECURSE HEADERS "include/*.hpp")
file(GLOB_RECURSE SOURCE_FILES "src/*.[hc]pp")

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Qml Quick Charts 3DCore 3DRender 3DInput 3DLogic 3DExtras 3DAnimation REQUIRED)

qt5_add_resources(RESOURCES qrc/main.qrc)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADERS} ${RESOURCES})

qt5_use_modules(${PROJECT_NAME} Qml Quick Charts 3DCore 3DRender 3DInput 3DLogic 3DExtras 3DAnimation)
