# write brlcadversion.h
file(READ ${BRLCAD_SOURCE_DIR}/include/conf/MAJOR majorVersionNumber)
file(READ ${BRLCAD_SOURCE_DIR}/include/conf/MINOR minorVersionNumber)
file(READ ${BRLCAD_SOURCE_DIR}/include/conf/PATCH patchVersionNumber)

set(
  brlcadVersionContent
  "#define BRLCAD_LIB_MAJOR ${majorVersionNumber}"
  "#define BRLCAD_LIB_MINOR ${minorVersionNumber}"
  "#define BRLCAD_LIB_PATCH ${patchVersionNumber}"
)

file(WRITE brlcadversion.h ${brlcadVersionContent})

file(WRITE ../../../../rt^3/include/brlcad/brlcadversion.h ${brlcadVersionContent})

# for all targets
include_directories(${BRLCAD_BINARY_DIR}/include ${BRLCAD_SOURCE_DIR}/include ${TCL_INCLUDE_PATH})

# build the BrlcadCore.dll
set(BRLCADCORE_SOURCES BrlcadCore.def BrlcadCore.rc)

add_library(BrlcadCore SHARED ${BRLCADCORE_SOURCES})

target_link_libraries(
  BrlcadCore
  libbg-static
  libbn-static
  libbrep-static
  libbu-static
  libbv-static
  libged-static
  regex-static
  librt-static
  libbrep-static
  libwdb-static
  libnmg-static
  zlib-static
  openNURBS-static
)

set_property(TARGET BrlcadCore PROPERTY LINK_INTERFACE_LIBRARIES "")

# build the brlcad.dll
set(BRLCAD_SOURCES TclDummies.c brlcad.def brlcad.rc)

set(
  COREINTERFACE_SOURCES
  ../../../../rt^3/src/coreInterface/Arb8.cpp
  ../../../../rt^3/src/coreInterface/BagOfTriangles.cpp
  ../../../../rt^3/src/coreInterface/Combination.cpp
  ../../../../rt^3/src/coreInterface/Cone.cpp
  ../../../../rt^3/src/coreInterface/ConstDatabase.cpp
  ../../../../rt^3/src/coreInterface/Database.cpp
  ../../../../rt^3/src/coreInterface/Ellipsoid.cpp
  ../../../../rt^3/src/coreInterface/EllipticalTorus.cpp
  ../../../../rt^3/src/coreInterface/FileDatabase.cpp
  ../../../../rt^3/src/coreInterface/Halfspace.cpp
  ../../../../rt^3/src/coreInterface/HyperbolicCylinder.cpp
  ../../../../rt^3/src/coreInterface/Hyperboloid.cpp
  ../../../../rt^3/src/coreInterface/MemoryDatabase.cpp
  ../../../../rt^3/src/coreInterface/NonManifoldGeometry.cpp
  ../../../../rt^3/src/coreInterface/Object.cpp
  ../../../../rt^3/src/coreInterface/ParabolicCylinder.cpp
  ../../../../rt^3/src/coreInterface/Paraboloid.cpp
  ../../../../rt^3/src/coreInterface/Particle.cpp
  ../../../../rt^3/src/coreInterface/Pipe.cpp
  ../../../../rt^3/src/coreInterface/Sketch.cpp
  ../../../../rt^3/src/coreInterface/Sphere.cpp
  ../../../../rt^3/src/coreInterface/Torus.cpp
  ../../../../rt^3/src/coreInterface/Unknown.cpp
  ../../../../rt^3/src/coreInterface/VectorList.cpp
  ../../../../rt^3/src/coreInterface/globals.cpp
)

file(GLOB COREINTERFACE_HEADERS ../../../../rt^3/include/brlcad/*.h)

include_directories(../../../../rt^3/include ${ZLIB_INCLUDE_DIRS} ${OPENNURBS_INCLUDE_DIR})

if(MSVC)
  add_definitions("-DBRLCAD_COREINTERFACE_EXPORT=__declspec(dllexport)")

  set(COREINTERFACE_MSVC_LIBRARIES shlwapi Ws2_32 Psapi Rpcrt4)
endif(MSVC)

add_library(brlcad SHARED ${BRLCAD_SOURCES} ${COREINTERFACE_SOURCES})

target_link_libraries(
  brlcad
  libbg-static
  libbn-static
  libbrep-static
  libbu-static
  libbv-static
  libged-static
  regex-static
  librt-static
  libbrep-static
  libwdb-static
  libnmg-static
  poly2tri-static
  zlib-static
  openNURBS-static
  ${COREINTERFACE_MSVC_LIBRARIES}
)

set_property(TARGET brlcad PROPERTY LINK_INTERFACE_LIBRARIES "")

install(TARGETS brlcad RUNTIME DESTINATION bin LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)

install(FILES ${COREINTERFACE_HEADERS} DESTINATION include/brlcad)

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
