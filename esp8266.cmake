# this is cmake file for ardunio esp8266
# must define ESP_ROOT ESP_BOARD ARDUINO_FCPU
set(CMAKE_SYSTEM_NAME Generic)

enable_language(C)
enable_language(ASM)

#cmake_policy(SET CMP0046 OLD)

if(NOT CHIP)
    set(CHIP "esp8266")
endif()

# esp path
set(CORE_DIR "${ESP_ROOT}/cores/${CHIP}")
set(PINS_DIR "${ESP_ROOT}/variants/${ESP_BOARD}")
set(TOOLS_DIR  "${ESP_ROOT}/tools")

# lwip
set(LWIP_VERSION "2")

## toolchain
set(TOOLCHAIN_BIN_DIR "${TOOLS_DIR}/xtensa-lx106-elf/bin")
set(CMAKE_ASM_COMPILER  "${TOOLCHAIN_BIN_DIR}/xtensa-lx106-elf-gcc")
set(CMAKE_AR            "${TOOLCHAIN_BIN_DIR}/xtensa-lx106-elf-ar")
set(CMAKE_C_COMPILER    "${TOOLCHAIN_BIN_DIR}/xtensa-lx106-elf-gcc")
set(CMAKE_CXX_COMPILER  "${TOOLCHAIN_BIN_DIR}/xtensa-lx106-elf-g++")
set(ESP_SIZE "${TOOLCHAIN_BIN_DIR}/xtensa-lx106-elf-size")
set(CMAKE_CXX_LINK_EXECUTABLE
    "<CMAKE_C_COMPILER>  <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")

set(ESP_TOOL "${TOOLS_DIR}/esptool/esptool")
set(SDK_DIR "${TOOLS_DIR}/sdk")
set(BOOT_LOADER ${ESP_ROOT}/bootloaders/eboot/eboot.elf)
    
set(CMAKE_CXX_FLAGS "-w -Os -g -mlongcalls -mtext-section-literals -fno-exceptions -fno-rtti -falign-functions=4 -std=c++11 -MMD -ffunction-sections -fdata-sections")
set(CMAKE_C_FLAGS "-w -Os -g -Wpointer-arith -Wno-implicit-function-declaration -Wl,-EL -fno-inline-functions -nostdlib -mlongcalls -mtext-section-literals -falign-functions=4 -MMD -std=gnu99 -ffunction-sections -fdata-sections")
set(CMAKE_ASM_FLAGS "-g -x assembler-with-cpp -MMD -mlongcalls")
set(CMAKE_EXE_LINKER_FLAGS "-g -w -Os -nostdlib -Wl,--no-check-sections -u call_user_start -u _printf_float -u _scanf_float -Wl,-static -T${ESP_FLASH_LD} -Wl,--gc-sections -Wl,-wrap,system_restart_local -Wl,-wrap,spi_flash_read")

add_definitions(-D__ets__ -DICACHE_FLASH -U__STRICT_ANSI__ -DESP8266)

if(${LWIP_VERSION} MATCHES "2")
    add_definitions(-DLWIP_OPEN_SRC -DTCP_MSS=536)
else()
    add_definitions(-DLWIP_OPEN_SRC)
endif()

include_directories(${CORE_DIR})
include_directories(${PINS_DIR})
include_directories(${SDK_DIR}/include ${SDK_DIR}/lwip${LWIP_VERSION}/include ${SDK_DIR}/libc/xtensa-lx106-elf)

link_directories(${SDK_DIR}/lib ${SDK_DIR}/ld ${SDK_DIR}/libc/xtensa-lx106-elf/lib)

file(GLOB_RECURSE CORE_CPP_FILES ${CORE_DIR}/*.cpp)
file(GLOB_RECURSE CORE_ASM_FILES ${CORE_DIR}/*.S)
file(GLOB_RECURSE CORE_C_FILES ${CORE_DIR}/*.c)

set(LIB_DEPS hal phy pp net80211 lwip${LWIP_VERSION} wpa crypto main wps axtls espnow smartconfig airkiss mesh wpa2 stdc++ m c gcc)

add_library(ardunio STATIC ${CORE_ASM_FILES} ${CORE_CPP_FILES} ${CORE_C_FILES})
target_compile_definitions(ardunio PUBLIC F_CPU=${ARDUINO_FCPU} ARDUINO=10605 ARDUINO_${ESP_BOARD}
    ARDUINO_ARCH_${CHIP} ARDUINO_BOARD=${ESP_BOARD})
    

# auto add all dependencies
set(SYS_LIB_ROOT "${ESP_ROOT}/libraries")
foreach(lib in ${LIBS})
    if(EXISTS ${SYS_LIB_ROOT}/${lib})
        include_directories(${SYS_LIB_ROOT}/${lib}/src)
        file(GLOB_RECURSE LIB_FILES ${SYS_LIB_ROOT}/${lib}/src/*.cpp)
        list(APPEND SRCS ${LIB_FILES})
    endif()
    if(EXISTS ${USER_LIB_DIR}/${lib})
        include_directories(${USER_LIB_DIR}/${lib} ${USER_LIB_DIR}/${lib}/src)
        file(GLOB_RECURSE LIB_FILES ${USER_LIB_DIR}/${lib}/*.cpp ${USER_LIB_DIR}/${lib}/*.c)
        list(APPEND SRCS ${LIB_FILES})
    endif()
endforeach()

file(GLOB_RECURSE MAIN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
list(APPEND SRCS ${MAIN_FILES})

message("all srcs: ${SRCS}")

add_executable(${CMAKE_PROJECT_NAME}.elf ${SRCS})
target_link_libraries(smarthome.elf -Wl,--start-group ardunio ${LIB_DEPS} -Wl,--end-group)
    
add_custom_target(bin ALL ${ESP_TOOL} -eo "${BOOT_LOADER}" -bo "${CMAKE_PROJECT_NAME}.bin" -bm ${ESP_FLASH_MODE} -bf ${ESP_FLASH_FREQ} -bz ${ESP_FLASH_SIZE} -bs .text -bp 4096 -ec -eo "${CMAKE_PROJECT_NAME}.elf" -bs .irom0.text -bs .text -bs .data -bs .rodata -bc -ec)

# calc size
find_program(AWK awk)

if(EXISTS ${AWK})
    set(AWK_STRING "$1 ~ /^\\.irom0\\.text|\\.text|\\.data|\\.rodata/ {fsum += $2} $1 ~ /^\\.data|\\.rodata|\\.bss/ {msum += $2} END {print \"flash use: \" fsum \" byte\\nmemory use: \" msum \" bytes\" }")
    add_custom_target(size ALL 
        COMMAND ${ESP_SIZE} -A ${CMAKE_PROJECT_NAME}.elf | ${AWK} ${AWK_STRING}
        VERBATIM
        DEPENDS bin
        COMMENT "calculate flash and memory usage")
endif()

add_custom_target(upload
    COMMAND ${ESP_TOOL} -vv -cd nodemcu -cb ${ARDUINO_UPLOAD_SPEED} -cp "${ARDUINO_PORT}" -ca 0x00000 -cf "${CMAKE_PROJECT_NAME}.bin"
    DEPENDS bin
    COMMENT "upload to borad")
