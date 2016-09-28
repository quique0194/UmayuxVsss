#-------------------------------------------------
#
# Project created by QtCreator 2016-09-20T16:42:35
#
#-------------------------------------------------

QT       += core gui
QT       += network
QMAKE_CXXFLAGS += -O3

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = UMAYUX-VSSS
TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    readcamerathread.cpp \
    cutvideowidget.cpp \
    easydisablebtn.cpp \
    selectareavideowidget.cpp \
    showimagewidget.cpp \
    calibration.cpp \
    showinrangewidget.cpp \
    calibrationhandler.cpp \
    showvisionwidget.cpp \
    asynccmd.cpp \
    cmdoutput.cpp

HEADERS  += mainwindow.h \
    readcamerathread.h \
    cutvideowidget.h \
    easydisablebtn.h \
    selectareavideowidget.h \
    showimagewidget.h \
    calibration.h \
    showinrangewidget.h \
    calibrationhandler.h \
    showvisionwidget.h \
    asynccmd.h \
    cmdoutput.h

FORMS    += mainwindow.ui


BASEOPENCVPATH = C:\opencv\mingwQT
OPENCVVERSION = 310
INCLUDEPATH += $${BASEOPENCVPATH}\install\include

LIBS += $${BASEOPENCVPATH}\bin\libopencv_calib3d$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_core$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_features2d$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_flann$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_highgui$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_imgcodecs$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_imgproc$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_ml$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_objdetect$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_photo$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_shape$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_stitching$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_superres$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_video$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_videoio$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\libopencv_videostab$${OPENCVVERSION}.dll
LIBS += $${BASEOPENCVPATH}\bin\opencv_ffmpeg$${OPENCVVERSION}.dll
