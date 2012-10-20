# Makefile for compiling Player projects! 
# to run this script and copile your code type "make all clean" (no quotes)
# into the terminal, or "make all" if you don't want the .o files cleaned away.
# Jennifer Owen 28/05/2009
# jowen@cs.york.ac.uk

# change this stuff as you need to:

# the name you want to give the executable
TARGET = out

# the code files you want to compile
CFILES = Robot.cpp RobotCS.cpp RobotFA.cpp Agent.cpp CandidateSolution.cpp FateAgent.cpp Breeder.cpp Reaper.cpp Cupid.cpp Controller.cpp main.cpp

# the directory where your project is stored
PROJECT_DIR = .

#the folder where source files are stored
SRC = $(PROJECT_DIR)

# the folder where header files are stored
HEADERS = -I/usr/local/include/Stage-4.1/

# the folder where the Player header is stored
#PLAYER = /usr/local/include/player-3.1/libplayerc++

#############################################################
################## DON'T CHANGE CODE BELOW ##################
#############################################################

# which compiler to use 
CC = g++
STRIP = strip

# add lpthread to compile under ubuntu
LINKFLAGS = `pkg-config --libs playerc++`

# -Wall flag tells it to mention errors. -c flag tells it not to link
OBJECTFLAGS = `pkg-config --cflags playerc++` -c

# turns the names of the code files into names of objects. It replaces the suffix
# also can be done by $(CFILES:%.cc=%.cc) I think.
# OBJS = $(CFILES:.cc=.o)
OBJS = $(CFILES:%.cpp=$(SRC)/%.o)

# where to find all the header files. -I tells the compiler these folders 
# contain things that are included in your code
INCLUDES = -I$(PLAYER) -I$(HEADERS)

# This links the object files together into a binary. It's dependancy is
# the $(OBJS) bit, so that gets done first.
all: $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LINKFLAGS)
	$(STRIP) $(TARGET)

# this is run before the make. It puts together object files without linking
# them together into a binary executable        
# the object files are then dumped into the same folder as this makefile
$(OBJS): 
	$(CC) $(OBJECTFLAGS) $*.cpp 
# for every item in the OBJS list perform the above operation, replacing the $*.cpp with the OBJS item

# removes all the object files. Not automatically done by a call to make,
# need to type "make clean" (without the quotes) into the terminal.     
# or "make all clean"
clean:
	rm -f *.o *~
