NAME = scop

# Compiler and linker
CC = clang
LD = clang
AR = ar

# Paths
SRCDIR = src
INCDIR = include
LIBDIR = lib

OBJDIR = obj
BINDIR = .

# Library dependencies
LIBS = $(addprefix $(LIBDIR)/, )

LIBDIRS = $(dir $(LIBS))
LIBINCS = $(addsuffix $(INCDIR), $(LIBDIRS))
LIBARS = $(notdir $(LIBS))

# Sources
INCS = $(LIBINCS) $(INCDIR)
SRCS = $(addprefix $(SRCDIR)/,\
	camera.c\
	face_cnt.c\
	face.c\
	file.c\
	main.c\
	matrix.c\
	object_cmd_op.c\
	object_cmd.c\
	object_load.c\
	object.c\
	scop_draw.c\
	scop_load.c\
	scop.c\
	shader.c\
	vector.c\
	vertex_cnt.c\
	vertex.c\
	window.c\
)

OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS = $(OBJS:.o=.d)

# Include directories
SYSINCS = /usr/include /usr/local/include

# OpenGL libraries
include gl.mk

# Flags
CFLAGS = -Wall -Wextra -Werror $(INCS:%=-I%) $(GLCFLAGS)
DFLAGS = -MT $@ -MMD -MP -MF $(OBJDIR)/$*.d
LDFLAGS = $(LIBDIRS:%=-L%) $(GLLDFLAGS)
LDLIBS = $(LIBARS:lib%.a=-l%) $(GLLDLIBS:%=-l%) -lm
ARFLAGS = -rcus

# Compiling commands
COMPILE.c = $(CC) $(DFLAGS) $(CFLAGS) -c
COMPILE.o = $(LD) $(LDFLAGS)
ARCHIVE.o = $(AR) $(ARFLAGS)

all: $(BINDIR)/$(NAME)

# Directories
$(OBJDIR) $(BINDIR):
	@echo "MK $@"
	mkdir -p "$@"

# Libraries
$(LIBS): %.a: FORCE
	make -C $(dir $@) NAME=$(@F)

# Objects
$(OBJS): $(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	@mkdir -p '$(@D)'
	@echo "CC $<"
	$(COMPILE.c) $< -o $@

# Dependencies
$(DEPS): $(OBJDIR)/%.d:
include $(wildcard $(DEPS))

# Binaries
$(BINDIR)/$(NAME): $(OBJS) $(LIBS) | $(BINDIR)
	@echo "LD $@"
	$(COMPILE.o) $^ -o $@ $(LDLIBS)
# Use	@echo "AR $@"
#		$(ARCHIVE.o) $@ $^
# instead when building a static library

debug: CFLAGS+=-DDEBUG -g3 -fsanitize=address
debug: LDFLAGS+=-g3 -fsanitize=address
debug: fclean $(BINDIR)/$(NAME)

clean:
	$(foreach dir, $(LIBDIRS),\
		echo "MK $(addprefix -C , $(dir)) $@" && make -C $(dir) $@ ; ):

	@rm -r "$(OBJDIR)" 2>/dev/null && echo "RM $(OBJDIR)" || :

fclean: clean
	$(foreach dir, $(LIBDIRS),\
		echo "MK $(addprefix -C, $(dir)) $@" && make -C $(dir) $@ ; ):

	@rm "$(BINDIR)/$(NAME)" 2>/dev/null && echo "RM $(BINDIR)/$(NAME)" || :
	@rmdir "$(BINDIR)" 2>/dev/null && echo "RM $(BINDIR)" || :

re: fclean all

FORCE: ;

.PHONY: all clean fclean re

# Assign a value to VERBOSE to enable verbose output
$(VERBOSE).SILENT:
