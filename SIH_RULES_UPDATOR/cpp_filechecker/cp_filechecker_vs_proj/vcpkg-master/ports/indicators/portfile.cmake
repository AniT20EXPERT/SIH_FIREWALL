# header-only library

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO p-ranav/indicators
    REF v2.3
    SHA512 59FB17E93C886CEC2418E93E221FEBD2BF3160691B3473363417F8F8FA3D50D42D1DE0E55D62598EBA800B13C1D5AB1422D0A2C867246006B61340BA32D99232
    HEAD_REF master
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DINDICATORS_BUILD_TESTS=OFF
        -DINDICATORS_SAMPLES=OFF
        -DINDICATORS_DEMO=OFF
)

vcpkg_cmake_install()

vcpkg_cmake_config_fixup(CONFIG_PATH lib/cmake/indicators)

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug" "${CURRENT_PACKAGES_DIR}/lib")

file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
file(INSTALL "${SOURCE_PATH}/LICENSE.termcolor" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}")