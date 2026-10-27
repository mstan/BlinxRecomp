cmake_minimum_required(VERSION 3.20)
project(BlinxRecomp LANGUAGES C CXX)
enable_testing()
set(XBOXLLE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/xboxlle" CACHE PATH "Pinned shared Xbox framework")
set(BLINX_GENERATED "${CMAKE_CURRENT_SOURCE_DIR}/build/generated/gen" CACHE PATH "Private original NTSC generated code")
if(NOT EXISTS "${XBOXLLE_ROOT}/pilots/native-title/CMakeLists.txt")
  message(FATAL_ERROR "Initialize the xboxlle submodule before building")
endif()
if(NOT EXISTS "${BLINX_GENERATED}/pilot_image.h")
  message(FATAL_ERROR "Run tools/game.py prepare or adopt with the matching owned NTSC input")
endif()
file(READ "${BLINX_GENERATED}/pilot_image.h" blinx_identity)
string(FIND "${blinx_identity}" "e6b7cd8a5527dd001e05bb3e881d9b24ffc58824ff67def394215ddbc8009249" identity_at)
if(identity_at LESS 0)
  message(FATAL_ERROR "Generated code does not belong to the reviewed original NTSC Blinx")
endif()
set(PILOT_GENERATED "${BLINX_GENERATED}" CACHE PATH "Private title output" FORCE)
set(PILOT_TITLE_MOD_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/mods/widescreen.c" CACHE FILEPATH
  "Statically linked trusted Blinx plugin registry" FORCE)
add_subdirectory("${XBOXLLE_ROOT}/pilots/native-title" framework)
set_target_properties(xlle_native_title PROPERTIES OUTPUT_NAME BlinxRecomp)
add_executable(blinx_widescreen_contract tests/widescreen_contract.c src/mods/widescreen.c
  "${XBOXLLE_ROOT}/pilots/native-title/visual_mods.c"
  "${XBOXLLE_ROOT}/pilots/native-title/visual_mod_plan.c")
target_include_directories(blinx_widescreen_contract PRIVATE "${XBOXLLE_ROOT}/pilots/native-title")
add_test(NAME blinx_widescreen_disabled COMMAND blinx_widescreen_contract)
add_executable(blinx_cheats_contract tests/cheats_contract.c src/mods/widescreen.c
  "${XBOXLLE_ROOT}/pilots/native-title/visual_mods.c"
  "${XBOXLLE_ROOT}/pilots/native-title/visual_mod_plan.c")
target_include_directories(blinx_cheats_contract PRIVATE "${XBOXLLE_ROOT}/pilots/native-title")
foreach(mode off time more infinite both)
  add_test(NAME blinx_cheats_${mode} COMMAND blinx_cheats_contract ${mode})
endforeach()
add_test(NAME blinx_cheats_rejects_invalid COMMAND blinx_cheats_contract invalid)
set_tests_properties(blinx_cheats_rejects_invalid PROPERTIES
  PASS_REGULAR_EXPRESSION "TITLE MOD STOP\\] invalid Blinx cheat selection")
add_test(NAME blinx_cheats_rejects_locked COMMAND blinx_cheats_contract locked)
set_tests_properties(blinx_cheats_rejects_locked PROPERTIES
  PASS_REGULAR_EXPRESSION "TITLE MOD STOP\\] Blinx time-limit constants are not writable")
find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_test(NAME blinx_widescreen_profiles COMMAND "${Python3_EXECUTABLE}"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/widescreen_suite.py" --contract "$<TARGET_FILE:blinx_widescreen_contract>")
add_test(NAME blinx_visual_profiles COMMAND "${Python3_EXECUTABLE}"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/visual_mods_suite.py" --contract "$<TARGET_FILE:blinx_widescreen_contract>"
  --framework "${XBOXLLE_ROOT}")
