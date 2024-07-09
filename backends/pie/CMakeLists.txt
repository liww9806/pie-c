set (PIE_SRCS main.cpp)
add_cpplint_files (${CMAKE_CURRENT_SOURCE_DIR} "${PIE_SRCS}")

set (PIE_BACKEND_SRCS
    control.h
    control.cpp
    deparser.h
    deparser.cpp
    parser.h
    parser.cpp
)
add_cpplint_files (${CMAKE_CURRENT_SOURCE_DIR} "${PIE_BACKEND_SRCS}")

build_unified(PIE_BACKEND_SRCS)

add_library(pie_backend STATIC ${PIE_BACKEND_SRCS})

add_dependencies(pie_backend genIR)
find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

add_executable(p4c-pie ${PIE_SRCS})

message(STATUS "p4c lib: ${P4C_LIBRARIES}") 
target_link_libraries (p4c-pie frontend ir LLVM ${llvm_libs} ${P4C_LIBRARIES} ${P4C_LIB_DEPS} pie_backend)