# CMakeList.txt : projet CMake pour DataMorph, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

project("DataMorph")

add_compile_options("/bigobj")

###################### Main DataMorph Project ######################

set(MAIN_FILE main.cpp)
set(PROJECT_SOURCES 
		src/App.cpp
		src/core/imgui_extension.cpp
		src/Constants.cpp
		src/gui/MainWindow.cpp
		src/gui/UpdateWindow.cpp
		src/gui/NewProjectWindow.cpp
		src/gui/GraphWindow.cpp
		src/gui/StatsWindow.cpp
		src/FontManager.cpp
		src/Utils.cpp
		src/core/Project.cpp
		src/core/Header.cpp
		src/core/Expression.cpp
		src/gui/CommonPopups.cpp
		src/core/Graph.cpp
		src/core/Regression.cpp
		src/core/Model.cpp
		src/core/FileHandler.cpp
		src/core/Settings.cpp
		src/gui/SettingsWindow.cpp
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
add_executable(DataMorph ${MAIN_FILE} ${PROJECT_SOURCES} ${DEPENDENCIES_SOURCES} resources/resources.rc)


# To hide the console on Windows
#target_link_options(DataMorph PRIVATE "/SUBSYSTEM:WINDOWS")
#target_link_options(DataMorph PRIVATE "/ENTRY:mainCRTStartup")

add_subdirectory(nativefiledialog-extended)
target_link_libraries(DataMorph PRIVATE nfd)

target_include_directories(DataMorph PUBLIC include/)
target_include_directories(DataMorph PRIVATE dependencies/include/imgui)
target_include_directories(DataMorph PRIVATE dependencies/include)

target_link_libraries(DataMorph PRIVATE ${LIBS_LIB})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET DataMorph PROPERTY CXX_STANDARD 20)
endif()

####################################################################

######################## Resources Embedder ########################

# Creates C resources file from files in given directory

set(MAIN_FILE src/embedder/main.cpp)
set(PROJECT_SOURCES 
)

add_executable(embedder ${MAIN_FILE} ${PROJECT_SOURCES})

target_include_directories(embedder PUBLIC include/)
target_include_directories(embedder PRIVATE dependencies/include/imgui)
target_include_directories(embedder PRIVATE dependencies/include)

#function (write_data dir outcontent)
#message("Embedding data in ${dir}")
#	file(GLOB bins ${dir}/* LIST_DIRECTORIES false)
#	foreach(bin ${bins})
#		string(REGEX MATCH "([^/]+)$" filename ${bin})
#		if (IS_DIRECTORY ${dir}/${filename})
#			write_data(${dir}/${filename} ${outcontent})
#		else()
#			message("> Embedding ${filename}")
#			string(REGEX REPLACE "\\.| |-" "_" filename ${filename})
#			file(READ ${bin} filedata HEX)
#			string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})
#			set(${outcontent} "${outcontent}\nconst unsigned char ${filename}[] = {${filedata}};\nconst unsigned ${filename}_size = sizeof(${filename});\n" PARENT_SCOPE)
#		endif()
#	endforeach()
#	return(PROPAGATE ${outcontent})
#endfunction()

#function(create_resources dir output_file output_content)
#   # Create empty output file
#	file(WRITE "include/${output_file}" 
#	"#ifndef RESOURCES_H\n#define RESOURCES_H\n\n")
#    # Iterate through input files
#    write_data(${dir} ${output_content})
#	message("Writing to include/${output_file}")
#	file(APPEND include/${output_file} "\n${output_content}\n#endif")
#endfunction()

#create_resources(resources "resources.h" "a")