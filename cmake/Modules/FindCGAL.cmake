if (CGAL_INCLUDES)
    set(CGAL_FIND_QUIETLY TRUE)
endif (CGAL_INCLUDES)

find_path(CGAL_INCLUDES CGAL/Exact_predicates_inexact_constructions_kernel.h)
find_library(CGAL_LIB NAMES CGAL)
find_library(CGAL_CORE_LIB NAMES CGAL_Core)
find_library(GMP_LIB NAMES gmp)
find_library(BOOST_THREAD_LIB NAMES boost_thread)
find_library(BOOST_SYSTEM_LIB NAMES boost_system)

if (CGAL_LIB)
    set(CGAL_LIBRARIES_TMP ${CGAL_LIBRARIES_TMP} ${CGAL_LIB})
endif()

if (CGAL_CORE_LIB)
    set(CGAL_LIBRARIES_TMP ${CGAL_LIBRARIES_TMP} ${CGAL_CORE_LIB})
endif()

if (GMP_LIB)
    set(CGAL_LIBRARIES_TMP ${CGAL_LIBRARIES_TMP} ${GMP_LIB})
endif()

if (BOOST_THREAD_LIB)
    set(CGAL_LIBRARIES_TMP ${CGAL_LIBRARIES_TMP} ${BOOST_THREAD_LIB})
endif()

if (BOOST_SYSTEM_LIB)
    set(CGAL_LIBRARIES_TMP ${CGAL_LIBRARIES_TMP} ${BOOST_SYSTEM_LIB})
endif()

set(CGAL_LIBRARIES ${CGAL_LIBRARIES_TMP}
	CACHE STRING "All libraries required by CGAL")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(CGAL DEFAULT_MSG
    CGAL_LIBRARIES
    CGAL_INCLUDES)

mark_as_advanced(CGAL_LIBRARIES CGAL_INCLUDES)
