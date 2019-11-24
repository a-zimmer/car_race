CPPSOURCES = $(wildcard *.cpp)

ifeq (${CPU}, armhf)
    FLAGS += -marm -march=armv6 -mfpu=vfp -mfloat-abi=hard
else ifeq (${CPU}, arm)
    FLAGS += -marm -march=armv6 -mfpu=vfp -mfloat-abi=softfp
else ifeq (${CPU}, x86)
    #FLAGS += -m32
else
    override CPU = x86_64
    #FLAGS += -m64
endif

ifeq (${CONFIG}, Debug)
    #FLAGS += -g
    SUFFIX = L
else
    override CONFIG = Release
    #FLAGS += -O2
    SUFFIX =
endif

SOURCE_FILES = \
    common/texture.cpp \
    common/shader.cpp \
    common/FMOD/common.cpp \
    common/FMOD/common_platform.cpp \
    common/text2D.cpp \
    common/song.cpp

INCLUDE_DIRS = \
    -I common/FMOD/lowlevel/inc

LOWLEVEL_LIB = common/FMOD/lowlevel/lib/${CPU}/libfmod${SUFFIX}.so

Target: carRace

all:	Target

carRace: $(CPPSOURCES:.cpp=.o)
	g++ -pthread $<  ${SOURCE_FILES} ${FLAGS} -o $@ -lAntTweakBar -lglfw -lGLEW -lGL -Wl,-rpath=\$$ORIGIN/$(dir ${LOWLEVEL_LIB}) ${LOWLEVEL_LIB} ${INCLUDE_DIRS}
   

run:
	./carRace
