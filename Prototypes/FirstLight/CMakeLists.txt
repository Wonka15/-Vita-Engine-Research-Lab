cmake_minimum_required(VERSION 3.16)

# VitaSDK's toolchain file must be selected before project().
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake"
        CACHE PATH "VitaSDK toolchain file")
  else()
    message(FATAL_ERROR "Set VITASDK or pass CMAKE_TOOLCHAIN_FILE.")
  endif()
endif()

project(first_light C)

include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Vita Engine First Light")
set(VITA_TITLEID "VEL000001")
set(VITA_VERSION "01.00")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
add_compile_options(-O2 -Wall -Wextra -ffast-math)

add_executable(first_light
  src/main.c
)

# vitaGL is installed separately into the active VitaSDK prefix.
target_link_libraries(first_light
  vitaGL
  stdc++
  c
  m
  mathneon
  SceGxm_stub
  SceDisplay_stub
  SceAppMgr_stub
  SceCtrl_stub
  SceCommonDialog_stub
  vitashark
  SceShaccCgExt
  taihen_stub
  SceShaccCg_stub
  SceKernelDmacMgr_stub
  SceLibKernel_stub
)

vita_create_self(first_light.self first_light)
vita_create_vpk(first_light.vpk ${VITA_TITLEID} first_light.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
)
