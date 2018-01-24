
cmake_minimum_required (VERSION 2.6)

project (xselman)

include(GNUInstallDirs)

if(CMAKE_COMPILER_IS_GNUCXX)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -W")
	set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -W")
endif(CMAKE_COMPILER_IS_GNUCXX)

find_package (PkgConfig REQUIRED)
pkg_check_modules (GTK2 REQUIRED gtk+-2.0)

# Main executable:
include_directories ("${GTK2_INCLUDE_DIRS}")
add_executable (xselman xselman.c)
target_link_libraries (xselman "${GTK2_LIBRARIES}")

# Manual:
add_custom_command(
	OUTPUT xselman.1
	DEPENDS manual.t2t
	COMMAND txt2tags -q -t man -i ${CMAKE_CURRENT_SOURCE_DIR}/manual.t2t -o ${CMAKE_CURRENT_BINARY_DIR}/xselman.1
	)
add_custom_command(
	OUTPUT xselman.1.gz
	DEPENDS xselman.1
	COMMAND gzip -9k ${CMAKE_CURRENT_BINARY_DIR}/xselman.1
	)
add_custom_target(manual ALL DEPENDS xselman.1.gz)

# Install:
install (
    TARGETS xselman
    DESTINATION bin
    )
install (
    FILES ${CMAKE_CURRENT_BINARY_DIR}/xselman.1.gz
    DESTINATION ${CMAKE_INSTALL_MANDIR}/man1/
    )