set(LIBB64_INCLUDE_DIRS ${EXTLIB_INSTALL_DIR}/include)

if(MSVC)
    set(LIBB64_LIBRARIES ${EXTLIB_INSTALL_DIR}/lib/b64.lib)
else()
    set(LIBB64_LIBRARIES ${EXTLIB_INSTALL_DIR}/lib/libb64.a)
endif()
