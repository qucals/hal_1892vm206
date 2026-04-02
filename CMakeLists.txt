cmake_minimum_required(VERSION 3.20)

project(mipsel-qemu
  LANGUAGES CXX C ASM
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# =============================================================================
# Конфигурация главной цели
# =============================================================================

file(GLOB_RECURSE _SOURCES 
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c*"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/asm/*.s"
)

file(GLOB_RECURSE _HEADERS
  "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*"
)

add_executable(${PROJECT_NAME} 
  ${_SOURCES}
)

target_sources(${PROJECT_NAME}
  PRIVATE
    FILE_SET HEADERS
    BASE_DIRS include
    FILES ${_HEADERS}
)

set_target_properties(${PROJECT_NAME} 
  PROPERTIES
    SUFFIX ".elf"
    LINK_SEARCH_START_STATIC ON
    LINK_SEARCH_END_STATIC ON
)

target_link_options(${PROJECT_NAME}
  PRIVATE -Wl,-Map=${CMAKE_BINARY_DIR}/project.map
  PRIVATE -Wl,-T ${CMAKE_CURRENT_SOURCE_DIR}/src/project.ld
  PRIVATE -Wl,--print-memory-usage
)

# =============================================================================
# Цель - запуск программы на эмуляторе
# =============================================================================

find_program(QEMU_SYSTEM_MIPSEL qemu-system-mipsel REQUIRED)

add_custom_target(run_qemu_gdb
  COMMAND ${QEMU_SYSTEM_MIPSEL}
    -M malta
    -cpu 24Kf
    -kernel $<TARGET_FILE:${PROJECT_NAME}>
    -nographic
    -serial mon:stdio
    -S
    -s
  DEPENDS ${PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Run ${PROJECT_NAME} in QEMU and wait for GDB on :1234"
  VERBATIM
)

# =============================================================================
# Конфигурация завивимостей
# =============================================================================

# set(UNITY_EXTENSION_FIXURE ON CACHE BOOL "Enable Unity fixture" FORCE)
# set(UNITY_EXTENSION_MEMORY ON CACHE BOOL "Enable Unity memory" FORCE)
#
# add_subdirectory(external/unity)
#
# target_include_directories(unity
#   PRIVATE
#     ${CMAKE_CURRENT_SOURCE_DIR}/config
# )
#
# target_compile_definitions(unity
#   PRIVATE
#     UNITY_INCLUDE_CONFIG_H
# )
#
# target_include_directories(test_1 
#   PRIVATE
#     include
#     external/unity/src
#     external/unity/extras/fixture/src
# )
#
# target_link_libraries(test_1
#   PRIVATE
#     unity
# )
