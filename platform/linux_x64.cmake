set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_C_COMPILER clang)

include(${CMAKE_CURRENT_LIST_DIR}/vars.cmake)

set(MCU_INCLUDE_MK_FILE ${REMCU_VM_PATH}/mcu_utils/linux.mk)
#set(STRIP_COMMAND strip)

include(${REMCU_VM_PATH}/cmake/common_linux_flags.cmake)

set(TARGET_REMCU_OS_NAME Linux_x64_r)

set(LICENSE_TYPE APACHE_2.0)