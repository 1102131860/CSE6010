# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(FinalProject1 LANGUAGES C)

# Add src file to executable program
add_executable(FinalProject1 
    main.c
    PSO.h
    PSO.c
)

# Define _CRT_SECURE_NO_WARNINGS for Visual Studio
target_compile_definitions(FinalProject1 PRIVATE _CRT_SECURE_NO_WARNINGS)

# Find and link OpenMP
find_package(OpenMP REQUIRED)
target_link_libraries(FinalProject1 PUBLIC OpenMP::OpenMP_C)

# Set working directory
set_target_properties(FinalProject1 PROPERTIES WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/")

# Custom command to copy data files to the executable's output directory
add_custom_command(
    TARGET FinalProject1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:FinalProject1>" "${CMAKE_CURRENT_SOURCE_DIR}/"
)
