######## Build options ########

verbose = 0

######## Build setup ########

# SRCROOT should always be the current directory
SRCROOT         = $(CURDIR)
FREERTOSROOT    = $(SRCROOT)/../../
APP             = $(notdir $(CURDIR))

# .o directory
ODIR            = obj

# Source VPATHS
VPATH           += $(FREERTOSROOT)/Source
VPATH	        += $(FREERTOSROOT)/Source/portable/MemMang
VPATH	        += $(FREERTOSROOT)/Source/portable/GCC/POSIX
VPATH           += $(FREERTOSROOT)/Demo
VPATH			+= $(FREERTOSROOT)/Project/FileIO
VPATH			+= $(FREERTOSROOT)/Project

# FreeRTOS Objects
C_FILES			+= croutine.c
C_FILES			+= event_groups.c
C_FILES			+= list.c
C_FILES			+= queue.c
C_FILES			+= tasks.c
C_FILES			+= timers.c

# portable Objects
C_FILES			+= heap_3.c
C_FILES			+= port.c

# Demo Objects
C_FILES			+= blocktim.c
C_FILES			+= countsem.c
C_FILES			+= GenQTest.c
C_FILES			+= QPeek.c
C_FILES			+= recmutex.c
C_FILES			+= BlockQ.c
C_FILES			+= death.c
C_FILES			+= dynamic.c
C_FILES			+= flop.c
C_FILES			+= integer.c
C_FILES			+= PollQ.c
C_FILES			+= semtest.c

C_FILES			+= AbortDelay.c
C_FILES			+= EventGroupsDemo.c
C_FILES			+= IntSemTest.c
C_FILES			+= QueueSet.c
C_FILES			+= QueueSetPolling.c
C_FILES			+= QueueOverwrite.c
C_FILES			+= TaskNotify.c
C_FILES			+= TimerDemo.c

# Main Object
C_FILES			+= main.c

# Include Paths
INCLUDES        += -I$(FREERTOSROOT)/Source/include
INCLUDES        += -I$(FREERTOSROOT)/Source/portable/GCC/POSIX/
INCLUDES        += -I$(FREERTOSROOT)/Demo/include
INCLUDES        += -I$(FREERTOSROOT)/Project

# Generate OBJS names
OBJS = $(patsubst %.c,%.o,$(C_FILES))

######## C Flags ########

# Warnings
CWARNS += -W
CWARNS += -Wall
#CWARNS += -Werror
CWARNS += -Wextra
CWARNS += -Wformat
CWARNS += -Wmissing-braces
CWARNS += -Wno-cast-align
CWARNS += -Wparentheses
CWARNS += -Wshadow
CWARNS += -Wno-sign-compare
CWARNS += -Wswitch
CWARNS += -Wuninitialized
CWARNS += -Wunknown-pragmas
CWARNS += -Wunused-function
CWARNS += -Wunused-label
CWARNS += -Wunused-parameter
CWARNS += -Wunused-value
CWARNS += -Wunused-variable
CWARNS += -Wmissing-prototypes

#CWARNS += -Wno-unused-function

CFLAGS += -m32
CFLAGS += -DDEBUG=1
#CFLAGS += -g -DUSE_STDIO=1 -D__GCC_POSIX__=1
CFLAGS += -g -UUSE_STDIO -D__GCC_POSIX__=1
ifneq ($(shell uname), Darwin)
CFLAGS += -pthread
endif

# MAX_NUMBER_OF_TASKS = max pthreads used in the POSIX port. 
# Default value is 64 (_POSIX_THREAD_THREADS_MAX), the minimum number required by POSIX.
CFLAGS += -DMAX_NUMBER_OF_TASKS=300

CFLAGS += $(INCLUDES) $(CWARNS) -O2

######## Makefile targets ########

# Rules
.PHONY : all
all: setup $(APP)

.PHONY : setup
setup:
# Make obj directory
	@mkdir -p $(ODIR)

# Fix to place .o files in ODIR
_OBJS = $(patsubst %,$(ODIR)/%,$(OBJS))

$(ODIR)/%.o: %.c
# If verbose, print gcc execution, else hide
ifeq ($(verbose),1)
	@echo ">> Compiling $<"
	$(CC) $(CFLAGS) -c -o $@ $<
else
	@echo ">> Compiling $(notdir $<)"
	@$(CC) $(CFLAGS) -c -o $@ $<
endif

$(APP): $(_OBJS)
	@echo ">> Linking $@..."
ifeq ($(verbose),1)
	$(CC) $(CFLAGS) $^ $(LINKFLAGS) $(LIBS) -o $@
else
	@$(CC) $(CFLAGS) $^ $(LINKFLAGS) $(LIBS) -o $@
endif

	@echo "-------------------------"
	@echo "BUILD COMPLETE: $@"
	@echo "-------------------------"

.PHONY : clean
clean:
	@-rm -rf $(ODIR) $(APP)
	@echo "--------------"
	@echo "CLEAN COMPLETE"
	@echo "--------------"
