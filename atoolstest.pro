#*****************************************************************************
# Copyright 2015-2019 Alexander Barthel alex@littlenavmap.org
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#****************************************************************************

# =============================================================================
# Set these environment variables for configuration - do not change this .pro file
# =============================================================================
#
# ATOOLS_INC_PATH
# Optional. Path to atools include. Default is "../atools/src" if not set.
# Also reads *.qm translation files from "$ATOOLS_INC_PATH/..".
#
# ATOOLS_LIB_PATH
# Optional. Path to atools static library. Default is "../build-atools-$${CONF_TYPE}"
# ("../build-atools-$${CONF_TYPE}/$${CONF_TYPE}" on Windows) if not set.
#
# OPENSSL_PATH
# Windows: Base path of WinSSL 1.1.1 installation (https://slproweb.com/products/Win32OpenSSL.html).
#          Defaults to "../openssl-1.1.1d-win32-mingw" if empty.
# Linux:   Not used.
# macOS:   Not used.
#
# ATOOLS_GIT_PATH
# Optional. Path to GIT executable. Revision will be set to "UNKNOWN" if not set.
# Uses "git" on macOS and Linux as default if not set.
# Example: "C:\Git\bin\git"
#
# DEPLOY_BASE
# Optional. Target folder for "make deploy". Default is "../deploy" plus project name ($$TARGET_NAME).
#
# ATOOLS_QUIET
# Optional. Set this to "true" to avoid qmake messages.
#
# =============================================================================
# End of configuration documentation
# =============================================================================

QT += sql xml core widgets testlib network
QT -= gui

CONFIG += console testcase build_all c++14
CONFIG -= app_bundle debug_and_release debug_and_release_target

TARGET = atoolstest

TARGET_NAME=$$TARGET

TEMPLATE = app

# =======================================================================
# Copy environment variables into qmake variables

ATOOLS_INC_PATH=$$(ATOOLS_INC_PATH)
ATOOLS_LIB_PATH=$$(ATOOLS_LIB_PATH)
OPENSSL_PATH=$$(OPENSSL_PATH)
GIT_PATH=$$(ATOOLS_GIT_PATH)
DEPLOY_BASE=$$(DEPLOY_BASE)
QUIET=$$(ATOOLS_QUIET)

# =======================================================================
# Fill defaults for unset

CONFIG(debug, debug|release) : CONF_TYPE=debug
CONFIG(release, debug|release) : CONF_TYPE=release

isEmpty(DEPLOY_BASE) : DEPLOY_BASE=$$PWD/../deploy

isEmpty(ATOOLS_INC_PATH) : ATOOLS_INC_PATH=$$PWD/../atools/src
isEmpty(ATOOLS_LIB_PATH) : ATOOLS_LIB_PATH=$$PWD/../build-atools-$$CONF_TYPE

win32: isEmpty(OPENSSL_PATH) : OPENSSL_PATH=$$PWD/../openssl-1.1.1d-win32-mingw

# =======================================================================
# Set compiler flags and paths

unix:!macx {
  isEmpty(GIT_PATH) : GIT_PATH=git

  QMAKE_LFLAGS += -no-pie

  # Makes the shell script and setting LD_LIBRARY_PATH redundant
  QMAKE_RPATHDIR=.
  QMAKE_RPATHDIR+=./lib
}

win32 {
  LIBS += -L$$OPENSSL_PATH

  WINDEPLOY_FLAGS = --compiler-runtime
  CONFIG(debug, debug|release) : WINDEPLOY_FLAGS += --debug
  CONFIG(release, debug|release) : WINDEPLOY_FLAGS += --release
}

macx {
  isEmpty(GIT_PATH) : GIT_PATH=git

  # Compatibility down to OS X 10.10
  QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.10
}

isEmpty(GIT_PATH) {
  GIT_REVISION='\\"UNKNOWN\\"'
} else {
  GIT_REVISION='\\"$$system('$$GIT_PATH' rev-parse --short HEAD)\\"'
}

LIBS += -L$$ATOOLS_LIB_PATH -latools
PRE_TARGETDEPS += $$ATOOLS_LIB_PATH/libatools.a
DEPENDPATH += $$ATOOLS_INC_PATH
INCLUDEPATH += $$PWD/src $$ATOOLS_INC_PATH
DEFINES += GIT_REVISION=$$GIT_REVISION
#DEFINES += QT_NO_CAST_FROM_BYTEARRAY
DEFINES += QT_NO_CAST_TO_ASCII

# =======================================================================
# Print values when running qmake

!isEqual(QUIET, "true") {
message(-----------------------------------)
message(GIT_PATH: $$GIT_PATH)
message(GIT_REVISION: $$GIT_REVISION)
message(OPENSSL_PATH: $$OPENSSL_PATH)
message(ATOOLS_INC_PATH: $$ATOOLS_INC_PATH)
message(ATOOLS_LIB_PATH: $$ATOOLS_LIB_PATH)
message(DEPLOY_BASE: $$DEPLOY_BASE)
message(DEFINES: $$DEFINES)
message(INCLUDEPATH: $$INCLUDEPATH)
message(LIBS: $$LIBS)
message(TARGET_NAME: $$TARGET_NAME)
message(QT_INSTALL_PREFIX: $$[QT_INSTALL_PREFIX])
message(QT_INSTALL_LIBS: $$[QT_INSTALL_LIBS])
message(QT_INSTALL_PLUGINS: $$[QT_INSTALL_PLUGINS])
message(QT_INSTALL_TRANSLATIONS: $$[QT_INSTALL_TRANSLATIONS])
message(QT_INSTALL_BINS: $$[QT_INSTALL_BINS])
message(CONFIG: $$CONFIG)
message(-----------------------------------)
}

# =====================================================================
# Files

SOURCES += \
  src/tests/airspacetest.cpp \
  src/tests/calctest.cpp \
  src/tests/gribtest.cpp \
  src/tests/metartest.cpp \
  src/tests/geotest.cpp \
  src/tests/dtmtest.cpp \
  src/tests/flightplantest.cpp \
  src/tests/routetest.cpp \
  src/tests/scenerycfgtest.cpp \
  src/tests/magdectest.cpp \
  src/tests/spatialtest.cpp \
  src/tests/updatetest.cpp \
  src/tests/stringtest.cpp \
  src/tests/versiontest.cpp \
  src/tests/onlinetest.cpp \
  src/tests/perftest.cpp \
  src/main.cpp

HEADERS += \
  src/tests/airspacetest.h \
  src/tests/calctest.h \
  src/tests/gribtest.h \
  src/tests/metartest.h \
  src/tests/geotest.h \
  src/tests/dtmtest.h \
  src/tests/flightplantest.h \
  src/tests/routetest.h \
  src/tests/scenerycfgtest.h \
  src/tests/magdectest.h \
  src/tests/spatialtest.h \
  src/tests/updatetest.h \
  src/tests/stringtest.h \
  src/tests/versiontest.h \
  src/tests/onlinetest.h \
  src/tests/perftest.h

RESOURCES += \
    resources.qrc

OTHER_FILES += \
  $$files(testdata/*, true) \
  .travis.yml \
  .gitignore \
  uncrustify.cfg \
  BUILD.txt \
  README.txt \
  LICENSE.txt

# =====================================================================
# Local deployment commands for development

unix:!macx {
  copydata.commands = cp -Rvf $$PWD/testdata $$OUT_PWD
}

# Linux specific deploy target
unix:!macx {
  DEPLOY_DIR=\"$$DEPLOY_BASE/$$TARGET_NAME\"
  DEPLOY_DIR_LIB=\"$$DEPLOY_BASE/$$TARGET_NAME/lib\"

  deploy.commands = rm -Rfv $$DEPLOY_DIR &&
  deploy.commands += mkdir -pv $$DEPLOY_DIR_LIB &&
  deploy.commands += mkdir -pv $$DEPLOY_DIR_LIB/sqldrivers &&
  deploy.commands += cp -Rvf $$OUT_PWD/atoolstest $$DEPLOY_DIR &&
  deploy.commands += cp -Rvf $$OUT_PWD/testdata $$DEPLOY_DIR &&
  deploy.commands += cp -vf $$PWD/CHANGELOG.txt $$DEPLOY_DIR &&
  deploy.commands += cp -vf $$PWD/README.txt $$DEPLOY_DIR &&
  deploy.commands += cp -vf $$PWD/LICENSE.txt $$DEPLOY_DIR &&
  deploy.commands += cp -vfa $$[QT_INSTALL_PLUGINS]/sqldrivers/libqsqlite.so*  $$DEPLOY_DIR_LIB/sqldrivers &&
  exists(/usr/lib/x86_64-linux-gnu/libssl.so) : deploy.commands += cp -vfaL /usr/lib/x86_64-linux-gnu/libssl.so $${DEPLOY_DIR_LIB}/libssl.so &&
  exists(/usr/lib/x86_64-linux-gnu/libcrypto.so) : deploy.commands += cp -vfaL /usr/lib/x86_64-linux-gnu/libcrypto.so $${DEPLOY_DIR_LIB}/libcrypto.so &&
  deploy.commands += cp -vfa $$[QT_INSTALL_LIBS]/libicudata.so*  $$DEPLOY_DIR_LIB &&
  deploy.commands += cp -vfa $$[QT_INSTALL_LIBS]/libicui18n.so*  $$DEPLOY_DIR_LIB &&
  deploy.commands += cp -vfa $$[QT_INSTALL_LIBS]/libicuuc.so*  $$DEPLOY_DIR_LIB &&
  deploy.commands += cp -vfa $$[QT_INSTALL_LIBS]/libQt5Gui.so*  $$DEPLOY_DIR_LIB &&
  deploy.commands += cp -vfa $$[QT_INSTALL_LIBS]/libQt5Core.so*  $$DEPLOY_DIR_LIB &&
  deploy.commands += cp -vfa $$[QT_INSTALL_LIBS]/libQt5Sql.so*  $$DEPLOY_DIR_LIB &&
  deploy.commands += cp -vfa $$[QT_INSTALL_LIBS]/libQt5Widgets.so*  $$DEPLOY_DIR_LIB &&
  deploy.commands += cp -vfa $$[QT_INSTALL_LIBS]/libQt5Test.so*  $$DEPLOY_DIR_LIB &&
  deploy.commands += cp -vfa $$[QT_INSTALL_LIBS]/libQt5Network.so*  $$DEPLOY_DIR_LIB
}

# Windows specific deploy target only for release builds
win32 {
  defineReplace(p){return ($$shell_quote($$shell_path($$1)))}

  deploy.commands = rmdir /s /q $$p($$DEPLOY_BASE/$$TARGET_NAME) &
  deploy.commands += mkdir $$p($$DEPLOY_BASE/$$TARGET_NAME/sqldrivers) &&
  deploy.commands += xcopy $$p($$OUT_PWD/atoolstest.exe) $$p($$DEPLOY_BASE/$$TARGET_NAME) &&
  deploy.commands += xcopy $$p($$OPENSSL_PATH/libcrypto-1_1.dll) $$p($$DEPLOY_BASE/$$TARGET_NAME) &&
  deploy.commands += xcopy $$p($$OPENSSL_PATH/libssl-1_1.dll) $$p($$DEPLOY_BASE/$$TARGET_NAME) &&
  deploy.commands += xcopy $$p($$PWD/CHANGELOG.txt) $$p($$DEPLOY_BASE/$$TARGET_NAME) &&
  deploy.commands += xcopy $$p($$PWD/README.txt) $$p($$DEPLOY_BASE/$$TARGET_NAME) &&
  deploy.commands += xcopy $$p($$PWD/LICENSE.txt) $$p($$DEPLOY_BASE/$$TARGET_NAME) &&
  deploy.commands += xcopy $$p($$[QT_INSTALL_BINS]/libgcc*.dll) $$p($$DEPLOY_BASE/$$TARGET_NAME) &&
  deploy.commands += xcopy $$p($$[QT_INSTALL_BINS]/libstdc*.dll) $$p($$DEPLOY_BASE/$$TARGET_NAME) &&
  deploy.commands += xcopy $$p($$[QT_INSTALL_BINS]/libwinpthread*.dll) $$p($$DEPLOY_BASE/$$TARGET_NAME) &&
  deploy.commands += xcopy /i /s /e /f /y $$p($$PWD/testdata) $$p($$DEPLOY_BASE/$$TARGET_NAME/testdata) &&
  deploy.commands += $$p($$[QT_INSTALL_BINS]/windeployqt) $$WINDEPLOY_FLAGS $$p($$DEPLOY_BASE/$$TARGET_NAME)
}

# =====================================================================
# Additional targets

# Need to copy data when compiling
all.depends = copydata

# Deploy needs compiling before
deploy.depends = all

QMAKE_EXTRA_TARGETS += deploy copydata all
