# opengl link libraries
SET(opengl_libs glfw3.lib opengl32.lib)

# set executable output path to bin/
SET(EXECUTABLE_OUTPUT_PATH "bin/")

# dirs to copy
SET(dirs_to_copy shader)

# copy dirs to output path
MACRO(COPYDIRS dirs)
    FOREACH (dir ${dirs})
        FILE(GLOB dir_files "${dir}/*")
        SET(dir_files_list "")
        LIST(APPEND dir_files_list ${dir_files})
        FOREACH (f ${dir_files_list})
            FILE(COPY ${f} DESTINATION "${EXECUTABLE_OUTPUT_PATH}/${dir}")
        ENDFOREACH ()
    ENDFOREACH ()
ENDMACRO()

COPYDIRS("${dirs_to_copy}")

FILE(GLOB_RECURSE source_files "*.c" "*.cpp")

FILE(GLOB_RECURSE header_files "*.h" "*.hpp")

# include all the *.h file in current directory
INCLUDE_DIRECTORIES(${header_files})

# get the parent directory name
GET_FILENAME_COMPONENT(parent_dir_name ${CMAKE_CURRENT_SOURCE_DIR} NAME)

# set the name of executable program as parent dir name and execute all the source file
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${source_files})

# link libraries when running
TARGET_LINK_LIBRARIES(
        ${CMAKE_PROJECT_NAME}
        ${opengl_libs}
        ${OpenCV_LIBS}
        ${ASSIMP_LIBRARIES}
)