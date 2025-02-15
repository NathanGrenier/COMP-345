# Disable tests in vendor libraries (if any)
set(SDL_TEST OFF)

# SDL
add_subdirectory(SDL)
# SDL_image
set(SDLIMAGE_AVIF OFF)
set(SDLIMAGE_BMP OFF)
set(SDLIMAGE_JPEG OFF)
set(SDLIMAGE_WEBP OFF)
set(SDLIMAGE_VENDORED ON)
add_subdirectory(SDL_image)
# SDL_ttf
set(SDLTTF_VENDORED ON)
add_subdirectory(SDL_ttf)

add_library(vendor INTERFACE)
target_link_libraries(vendor INTERFACE SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf)