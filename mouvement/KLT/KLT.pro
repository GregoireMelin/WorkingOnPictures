MOC_DIR = .moc
OBJECTS_DIR = .obj

TEMPLATE = app
CONFIG += qt warn_on release thread
SOURCES = KLT.cpp
TARGET = KLT
CONFIG -= app_bundle

INCLUDEPATH += /usr/include/opencv2/

LIBS = -L/usr/local/lib/ -lopencv_highgui -lopencv_imgproc -lopencv_core -lopencv_video 
