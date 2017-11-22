MOC_DIR = .moc
OBJECT_DIR = .obj

TEMPLATE   = app
CONFIG  += qt warn_on release thread
SOURCES  = make-se.cpp
TARGET   = make-se
CONFIG  -= app_bundle

enseirb {
INCLUDEPATH += /opt/opencv/include/opencv2
QMAKE_LFLAGS += -Wl,-R/opt/opencv/lib -L/opt/opencv/lib
}

LIBS += \
-lopencv_core \
-lopencv_highgui \
-lopencv_imgproc
