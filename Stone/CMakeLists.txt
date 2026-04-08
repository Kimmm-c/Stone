# CMakeList.txt : CMake project for Stone, include source and define
# project specific logic here.
#
# Collect all source and header files under src/
file(GLOB_RECURSE PROJECT_SOURCES
    "${CMAKE_SOURCE_DIR}/Stone/src/*.cpp"
)

# Add source to this project's executable.
add_executable (Stone
                "Stone.cpp"
                ${PROJECT_SOURCES}
                )

# ---- SDL paths ----
set(SDL3_LIB_DIR "${CMAKE_SOURCE_DIR}/Stone/libs/SDL3")
set(SDL3_IMAGE_LIB_DIR "${CMAKE_SOURCE_DIR}/Stone/libs/SDL3_Image/x64")
set(SDL3_MIXER_LIB_DIR "${CMAKE_SOURCE_DIR}/Stone/libs/SDL3_Mixer")
set(SDL3_TTF_LIB_DIR "${CMAKE_SOURCE_DIR}/Stone/libs/SDL3_ttf")

# ---- Include Directories (VC++ -> Include Directories) ----
target_include_directories(Stone
    PRIVATE 
        "${CMAKE_SOURCE_DIR}/Stone/include"
        "${CMAKE_SOURCE_DIR}/Stone/include/vendor"
        "${CMAKE_SOURCE_DIR}/Stone/include/systems"
)

# ---- Library Directories (VC++ -> Library Directories) ----
target_link_directories(Stone
    PRIVATE 
        ${SDL3_LIB_DIR}
        ${SDL3_IMAGE_LIB_DIR}
        ${SDL3_MIXER_LIB_DIR}
        ${SDL3_TTF_LIB_DIR}
)

# ---- Linker -> Input -> Additional Dependencies ----
target_link_libraries(Stone
    PRIVATE 
        "${CMAKE_SOURCE_DIR}/Stone/libs/SDL3/libSDL3.dll.a"
        SDL3_image.lib
        "${CMAKE_SOURCE_DIR}/Stone/libs/SDL3_Mixer/libSDL3_mixer.dll.a"
        "${CMAKE_SOURCE_DIR}/Stone/libs/SDL3_ttf/libSDL3_ttf.dll.a"
)

# Copy DLLs after build
file(GLOB DLL_FILES
    "${CMAKE_SOURCE_DIR}/Stone/dlls/*.dll"
)

add_custom_command(TARGET Stone POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${DLL_FILES}
        $<TARGET_FILE_DIR:Stone>
)

# Set Paths
target_compile_definitions(Stone PRIVATE
    ASSET_PATH="${CMAKE_SOURCE_DIR}/Stone/assets/"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Stone PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
