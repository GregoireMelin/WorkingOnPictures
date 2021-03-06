MOC_DIR = .moc
OBJECT_DIR = .obj

TEMPLATE   = app
CONFIG  += qt warn_on release thread
SOURCES  = panorama.cpp
TARGET   = panorama
CONFIG  -= app_bundle

enseirb {
INCLUDEPATH += /opt/opencv/include/opencv2
QMAKE_LFLAGS += -Wl,-R/opt/opencv/lib -L/opt/opencv/lib



}

LIBS += \
-lopencv_core \
-lopencv_highgui \
-lopencv_imgproc \
-lopencv_features2d \
-lopencv_nonfree \
-lopencv_calib3d
