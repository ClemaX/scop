SYSINCS = /usr/include /usr/local/include

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	GLLDLIBS = GL GLEW glfw
endif
ifeq ($(UNAME_S),Darwin)
	BREW = $(shell dirname $(dir $(shell which brew)))
	
	ifneq ($(BREW), )
		GLINCDIR = $(dir $(shell find -L $(BREW)/include -maxdepth 1 -type d -name GL -print -quit))
		GLLIBDIR = $(dir $(shell find -L $(BREW)/lib -maxdepth 1 -type f -iname "libglfw*" -print -quit))
	endif

	ifeq ($(GLINCDIR), )
		$(error Could not find OpenGL headers!)
	endif

	ifeq ($(GLLIBDIR), )
		$(error Could not find OpenGL library!)
	endif
	
	GLCFLAGS = -I$(GLINCDIR)
	GLLDFLAGS = -L$(GLLIBDIR) -framework OpenGL

	GLLDLIBS = glew glfw
endif

ifeq ($(GLLDLIBS), )
	$(error Could not detect OS!)
endif
