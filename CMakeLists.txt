cmake_minimum_required(VERSION 3.25.0)

project(TCC C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(AppName TCC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/app)


if (MSVC)
    add_compile_options(/O2 /experimental:c11atomics)
endif ()

find_package(OpenGL REQUIRED)

add_executable(${AppName}
        TCC/core/TCC_money.c
        TCC/core/TCC_STS.c
        TCC/infra/TCC_money_load.c
        #TCC/core/TCC_compute_shader.c

        main.c)


target_include_directories(${AppName} PRIVATE ${CMAKE_SOURCE_DIR})

# # Для таймера и/или ui раскомментировать
add_subdirectory(third_party/SDL)
target_link_libraries(${AppName} PRIVATE SDL2)

#add_subdirectory("third_party/GLEW/build/cmake")
target_include_directories(${AppName} PRIVATE "third_party/GLEW/include")
#target_link_libraries(${AppName} PRIVATE GLEW)

# Найти PostgreSQL
find_package(PostgreSQL REQUIRED)

# Добавить include директории
include_directories(${PostgreSQL_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR})

include_directories(${CMAKE_SOURCE_DIR}/TCC)

# Добавить исходные файлы базы данных
set(DB_SOURCES
        DataBase/postgres/tcc_db.c
)

target_sources(${AppName} PRIVATE ${DB_SOURCES})
target_link_libraries(${AppName} PRIVATE PostgreSQL::PostgreSQL)

# После find_package(PostgreSQL REQUIRED)
find_package(Threads REQUIRED)
target_link_libraries(${AppName} PRIVATE Threads::Threads)