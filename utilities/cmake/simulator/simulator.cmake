IF(UNIT_TESTS_SWITCH)
        file(GLOB_RECURSE SOURCES "simulator/*.*" "common/*.*" "src/*.*" "apps/*.*" "tests/*.*")
        #exclude src/main.c from the compilation list as it needs to be overriden by unit_tests_main.c
        LIST(REMOVE_ITEM SOURCES "${PROJECT_SOURCE_DIR}/src/main.c")

        #need these macros to correctly configure unity test framework
        add_compile_definitions(UNITY_INCLUDE_CONFIG_H)
        add_compile_definitions(UNITY_FIXTURE_NO_EXTRAS)
ELSE()
        file(GLOB_RECURSE SOURCES "simulator/*.*" "common/*.*" "src/*.*" "apps/*.*")
ENDIF(UNIT_TESTS_SWITCH)

add_compile_definitions(USE_SIMULATOR=1 ATCAPRINTF USE_MONERO=1 USE_BIP32_CACHE=0 USE_BIP39_CACHE=0)
IF (DEV_SWITCH)
    add_compile_definitions(DEV_BUILD)
ENDIF(DEV_SWITCH)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(EXECUTABLE ${PROJECT_NAME})
find_package(SDL2 REQUIRED SDL2)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${EXECUTABLE} ${SOURCES} ${CMAKE_CURRENT_BINARY_DIR}/version.c ${MINI_GMP_SRCS} ${POSEIDON_SRCS} ${PROTO_SRCS} ${PROTO_HDRS} ${INCLUDES})

if ("${FIRMWARE_TYPE}" STREQUAL "Main")
    add_compile_definitions(X1WALLET_INITIAL=0 X1WALLET_MAIN=1)
elseif("${FIRMWARE_TYPE}" STREQUAL "Initial")
    add_compile_definitions(X1WALLET_INITIAL=1 X1WALLET_MAIN=0)
else()
    message(FATAL_ERROR "Firmware type not specified. Specify using -DFIRMWARE_TYPE=<Type> Type can be Main or Initial")
endif()
target_include_directories(${PROJECT_NAME} PRIVATE
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
        apps/xrp_app
        apps/starknet_app

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

        # Simulator
        simulator
        simulator/BSP
        simulator/Buzzer
        simulator/Flash
        simulator/nfc
        simulator/lv_drivers
        simulator/lv_drivers/display
        simulator/lv_drivers/indev
        simulator/porting
        simulator/USB

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
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/utils>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/nfc/events>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/manager_app>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/btc_app>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/evm_app>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/near_app>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/solana_app>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/inheritance_app>
        $<$<BOOL:UNIT_TESTS_SWITCH>:${PROJECT_SOURCE_DIR}/tests/apps/xrp_app>
        )

IF(UNIT_TESTS_SWITCH)
        target_compile_options(${EXECUTABLE} PRIVATE --coverage -g -O0)
        target_link_libraries(${EXECUTABLE} PRIVATE -lgcov )
ENDIF(UNIT_TESTS_SWITCH)
target_link_libraries(${EXECUTABLE} PRIVATE ${SDL2_LIBRARIES} -lm)
target_link_options(${EXECUTABLE} PRIVATE ${inherited})
add_custom_target (run COMMAND ${EXECUTABLE_OUTPUT_PATH}/${EXECUTABLE})
