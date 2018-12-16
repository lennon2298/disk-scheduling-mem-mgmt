cmake_minimum_required(VERSION 3.1.2)
project(minor1)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source")
set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Source/Resources/include")
set(SOURCES "${SRC_DIR}/rendertext.c" "${SRC_DIR}/main.c" )

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)


add_executable(${PROJECT_NAME} ${SOURCES} )
target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}")
#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

# GLFW
set(GLFW_DIR "${LIB_DIR}/GLFW")
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory("${GLFW_DIR}")
target_link_libraries(${PROJECT_NAME} "glfw" "${GLFW_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLFW_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE "GLFW_INCLUDE_NONE")

# glad
set(GLAD_DIR "${LIB_DIR}/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

#FreeType
set(FTP_DIR "${LIB_DIR}/freetype")
target_link_libraries(${PROJECT_NAME} "freetype" "${FREETYPE2_LIBRARIES}")
target_include_directories(${PROJECT_NAME} PRIVATE "${FTP_DIR}/freetype")

#KazMath
set(KAZ_DIR "${LIB_DIR}/kazmath")
target_include_directories(${PROJECT_NAME} PRIVATE "${KAZ_DIR}/")
#target_include_directories(${PROJECT_NAME} PRIVATE "${KAZ_DIR}/GL/*.h")
target_link_libraries(${PROJECT_NAME} "kazmath" "${KAZMATH_LIBS}")