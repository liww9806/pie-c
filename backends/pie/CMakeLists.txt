find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

set (PIE_SRCS main.cpp)
add_cpplint_files (${CMAKE_CURRENT_SOURCE_DIR} "${PIE_SRCS}")

set (PIE_BACKEND_SRCS
    backend.h
    backend.cpp
    control.h
    control.cpp
    deparser.h
    deparser.cpp
    llvm-ir.h
    llvm-ir.cpp
    midend.h
    midend.cpp
    parser.h
    parser.cpp
)

add_cpplint_files (${CMAKE_CURRENT_SOURCE_DIR} "${PIE_BACKEND_SRCS}")

add_executable(p4c-pie ${PIE_SRCS} ${PIE_BACKEND_SRCS})

add_library(libpie STATIC ${PIE_SRCS} ${PIE_BACKEND_SRCS})

add_dependencies(p4c-pie genIR)

target_link_libraries (p4c-pie frontend ir LLVM ${llvm_libs} ${P4C_LIBRARIES} ${P4C_LIB_DEPS})
