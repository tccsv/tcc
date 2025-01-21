cmake_minimum_required(VERSION 3.25.0)

project(TCC C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(AppName TCC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/app )


if (MSVC)
    add_compile_options(/O2 /experimental:c11atomics)
endif()


add_executable(${AppName}
    TCC/core/TCC_money.c
    TCC/core/TCC_STS.c
    TCC/infra/TCC_money_load.c
    
    main.c)



target_include_directories(${AppName} PRIVATE ${CMAKE_SOURCE_DIR})

# # Для таймера и/или ui раскомментировать
# add_subdirectory(third_party/SDL)
# target_link_libraries(${AppName} PRIVATE SDL2)