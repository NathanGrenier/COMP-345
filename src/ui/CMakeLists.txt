add_library(ui STATIC
    LButton.cpp
    LTexture.cpp
)

# Expose the public headers for ui (located in src/include/ui)
target_include_directories(ui PUBLIC ${CMAKE_SOURCE_DIR}/src/include/ui)

# Link ui with vendor if needed
target_link_libraries(ui PUBLIC vendor)