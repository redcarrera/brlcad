# Style sheet for XSLT transformation to book HTML pages
if(BRLCAD_EXTRADOCS_HTML)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../resources/brlcad/brlcad-presentation-xhtml-stylesheet.xsl.in
    ${CMAKE_CURRENT_BINARY_DIR}/../resources/brlcad/brlcad-presentation-xhtml-stylesheet.xsl
  )
  distclean("${CMAKE_CURRENT_BINARY_DIR}/../resources/brlcad/brlcad-presentation-xhtml-stylesheet.xsl")
endif(BRLCAD_EXTRADOCS_HTML)

if(BRLCAD_EXTRADOCS_PHP)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../resources/brlcad/brlcad-presentation-xhtml-stylesheet.xsl.in
    ${CMAKE_CURRENT_BINARY_DIR}/../resources/brlcad/brlcad-presentation-xhtml-stylesheet.xsl
  )
  distclean("${CMAKE_CURRENT_BINARY_DIR}/../resources/brlcad/brlcad-presentation-xhtml-stylesheet.xsl")
endif(BRLCAD_EXTRADOCS_PHP)

if(BRLCAD_EXTRADOCS_PPT)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../resources/brlcad/presentation.xsl.in
    ${CMAKE_CURRENT_BINARY_DIR}/../resources/brlcad/presentation.xsl
  )
  distclean("${CMAKE_CURRENT_BINARY_DIR}/../resources/brlcad/presentation.xsl")
endif(BRLCAD_EXTRADOCS_PPT)

# Files for PDF products
if(BRLCAD_EXTRADOCS_PDF)
  # Style sheet for XSLT transformation to PDF
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/../resources/brlcad/brlcad-presentation-fo-stylesheet.xsl.in
    ${CMAKE_CURRENT_BINARY_DIR}/../resources/brlcad/brlcad-presentation-fo-stylesheet.xsl
  )
  distclean("${CMAKE_CURRENT_BINARY_DIR}/../resources/brlcad/brlcad-presentation-fo-stylesheet.xsl")
endif(BRLCAD_EXTRADOCS_PDF)

# For HTML and FO (FO is an intermediate file used in the
# XML->PDF transformation) we use variables to hold the full
# stylesheet path.
set(XSL_PHP_STYLESHEET "${CMAKE_CURRENT_BINARY_DIR}/../resources/brlcad/wordpress.xsl")
set(XSL_HTML_STYLESHEET "${CMAKE_CURRENT_BINARY_DIR}/../resources/brlcad/brlcad-presentation-xhtml-stylesheet.xsl")
set(XSL_PPT_STYLESHEET "${CMAKE_CURRENT_BINARY_DIR}/../resources/brlcad/presentation.xsl")
set(XSL_FO_STYLESHEET "${CMAKE_CURRENT_BINARY_DIR}/../resources/brlcad/brlcad-presentation-fo-stylesheet.xsl")

set(
  presentations_EN
  intro-to-tcltk.xml
  brlcad-app-devel.xml
  Introduction-to-tcltk.xml
  Introduction-brlcad-app-devel.xml
)

set(
  presentations_EN_IMAGES
  images/copy-sph-part-left.png
  images/brlcad_app_devel.png
  images/intro_tcl_tk.png
  images/copy-sph-part-right.png
  images/mged-defaults.png
  images/special-character-interpretation.png
  images/tk-based-gui-for-mged.png
)
if(TARGET mged OR TARGET qged OR TARGET archer)
  if(BRLCAD_EXTRADOCS)
    add_doc(presentations_EN_IMAGES html/presentations/images)
    add_docbook("HTML;PHP;PPT;PDF" presentations_EN presentations presentations_EN_IMAGES_cp)
  endif(BRLCAD_EXTRADOCS)
endif(TARGET mged OR TARGET qged OR TARGET archer)

cmakefiles(
  README
  CMakeLists.txt
  ${presentations_EN}
  ${presentations_EN_IMAGES}
)

# Local Variables:
# tab-width: 8
# mode: cmake
# indent-tabs-mode: t
# End:
# ex: shiftwidth=2 tabstop=8
