set(sub_dir_name src)

set( fontconfig_SRCS
    fcarch.c
    fcatomic.c
    fccache.c
    fccfg.c
    fccharset.c
    fccompat.c
    fcdbg.c
    fcdefault.c
    fcdir.c
    fcformat.c
    fcfreetype.c
    fcfs.c
    fcinit.c
    fclang.c
    fclist.c
    fcmatch.c
    fcmatrix.c
    fcname.c
    fcobjs.c
    fcpat.c
    fcrange.c
    fcserialize.c
    fcstat.c
    fcstr.c
    fcweight.c
    fcxml.c
    ftglue.c
)

set( fontconfig_generated_HDRS
    fcobjshash.h
    fcalias.h
    fcaliastail.h
    fcftalias.h
    fcftaliastail.h
)

set( fontconfig_include_DIRS
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/../fc-lang
)

if(MSVC)
    add_definitions( -DWINPOSIX_EXPORT= -DPIC "-DFcPublic=__declspec(dllexport)" )
    if (CMAKE_SYSTEM_NAME MATCHES "Windows")
        set( fontconfig_SRCS ${fontconfig_SRCS} win/dirent.c)
		set( fontconfig_include_DIRS ${fontconfig_include_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/win)
    endif()
endif(MSVC)

ADD_CUSTOM_COMMAND(
   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fcobjshash.h
   COMMAND perl "${CMAKE_CURRENT_SOURCE_DIR}/makefcobjshash.pl \"${CMAKE_C_COMPILER}\" \"/P -I ${CMAKE_CURRENT_SOURCE_DIR} -I ${CMAKE_CURRENT_SOURCE_DIR}/..\" ${CMAKE_CURRENT_SOURCE_DIR}/fcobjshash.gperf.h fcobjshash.gperf.i"
   DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fcobjshash.gperf.h ${CMAKE_CURRENT_SOURCE_DIR}/fcobjs.h
)

macro(MAKE_ALIAS _prefix _headers)
	ADD_CUSTOM_COMMAND(
	   OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_prefix}alias.h
	   COMMAND perl "${CMAKE_CURRENT_SOURCE_DIR}/makealias \"${CMAKE_CURRENT_SOURCE_DIR}\" ${CMAKE_CURRENT_BINARY_DIR}/${_prefix}alias.h ${CMAKE_CURRENT_BINARY_DIR}/${_prefix}aliastail.h ${_headers}"
	   DEPENDS ${_headers}
	)
endmacro()

set(PUBLIC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../fontconfig/fontconfig.h
	${CMAKE_CURRENT_SOURCE_DIR}/fcdeprecate.h
	${CMAKE_CURRENT_SOURCE_DIR}/../fontconfig/fcprivate.h
)
set(PUBLIC_FT_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/../fontconfig/fcfreetype.h
)

MAKE_ALIAS("fc" ${PUBLIC_FILES})
MAKE_ALIAS("fcft"  ${PUBLIC_FT_FILES})

set_source_files_properties(${fontconfig_generated_HDRS} PROPERTIES GENERATED TRUE)

include_directories(${fontconfig_include_DIRS})
add_library( fontconfig SHARED ${fontconfig_SRCS} ${fontconfig_generated_HDRS} )
set_target_properties(fontconfig PROPERTIES OUTPUT_NAME fontconfig )

target_link_libraries( fontconfig ${FREETYPE_LIBRARIES})
target_link_libraries( fontconfig ${EXPAT_LIBRARY} )

install( TARGETS fontconfig RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib )

