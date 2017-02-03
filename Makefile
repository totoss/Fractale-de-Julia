#definition des commandes
CXX = g++
RM = rm -f
TAR = tar
MKDIR = mkdir
CHMOD = chmod
CP = rsync -R

#options du compilateur
CXXFLAGS = -Wall
LDFLAGS = -lpthread -lopencv_core -lopencv_highgui -lopencv_imgproc -std=c++11

#fichiers et dossiers
PROGNAME = fractale
SRC = $(filter-out src/complex.cpp, $(wildcard src/*.cpp))
HEADERS= $(wildcard *.hpp)
OBJS = $(SRC:.cpp=.o)

PACKAGE=$(PROGNAME)
VERSION = 0.1-test
distdir = $(PACKAGE)-$(VERSION)
DISTFILES = $(SRC) Makefile $(HEADERS) 

all: $(PROGNAME)

$(PROGNAME): $(OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(PROGNAME) $(OBJS) *~

dist: distdir
	$(CHMOD) -R a+r $(distdir)
	$(TAR) zcvf $(distdir).tgz $(distdir)
	$(RM) -r $(distdir)

distdir: $(DISTFILES)
	$(RM) -r $(distdir)
	$(MKDIR) $(distdir)
	$(CHMOD) 777 $(distdir)
	$(CP) $(DISTFILES) $(distdir)
