TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    RODS/RODS.cpp \
    RODS/basis.cpp \
    RODS/dof.cpp \
    RODS/dsystem.cpp \
    RODS/element/beamElastic.cpp \
    RODS/element/dashpot.cpp \
    RODS/element/dashpot2D.cpp \
    RODS/element/dashpotExp.cpp \
    RODS/element/element.cpp \
    RODS/element/element1D.cpp \
    RODS/element/element2D.cpp \
    RODS/element/frameElastic.cpp \
    RODS/element/inerter.cpp \
    RODS/element/inerter2D.cpp \
    RODS/element/slider.cpp \
    RODS/element/spis2.cpp \
    RODS/element/spring.cpp \
    RODS/element/spring2D.cpp \
    RODS/element/springBilinear.cpp \
    RODS/element/springBoucWen.cpp \
    RODS/element/springNonlinear.cpp \
    RODS/element/truss.cpp \
    RODS/element/trussElastic.cpp \
    RODS/line.cpp \
    RODS/material/SMABilinear.cpp \
    RODS/material/concreteTrilinear.cpp \
    RODS/material/elastic.cpp \
    RODS/material/elastoplastic.cpp \
    RODS/material/material.cpp \
    RODS/material/material1D.cpp \
    RODS/material/steelBilinear.cpp \
    RODS/node.cpp \
    RODS/recorder/dofRecorder.cpp \
    RODS/recorder/elementRecorder.cpp \
    RODS/recorder/recorder.cpp \
    RODS/section/section.cpp \
    RODS/section/sectionElasticBar.cpp \
    RODS/timeseries.cpp

HEADERS += \
    RODS/basis.h \
    RODS/dof.h \
    RODS/dsystem.h \
    RODS/element/beamElastic.h \
    RODS/element/dashpot.h \
    RODS/element/dashpot2D.h \
    RODS/element/dashpotExp.h \
    RODS/element/element.h \
    RODS/element/element1D.h \
    RODS/element/element2D.h \
    RODS/element/frameElastic.h \
    RODS/element/inerter.h \
    RODS/element/inerter2D.h \
    RODS/element/slider.h \
    RODS/element/spis2.h \
    RODS/element/spring.h \
    RODS/element/spring2D.h \
    RODS/element/springBilinear.h \
    RODS/element/springBoucWen.h \
    RODS/element/springNonlinear.h \
    RODS/element/truss.h \
    RODS/element/trussElastic.h \
    RODS/line.h \
    RODS/material/SMABilinear.h \
    RODS/material/concreteTrilinear.h \
    RODS/material/elastic.h \
    RODS/material/elastoplastic.h \
    RODS/material/material.h \
    RODS/material/material1D.h \
    RODS/material/steelBilinear.h \
    RODS/node.h \
    RODS/numeric.h \
    RODS/plot.h \
    RODS/recorder/dofRecorder.h \
    RODS/recorder/elementRecorder.h \
    RODS/recorder/recorder.h \
    RODS/section/section.h \
    RODS/section/sectionElasticBar.h \
    RODS/timeseries.h

INCLUDEPATH += RODS RODS/material RODS/element \
    RODS/section RODS/recorder

unix|win32: LIBS += -lopenblas
