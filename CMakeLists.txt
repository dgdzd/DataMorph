# CMakeList.txt : projet CMake pour DataMorph, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

project("DataMorph")

add_compile_options("/bigobj")

set(MAIN_FILE "main.cpp")
set(PROJECT_SOURCES 
		src/App.cpp
		src/core/imgui_extension.cpp
		src/Constants.cpp
		src/gui/MainWindow.cpp
		src/gui/UpdateWindow.cpp
		src/gui/NewProjectWindow.cpp
		src/gui/GraphWindow.cpp
		src/FontManager.cpp
		src/Utils.cpp
		src/core/Project.cpp
		src/core/Header.cpp
		src/core/Expression.cpp
)
set(DEPENDENCIES_SOURCES 
		dependencies/src/glad/glad.c
		dependencies/src/imgui/imgui.cpp
		dependencies/src/imgui/imgui_demo.cpp
		dependencies/src/imgui/imgui_draw.cpp
		dependencies/src/imgui/imgui_impl_glfw.cpp
		dependencies/src/imgui/imgui_impl_opengl3.cpp
		dependencies/src/imgui/imgui_tables.cpp
		dependencies/src/imgui/imgui_widgets.cpp
		dependencies/include/imgui/misc/freetype/imgui_freetype.cpp
		dependencies/src/imgui/implot.cpp
		dependencies/src/imgui/implot_demo.cpp
		dependencies/src/imgui/implot_items.cpp
		dependencies/src/imgui/imgui_stdlib.cpp
)

file(GLOB LIBS_LIB "dependencies/libs/*.lib")

# Copy resource files to executable directory
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})

# Ajoutez une source à l'exécutable de ce projet.
add_executable(DataMorph ${MAIN_FILE} ${PROJECT_SOURCES} ${DEPENDENCIES_SOURCES})

target_include_directories(DataMorph PUBLIC include/)
target_include_directories(DataMorph PRIVATE dependencies/include/imgui)
target_include_directories(DataMorph PRIVATE dependencies/include)

target_link_libraries(DataMorph PRIVATE ${LIBS_LIB})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DataMorph PROPERTY CXX_STANDARD 20)
endif()

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
