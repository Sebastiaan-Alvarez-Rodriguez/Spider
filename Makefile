CC  ?= gcc
CXX ?= g++
SRC  = src
OBJ  = obj
LIB  = libs

EXEC       = awc
WARNINGS   = -Wall -Wextra -pedantic
NOWARNINGS = -w
DEBUG      = -g

IDIRS = -I$(SRC) -I$(SRC)/haut-html/include
LIBS  =  -lrt -lpthread -lhaut -lcurl -lstdc++fs
LDIRS = -L$(LIB)

CFLAGS     = $(IDIRS) -std=c11 $(WARNINGS) $(DEBUG)
CFASTFLAGS = $(IDIRS) -std=c11 \
	-Ofast \
	-march=native \
	-ffast-math \
	-funsafe-math-optimizations \
	-fassociative-math \
	-freciprocal-math \
	-ffinite-math-only \
	-fno-signed-zeros \
	-fno-trapping-math \
	-funroll-loops

CXXFLAGS     = $(IDIRS) -std=c++17 $(WARNINGS) $(DEBUG)
CXXFASTFLAGS = $(IDIRS) -std=c++17 \
	-Ofast \
	-march=native \
	-ffast-math \
	-funsafe-math-optimizations \
	-fassociative-math \
	-freciprocal-math \
	-ffinite-math-only \
	-fno-signed-zeros \
	-fno-trapping-math \
	-funroll-loops

find = $(shell find $1 -type f -name $2 -print 2>/dev/null)

CSRCS := $(call find, $(SRC)/, "*.c")
COBJS := $(CSRCS:%.c=$(OBJ)/%.o)

CXXSRCS := $(call find, $(SRC)/, "*.cpp")
CXXOBJS := $(CXXSRCS:%.cpp=$(OBJ)/%.o)

CLEAR  = [0m
CYAN   = [1;36m
GREEN  = [1;32m
YELLOW = [1;33m
WHITE  = [1;37m

MAKEFLAGS = -j

FAST := $(filter fast,$(MAKECMDGOALS))

xoutofy = $(or $(eval PROCESSED := $(PROCESSED) .),$(info $(WHITE)[$(YELLOW)$(words $(PROCESSED))$(WHITE)] $1$(CLEAR)))

.PHONY: debug fast

# Requests object creation, links, builds debug executable
debug: lib $(COBJS) $(CXXOBJS)
	@$(call xoutofy,$(GREEN)Linking debug $(EXEC))
	$(CXX) $(CXXFLAGS) $(COBJS) $(CXXOBJS) -o $(EXEC) $(LIBS) $(LDIRS)

# Requests object creation, links, builds fast executable
fast: lib $(COBJS) $(CXXOBJS)
	@$(call xoutofy,$(GREEN)Linking fast $(EXEC))
	$(CXX) $(CXXFASTFLAGS) $(COBJS) $(CXXOBJS) -o $(EXEC) $(LIBS) $(LDIRS)

# Compiles haut library
lib: $(COBJS)
	@$(call xoutofy,$(YELLOW)Linking haut library)
	mkdir -p $(LIB)
	ar rcs $(LIB)/libhaut.a $(OBJ)/$(SRC)/haut-html/src/haut.o $(OBJ)/$(SRC)/haut-html/src/string_util.o $(OBJ)/$(SRC)/haut-html/src/state_machine.o

# Makes library warnings shut up!
$(OBJ)/$(SRC)/haut-html/%.o: $(SRC)/haut-html/%.c
	@$(call xoutofy,$(CYAN)Compiling $<)
	@mkdir -p $(dir $@)
	$(CC) $(if $(FAST),$(CFASTFLAGS),$(CFLAGS)) $(NOWARNINGS) -o $@ -c $<

# Compiles regular c files
$(OBJ)/%.o: %.c
	@$(call xoutofy,$(CYAN)Compiling $<)
	@mkdir -p $(dir $@)
	$(CC) $(if $(FAST),$(CFASTFLAGS),$(CFLAGS)) -o $@ -c $<

# Compiles regular cpp files
$(OBJ)/%.o: %.cpp
	@$(call xoutofy,$(CYAN)Compiling $<)
	@mkdir -p $(dir $@)
	$(CXX) $(if $(FAST),$(CXXFASTFLAGS),$(CXXFLAGS)) -o $@ -c $<

$(OBJ):
	mkdir -p $(OBJ)

clean:
	@echo Cleaning...
	@rm -rf $(OBJ) $(LIB) $(EXEC)
	@echo Done!

git: clean
	git add .
	git commit
	git push

c: clean