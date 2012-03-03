CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		grub2-remenu.o

LIBS =

TARGET =	grub2-remenu

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
