prefix=/usr/local
libdir=$(prefix)/lib

CFLAGS=-g -fPIC -W -Wall -Wpointer-arith -Wno-unused-parameter
LDFLAGS=

VPATH=.

SRCS := utilsock.cpp utilsockopt.cpp utilnet.cpp\
	file.c time.c debug.c progressbar.c urlescape.c
OBJS := $(patsubst %.cpp,%.o, $(SRCS))
OBJS := $(patsubst %.c,%.o, $(OBJS))
HEADERS := $(patsubst %.cpp,%.h, $(SRCS))
HEADERS := $(patsubst %.c,%.h, $(HEADERS))

TARGET=libcommonutils.so
all:$(TARGET)

$(TARGET):$(OBJS)
	@echo "\033[32;1m#### Generate target $@ \033[0m"
	$(CC) -o $@ $^ -shared -Wl,-soname,$@ $(LDFLAGS)

%.o:%.cpp
	$(CXX) $(CFLAGS) -c  $^ -o $@

%.o:%.c
	$(CC) $(CFLAGS) -c  $^ -o $@

clean:
	$(RM) -r $(OBJS) $(TARGET)

install:
	mkdir -p $(prefix)/include/utils
	@for i in $(HEADERS); do\
	  [ -f $$i ] && cp $$(readlink -f $$i) $(prefix)/include/utils;\
	done
	cp $(shell readlink -f $(TARGET)) $(libdir)

uninstall:
	$(RM) $(libdir)/$(TARGET)
	$(RM) -r $(prefix)/include/utils
