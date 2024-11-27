cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(GraphServer)

# Establecer el estándar de C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Añadir los archivos fuente
add_executable(GraphServer main.cpp)

# Incluir directorios
include_directories(${PROJECT_SOURCE_DIR}/include)

# Añadir las bibliotecas necesarias
find_package(Threads REQUIRED)

# Enlazar las bibliotecas
target_link_libraries(GraphServer PRIVATE Threads::Threads)
