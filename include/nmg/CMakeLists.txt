set(
  nmg_headers
  check.h
  debug.h
  defines.h
  edge.h
  face.h
  globals.h
  index.h
  io.h
  isect.h
  loop.h
  model.h
  nurb.h
  plot.h
  print.h
  radial.h
  ray.h
  region.h
  shell.h
  topology.h
  vertex.h
  visit.h
)
brlcad_manage_files(nmg_headers ${INCLUDE_DIR}/brlcad/nmg REQUIRED libnmg)

cmakefiles(CMakeLists.txt)

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
