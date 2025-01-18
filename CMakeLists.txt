# CMakeList.txt : projet CMake pour DataMorph, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

project("DataMorph")

set(MAIN_FILE "main.cpp")
set(PROJECT_SOURCES 
		src/app.cpp
)
set(DEPENDENCIES_SOURCES dependencies/src/glad/glad.c)

file(GLOB LIBS_LIB "dependencies/libs/*.lib")

# Ajoutez une source à l'exécutable de ce projet.
add_executable(DataMorph ${MAIN_FILE} ${PROJECT_SOURCES} ${DEPENDENCIES_SOURCES})

target_include_directories(DataMorph PUBLIC include/)
target_include_directories(DataMorph PRIVATE dependencies/include)

target_link_libraries(DataMorph PRIVATE ${LIBS_LIB})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DataMorph PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
