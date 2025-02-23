cmake_minimum_required(VERSION 3.5)

project(3d_viewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets OpenGLWidgets)

find_package(OpenGL REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(PROJECT_SOURCES
    UI/OpenGLWindow.cpp
    UI/OpenGLWindow.h
    UI/MainWindow.h
    UI/MainWindow.cpp
    UI/ArrowButton.h
    UI/ModelRenderer.h
    UI/ModelRenderer.cpp
    main.cpp
    loader.cpp
    model.h
    loader.h
    transformations/transformation.h
    transformations/transformation.cpp
    transformations/mat4.h
    shader/shader.h
    shader/shader.cpp
)

# Создание исполняемого файла
add_executable(3DViewer ${PROJECT_SOURCES})

target_link_libraries(3DViewer PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(3DViewer PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
target_link_libraries(3DViewer PRIVATE ${OPENGL_LIBRARIES})