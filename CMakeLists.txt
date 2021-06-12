# Copyright (C) 2009 Leandro Lisboa Penz <lpenz@lpenz.org>
# This file is subject to the terms and conditions defined in
# file 'LICENSE', which is part of this source code package.

cmake_minimum_required (VERSION 2.6)

project (xselman)

include(GNUInstallDirs)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -W")
endif(CMAKE_COMPILER_IS_GNUCXX)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK2 REQUIRED gtk+-2.0)
include_directories ("${GTK2_INCLUDE_DIRS}")

# Main targets

add_executable (xselman xselman.c)
target_link_libraries (xselman "${GTK2_LIBRARIES}")

# Manual

add_custom_command(
	OUTPUT xselman.1.gz
	DEPENDS xselman.1
	COMMAND gzip -9kc
		${CMAKE_CURRENT_SOURCE_DIR}/xselman.1
		> ${CMAKE_CURRENT_BINARY_DIR}/xselman.1.gz
	)
add_custom_target(manual ALL DEPENDS xselman.1.gz)

# Install

install (
    TARGETS xselman
    DESTINATION bin/
    )
install (
    FILES ${CMAKE_CURRENT_BINARY_DIR}/xselman.1.gz
    DESTINATION ${CMAKE_INSTALL_MANDIR}/man1/
    )
install (
    FILES README.md LICENSE AUTHORS
    DESTINATION ${CMAKE_INSTALL_DOCDIR}/
    )

# cpack deb

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_CONTACT "Leandro Lisboa Penz <lpenz@lpenz.org>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "X selection management")
set(CPACK_DEBIAN_PACKAGE_SECTION utils)
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
execute_process(COMMAND git describe --tags
                OUTPUT_VARIABLE GIT_DESCRIBE)
string(REGEX REPLACE "-g[0-9a-f]+\n$" "" CPACK_PACKAGE_VERSION "${GIT_DESCRIBE}")
set(CPACK_PACKAGE_FILE_NAME "xselman-${CPACK_PACKAGE_VERSION}")
include(CPack)

