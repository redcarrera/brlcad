set(STEP_SCHEMA_FILE ${CMAKE_CURRENT_SOURCE_DIR}/ap214e3.exp)
add_definitions(-DAP214e3)

if(DEFINED ENABLE_STEP_EXPERIMENTAL)
  generate_schema_inputs(${STEP_SCHEMA_FILE} gap214)
endif(DEFINED ENABLE_STEP_EXPERIMENTAL)

set(
  gap214_SOURCES
  ${express_srcs}
  ../BRLCADWrapper.cpp
  ../g-step/AP203.cpp
  ../g-step/Assembly_Product.cpp
  ../g-step/Default_Geometric_Context.cpp
  ../g-step/ON_Brep.cpp
  ../g-step/ON_NurbsCurve.cpp
  ../g-step/ON_NurbsSurface.cpp
  ../g-step/Shape_Representation.cpp
  ../g-step/Shape_Representation_Relationship.cpp
  Shape_Definition_Representation.cpp
  Add_Tree.cpp
  Comb.cpp
  G_Objects.cpp
  g-ap214.cpp
)

set(
  gap214_LIBS
  libwdb
  librt
  libbrep
  libbn
  libbu
  ${STEPCODE_UTILS_LIBRARY}
  ${STEPCODE_EDITOR_LIBRARY}
  ${STEPCODE_DAI_LIBRARY}
  ${STEPCODE_CORE_LIBRARY}
  ${OPENNURBS_LIBRARIES}
)

set(
  AP214_INCLUDE_DIRS
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/..
  ${CMAKE_CURRENT_SOURCE_DIR}/../g-step
  ${STEPCODE_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/src/libbrep # for PullbackCurve.h
)

if(DEFINED ENABLE_STEP_EXPERIMENTAL)
  brlcad_addexec(g-ap214 "${gap214_SOURCES}" "${gap214_LIBS}" NO_STRICT FOLDER Conv)
  target_include_directories(g-ap214 BEFORE PRIVATE ${AP214_INCLUDE_DIRS})
  add_dependencies(g-ap214 step-express-${SCHEMA_NAME}-gap214)
  set_property(
    SOURCE ${CMAKE_CURRENT_BINARY_DIR}/AUTOMOTIVE_DESIGN/SdaiAUTOMOTIVE_DESIGN.cc
    APPEND
    PROPERTY COMPILE_FLAGS "/bigobj"
  )
endif(DEFINED ENABLE_STEP_EXPERIMENTAL)

set(IGNORE_FILES ${gap214_SOURCES} CMakeLists.txt AP214e3.h ap214e3.exp)
cmakefiles(${IGNORE_FILES})

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
