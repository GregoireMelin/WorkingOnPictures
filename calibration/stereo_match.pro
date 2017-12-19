MOC_DIR = .moc
OBJECTS_DIR = .obj

TEMPLATE = app
CONFIG += qt debug  warn_on  thread
SOURCES = stereo_match.cpp
TARGET = stereo_match
CONFIG -= app_bundle

INCLUDEPATH += /usr/local/include/opencv2/

LIBS = -L/usr/local/lib/ -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_calib3d
