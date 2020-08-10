QT += widgets
requires(qtConfig(filedialog))
qtHaveModule(printsupport): QT += printsupport
QMAKE_CXXFLAGS+= -openmp

HEADERS       = mainwindow.h \
                acedialog.h \
                embossfilterdialog.h \
                fdfilterdialog.h \
                fftw3.h \
                floatslider.h \
                imageprocess.h \
                mdichild.h \
                padding.h \
                sdfilterdialog.h \
                transform.h
SOURCES       = main.cpp \
                acedialog.cpp \
                embossfilterdialog.cpp \
                fdfilterdialog.cpp \
                imagepocess.cpp \
                mainwindow.cpp \
                mdichild.cpp \
                padding.cpp \
                sdfilterdialog.cpp \
                transform.cpp
RESOURCES     = \
    dip.qrc \
    res.qrc \
    res1.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/mainwindows/mdi
INSTALLS += target

win32: LIBS += -L$$PWD/./ -llibfftw3-3 -llibfftw3f-3 -llibfftw3l-3

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

TRANSLATIONS += $$PWD/languages/zh_CN.ts \
               $$PWD/languages/en_US.ts

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lfftw3.3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lfftw3.3
else:unix: LIBS += -L$$PWD/./ -lfftw3.3

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lfftw3f.3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lfftw3f.3
else:unix: LIBS += -L$$PWD/./ -lfftw3f.3

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lfftw3l.3
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lfftw3l.3
else:unix: LIBS += -L$$PWD/./ -lfftw3l.3

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
