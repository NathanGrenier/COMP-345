# Disable tests in vendor libraries (if any)
set(SDL_TEST OFF)

# SDL
add_subdirectory(SDL EXCLUDE_FROM_ALL)
# SDL_image
set(SDLIMAGE_AVIF OFF)
set(SDLIMAGE_BMP ON)
set(SDLIMAGE_JPEG OFF)
set(SDLIMAGE_WEBP OFF)
add_subdirectory(SDL_image EXCLUDE_FROM_ALL)
# SDL_ttf
add_subdirectory(SDL_ttf EXCLUDE_FROM_ALL)
# json
add_subdirectory(json EXCLUDE_FROM_ALL)

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf nlohmann_json::nlohmann_json)