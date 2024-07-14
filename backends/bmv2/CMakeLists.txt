# Copyright 2013-present Barefoot Networks, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Makefile for a backend that generates code for the Behavioral Model version 2 (BMv2)
# compiling for the simple_switch target.

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/simple_switch/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/simple_switch/version.h" @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/psa_switch/version.h.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/psa_switch/version.h" @ONLY)

# sources for backend executable
set (BMV2_SIMPLE_SWITCH_SRCS
    simple_switch/main.cpp
    simple_switch/midend.cpp
    simple_switch/midend.h
    simple_switch/simpleSwitch.cpp
    simple_switch/simpleSwitch.h
    simple_switch/options.h
    )

set (BMV2_PSA_SWITCH_SRCS
    psa_switch/main.cpp
    psa_switch/midend.cpp
    psa_switch/midend.h
    psa_switch/psaSwitch.cpp
    psa_switch/psaSwitch.h
    psa_switch/psaProgramStructure.cpp
    psa_switch/psaProgramStructure.h
    psa_switch/options.cpp
    psa_switch/options.h
    )

set (BMV2_BACKEND_COMMON_SRCS
  common/JsonObjects.cpp
  common/action.cpp
  common/controlFlowGraph.cpp
  common/deparser.cpp
  common/expression.cpp
  common/extern.cpp
  common/globals.cpp
  common/header.cpp
  common/helpers.cpp
  common/lower.cpp
  common/metermap.cpp
  common/parser.cpp
  common/programStructure.cpp
  )

set (BMV2_BACKEND_COMMON_HDRS
  common/JsonObjects.h
  common/action.h
  common/annotations.h
  common/backend.h
  common/check_unsupported.h
  common/control.h
  common/controlFlowGraph.h
  common/deparser.h
  common/expression.h
  common/extern.h
  common/globals.h
  common/header.h
  common/helpers.h
  common/lower.h
  common/metermap.h
  common/midend.h
  common/options.h
  common/parser.h
  common/programStructure.h
  common/sharedActionSelectorCheck.h
  )

set (IR_DEF_FILES ${IR_DEF_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/bmv2.def PARENT_SCOPE)

add_library(bmv2backend STATIC ${BMV2_BACKEND_COMMON_SRCS})
target_link_libraries(bmv2backend ir-generated frontend)

add_executable(p4c-bm2-ss ${BMV2_SIMPLE_SWITCH_SRCS})
target_link_libraries (p4c-bm2-ss bmv2backend ${P4C_LIBRARIES} ${P4C_LIB_DEPS})


