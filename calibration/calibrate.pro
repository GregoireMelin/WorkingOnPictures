MOC_DIR = .moc
OBJECTS_DIR = .obj

TEMPLATE = app
CONFIG += qt warn_on release thread
SOURCES = camera_calibration.cpp
TARGET = camera_calibration
CONFIG -= app_bundle

INCLUDEPATH += /usr/local/include/opencv2/

LIBS = -L/usr/local/lib/ -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_calib3d
