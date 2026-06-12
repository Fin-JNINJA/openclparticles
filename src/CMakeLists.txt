set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable (openclparticles WIN32 "openclparticles.cpp"  )

target_link_libraries(
    openclparticles
    PRIVATE
    wx::core wx::net wx::base
    )