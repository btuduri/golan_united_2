cmake_minimum_required(VERSION 3.9)
project(golan_united_2)


set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(source)
include_directories(
    ${SDL2_INCLUDE_DIR}
    ${SDL2_IMAGE_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
)
set(SOURCES_FILE 
    ${PROJECT_SOURCE_DIR}/datas.c
    ${PROJECT_SOURCE_DIR}/functions.c
    ${PROJECT_SOURCE_DIR}/main.c)

add_executable(golan_united_2 ${SOURCES_FILE})
target_link_libraries(
    golan_united_2 
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
)

install(TARGETS golan_united_2 DESTINATION bin)

# add the uninstall support
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    @ONLY)
add_custom_target(uninstall
"${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
