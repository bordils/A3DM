################################################################################
# Needed Qt packages and build plugin
################################################################################

CONFIG += qt opengl thread release
QT += opengl uitools 
LIBPATH += /assig/sgi/OM-6.3/lib
LIBS += -lOpenMeshCore -lOpenMeshTools -lglut -lGLU
INCLUDEPATH += . /assig/sgi/OM-6.3/include

HEADERS = MeshViewerWidget.hh MeshViewerWidgetT.hh QGLViewerWidget.hh taulaMC.hpp
SOURCES = meshviewer.cc MeshViewerWidgetT.cc QGLViewerWidget.cc taulaMC.cpp
