#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "nappgui::sewer" for configuration "Release"
set_property(TARGET nappgui::sewer APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(nappgui::sewer PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/sewer.lib"
  )

list(APPEND _cmake_import_check_targets nappgui::sewer )
list(APPEND _cmake_import_check_files_for_nappgui::sewer "${_IMPORT_PREFIX}/lib/sewer.lib" )

# Import target "nappgui::osbs" for configuration "Release"
set_property(TARGET nappgui::osbs APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(nappgui::osbs PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/osbs.lib"
  )

list(APPEND _cmake_import_check_targets nappgui::osbs )
list(APPEND _cmake_import_check_files_for_nappgui::osbs "${_IMPORT_PREFIX}/lib/osbs.lib" )

# Import target "nappgui::core" for configuration "Release"
set_property(TARGET nappgui::core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(nappgui::core PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/core.lib"
  )

list(APPEND _cmake_import_check_targets nappgui::core )
list(APPEND _cmake_import_check_files_for_nappgui::core "${_IMPORT_PREFIX}/lib/core.lib" )

# Import target "nappgui::geom2d" for configuration "Release"
set_property(TARGET nappgui::geom2d APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(nappgui::geom2d PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/geom2d.lib"
  )

list(APPEND _cmake_import_check_targets nappgui::geom2d )
list(APPEND _cmake_import_check_files_for_nappgui::geom2d "${_IMPORT_PREFIX}/lib/geom2d.lib" )

# Import target "nappgui::draw2d" for configuration "Release"
set_property(TARGET nappgui::draw2d APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(nappgui::draw2d PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/draw2d.lib"
  )

list(APPEND _cmake_import_check_targets nappgui::draw2d )
list(APPEND _cmake_import_check_files_for_nappgui::draw2d "${_IMPORT_PREFIX}/lib/draw2d.lib" )

# Import target "nappgui::osgui" for configuration "Release"
set_property(TARGET nappgui::osgui APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(nappgui::osgui PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/osgui.lib"
  )

list(APPEND _cmake_import_check_targets nappgui::osgui )
list(APPEND _cmake_import_check_files_for_nappgui::osgui "${_IMPORT_PREFIX}/lib/osgui.lib" )

# Import target "nappgui::gui" for configuration "Release"
set_property(TARGET nappgui::gui APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(nappgui::gui PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/gui.lib"
  )

list(APPEND _cmake_import_check_targets nappgui::gui )
list(APPEND _cmake_import_check_files_for_nappgui::gui "${_IMPORT_PREFIX}/lib/gui.lib" )

# Import target "nappgui::osapp" for configuration "Release"
set_property(TARGET nappgui::osapp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(nappgui::osapp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/osapp.lib"
  )

list(APPEND _cmake_import_check_targets nappgui::osapp )
list(APPEND _cmake_import_check_files_for_nappgui::osapp "${_IMPORT_PREFIX}/lib/osapp.lib" )

# Import target "nappgui::inet" for configuration "Release"
set_property(TARGET nappgui::inet APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(nappgui::inet PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/inet.lib"
  )

list(APPEND _cmake_import_check_targets nappgui::inet )
list(APPEND _cmake_import_check_files_for_nappgui::inet "${_IMPORT_PREFIX}/lib/inet.lib" )

# Import target "nappgui::nrc" for configuration "Release"
set_property(TARGET nappgui::nrc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(nappgui::nrc PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/nrc.exe"
  )

list(APPEND _cmake_import_check_targets nappgui::nrc )
list(APPEND _cmake_import_check_files_for_nappgui::nrc "${_IMPORT_PREFIX}/bin/nrc.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
