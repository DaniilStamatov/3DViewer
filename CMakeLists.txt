cmake_minimum_required(VERSION 3.5)

project(3d_viewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")

file(GLOB UI_SOURCES
    UI/*.cpp
)

file(GLOB OPENGL_SOURCES
    OpenGl/*.cpp
)
set(PROJECT_SOURCES
    ${UI_SOURCES}
    ${OPENGL_SOURCES}
    UI/ArrowButton.h
    main.cpp
    ModelLoader/Loader.cpp
    transformations/transformation.cpp
    Controller/ModelController.cpp
    vendor/stb_image.cpp
)


# Создание исполняемого файла
add_executable(3DViewer ${PROJECT_SOURCES})

target_include_directories(3DViewer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/UI
    ${CMAKE_CURRENT_SOURCE_DIR}/transformations
    ${CMAKE_CURRENT_SOURCE_DIR}/OpenGl
    ${CMAKE_CURRENT_SOURCE_DIR}/Camera
    ${CMAKE_CURRENT_SOURCE_DIR}/Controller
    ${CMAKE_CURRENT_SOURCE_DIR}/shader
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelLoader
)

target_link_libraries(3DViewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(3DViewer PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_link_libraries(3DViewer PRIVATE ${OPENGL_LIBRARIES})