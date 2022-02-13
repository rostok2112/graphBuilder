cmake_minimum_required (VERSION 3.12)
project(graphBuilder LANGUAGES C)

set(CMAKE_BUILD_TYPE Release)
set(ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/bin)
set(DEP_NAME calc)

include(FetchContent)

FetchContent_Declare(  # fetch dependency from git 
    ${DEP_NAME}
    GIT_REPOSITORY  https://github.com/rostok2112/libcalc.git
    GIT_TAG         origin/main
)

FetchContent_GetProperties(${DEP_NAME})
if (NOT ${DEP_NAME}_POPULATED)
    FetchContent_Populate(${DEP_NAME})
endif ()

execute_process(  #start shell script of build depend
    COMMAND  sh build.sh
    WORKING_DIRECTORY ${${DEP_NAME}_SOURCE_DIR}
)

file(MAKE_DIRECTORY ${ROOT_DIR}/lib) # make lib dir
configure_file(${${DEP_NAME}_SOURCE_DIR}/export/${DEP_NAME}.h ${ROOT_DIR}/include COPYONLY) #copy headers to include/ directory
configure_file(${${DEP_NAME}_SOURCE_DIR}/export/lib${DEP_NAME}.a ${ROOT_DIR}/lib COPYONLY) #copy lib to lib/ directory 

file(GLOB 
	CS "${ROOT_DIR}/src/*.c") #sources

include_directories("${ROOT_DIR}/include") 
link_directories("${ROOT_DIR}/lib")

SET(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS}  -rdynamic")

add_executable(${PROJECT_NAME}  ${CS} )

target_link_libraries(${PROJECT_NAME} ${DEP_NAME})  # link dependency


# Find GTK
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
link_directories(${GTK3_LIBRARY_DIRS})
add_compile_options(${GTK3_CFLAGS_OTHER})
add_compile_options(${GTK3_LIBS_OTHER})
target_link_libraries(${PROJECT_NAME}  -lm -ldl ${GTK3_LIBRARIES})
include_directories( ${GTK3_INCLUDE_DIRS})

file(COPY ${ROOT_DIR}/resource DESTINATION ${ROOT_DIR}/bin/) #copy resource dir to bin