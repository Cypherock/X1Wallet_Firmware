
enable_language(C ASM)
set(EXECUTABLE ${PROJECT_NAME}.elf)
set(LINKER_SCRIPT STM32L486RGTX_FLASH.ld)
set(STARTUP_FILE startup_stm32l486xx.s)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

IF(UNIT_TESTS_SWITCH)
        file(GLOB_RECURSE SOURCES "stm32-hal/*.*" "common/*.*" "src/*.*" "apps/*.*" "tests/*.*")
        #exclude src/main.c from the compilation list as it needs to be overriden by unit_tests_main.c
        LIST(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.c")

        #need these macros to correctly configure unity test framework
        add_compile_definitions(UNITY_INCLUDE_CONFIG_H)
        add_compile_definitions(UNITY_FIXTURE_NO_EXTRAS)
ELSE()
        file(GLOB_RECURSE SOURCES "stm32-hal/*.*" "common/*.*" "src/*.*" "apps/*.*")
ENDIF(UNIT_TESTS_SWITCH)

add_executable(${EXECUTABLE} ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/version.c ${MINI_GMP_SRCS} ${POSEIDON_SRCS} ${PROTO_SRCS} ${PROTO_HDRS} ${INCLUDES} ${LINKER_SCRIPT} ${STARTUP_FILE})
target_compile_definitions(${EXECUTABLE} PRIVATE -DUSE_HAL_DRIVER -DSTM32L486xx )
add_compile_definitions(USE_SIMULATOR=0 USE_BIP32_CACHE=0 USE_BIP39_CACHE=0 STM32L4 USBD_SOF_DISABLED ENABLE_HID_WEBUSB_COMM=1)
IF (DEV_SWITCH)
    add_compile_definitions(DEV_BUILD)
ENDIF(DEV_SWITCH)

if ("${FIRMWARE_TYPE}" STREQUAL "Main")
    add_compile_definitions(X1WALLET_INITIAL=0 X1WALLET_MAIN=1)
elseif("${FIRMWARE_TYPE}" STREQUAL "Initial")
    add_compile_definitions(X1WALLET_INITIAL=1 X1WALLET_MAIN=0)
else()
    message(FATAL_ERROR "Firmware type not specified. Specify using -DFIRMWARE_TYPE=<Type> Type can be Main or Initial")
endif()
target_include_directories(${EXECUTABLE} PRIVATE
        apps/manager_app

        apps/btc_family
        apps/btc_family/btc
        apps/btc_family/dash
        apps/btc_family/doge
        apps/btc_family/ltc
        apps/evm_family
        apps/evm_family/eth
        apps/evm_family/polygon
        apps/evm_family/bsc
        apps/evm_family/fantom
        apps/evm_family/avalanche
        apps/evm_family/optimism
        apps/evm_family/arbitrum
        apps/near_app
        apps/solana_app
        apps/tron_app
        apps/inheritance_app
        apps/starknet_app
        
        apps/xrp_app
        
        src/
        src/menu
        src/wallet
        src/restricted_app
        src/onboarding
        src/settings

        src/card_operations
        src/card_flows

        src/level_one/controller
        src/level_one/tasks
        src/level_two/controller
        src/level_two/tasks
        src/level_three/add_wallet/controller
        src/level_three/add_wallet/tasks
        src/level_three/advanced_settings/controller
        src/level_three/advanced_settings/tasks
        src/level_three/old_wallet/controller
        src/level_three/old_wallet/tasks
        src/level_four/core/controller
        src/level_four/core/tasks
        src/level_four/card_health_check/
        src/level_four/factory_reset/
        src/level_four/tap_cards/controller
        src/level_four/tap_cards/tasks

        common/interfaces/card_interface
        common/interfaces/desktop_app_interface
        common/interfaces/flash_interface
        common/interfaces/user_interface
        common/libraries/atecc
        common/libraries/atecc/atcacert
        common/libraries/atecc/basic
        common/libraries/atecc/crypto
        common/libraries/atecc/crypto/hashes
        common/libraries/atecc/hal
        common/libraries/atecc/host
        common/libraries/atecc/jwt
        common/libraries/crypto
        common/libraries/crypto/mpz_operations
        common/libraries/crypto/aes
        common/libraries/crypto/chacha20poly1305
        common/libraries/crypto/ed25519-donna
        common/libraries/crypto/monero
        common/libraries/crypto/random_gen
        common/libraries/proof_of_work
        common/libraries/shamir
        common/libraries/util
        common/startup
        common/logger
        common/coin_support
        common/coin_support/eth_sign_data
        common/coin_support/tron_parse_txn
        common/flash
        common/Firewall
        common/core
        common/timers

        common
        common/lvgl
        common/lvgl/src
        common/lvgl/src/lv_core
        common/lvgl/src/lv_draw
        common/lvgl/src/lv_font
        common/lvgl/src/lv_hal
        common/lvgl/src/lv_misc
        common/lvgl/src/lv_objx
        common/lvgl/src/lv_themes

        # Device
        stm32-hal
        stm32-hal/BSP
        stm32-hal/Inc

        stm32-hal/Drivers/CMSIS/Include
        stm32-hal/Drivers/CMSIS/Device/ST/STM32L4xx/Include
        stm32-hal/Drivers/STM32L4xx_HAL_Driver/Inc
        stm32-hal/Drivers/STM32L4xx_HAL_Driver/Inc/Legacy
        stm32-hal/Drivers/STM32L4xx_HAL_Driver/Src

        stm32-hal/Peripherals
        stm32-hal/Peripherals/Buzzer
        stm32-hal/Peripherals/display
        stm32-hal/Peripherals/display/SSD1306
        stm32-hal/Peripherals/nfc/

        stm32-hal/Src
        stm32-hal/porting
        stm32-hal/Inc

        stm32-hal/libusb/
        stm32-hal/libusb/inc
        stm32-hal/libusb/src

        #unit test framework
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/framework/unity>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/framework/unity/src>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/framework/unity/extras/fixture/src>

        #unit test modules: this list needs to be updated whenever a test module is being added
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/common/core>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/common/util>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/p0_events>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/ui/ui_events_test>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/usb/events>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/nfc/events>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/nfc>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/manager_app>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/btc_app>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/evm_app>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/near_app>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/solana_app>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/inheritance_app>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/xrp_app>
        )

# warning flags ref: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
target_compile_options(${EXECUTABLE} PRIVATE
        -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
        -fdata-sections -ffunction-sections
        -Wall -Wextra -Wdouble-promotion -Wformat=2 -Wformat-security -Wformat-nonliteral
        -Wformat-overflow=2 -Wmissing-include-dirs -Wnull-dereference -Wswitch-default
        -Wswitch-enum -Wunused-const-variable=2 -Wuninitialized -Wstrict-overflow=4
        -D_POSIX_C_SOURCE=200809L
        $<$<CONFIG:Debug>:-g3>
        $<$<CONFIG:Release>:-Werror>
        )

target_link_options(${EXECUTABLE} PRIVATE
        -T${CMAKE_SOURCE_DIR}/STM32L486RGTX_FLASH.ld
        -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16
        -mfloat-abi=hard -u _printf_float -lc -lm -lnosys
        -Wl,-Map=${PROJECT_NAME}.map,--cref -Wl,--gc-sections
        )

# Used to suppress compile time warnings in libraries
file(GLOB_RECURSE LIBRARIES_SRC_DIR "common/libraries/atecc/*.c"
        "common/libraries/atecc/*.h" "common/lvgl/*.c" "common/lvgl/*.h"
        "common/libraries/crypto/*.c" "common/libraries/crypto/*.h"
        "stm32-hal/Peripherals/*.c" "stm32-hal/Peripherals/*.h")
set_source_files_properties(${LIBRARIES_SRC_DIR} PROPERTIES COMPILE_FLAGS "-w")

# Print executable size
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-size ${EXECUTABLE})

# Create hex file
add_custom_command(TARGET ${EXECUTABLE}
        POST_BUILD
        COMMAND arm-none-eabi-objcopy -O ihex ${EXECUTABLE} ${PROJECT_NAME}.hex
        COMMAND arm-none-eabi-objcopy -O binary ${EXECUTABLE} ${PROJECT_NAME}.bin)
if (SIGN_BINARY)
        add_custom_command(TARGET ${EXECUTABLE}
                POST_BUILD
                COMMAND python3 ${CMAKE_SOURCE_DIR}/utilities/script/index.py add-header --input="${PROJECT_NAME}.bin" --output=${PROJECT_NAME}_Header.bin --version=${CMAKE_SOURCE_DIR}/version.txt --private-key=${CMAKE_SOURCE_DIR}/utilities/script/private_key1.h
                COMMAND python3 ${CMAKE_SOURCE_DIR}/utilities/script/index.py sign-header --input=${PROJECT_NAME}_Header.bin --output=${PROJECT_NAME}-signed.bin --private-key=${CMAKE_SOURCE_DIR}/utilities/script/private_key2.h
                COMMAND rm ${PROJECT_NAME}_Header.bin)
endif()
