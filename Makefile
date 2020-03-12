CC  ?= gcc
CXX ?= g++
SRC  = src
OBJ  = obj
LIB  = libs

SPIDEREXEC = webspider
QUERYEXEC  = webquery
PICEXEC    = piccompare

WARNINGS   = -Wall -Wextra -pedantic
NOWARNINGS = -w
DEBUG      = -g

IDIRS = -I$(SRC) -I$(SRC)/lib/haut-html/include
LIBS  =  -lrt -lpthread -lhaut -lcurl -lstdc++fs -ljpeg -lm
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

# find = $(shell find $1 -type f -name $2 -print 2>/dev/null)
find = $(shell find $1 -type f ! -path $3 -name $2 -print 2>/dev/null)
findonly = $(shell find $1 -type f -path $3 -name $2 -print 2>/dev/null)



CSRCS := $(call find, $(SRC)/, "*.c", "$(SRC)/lib/*")
COBJS := $(CSRCS:%.c=$(OBJ)/%.o)

HAUTSRCS := $(call findonly, $(SRC)/, "*.c", "$(SRC)/lib/haut-html/*")
HAUTOBJS := $(HAUTSRCS:%.c=$(OBJ)/%.o)

PICSRCS := $(call findonly, $(SRC)/, "*.cpp", "$(SRC)/lib/piccompare/*")
PICOBJS := $(PICSRCS:%.cpp=$(OBJ)/%.o)

EXCL := $(SRC)/spider/main.cpp $(SRC)/query/main.cpp $(SRC)/pic/main.cpp
CXXSRCS := $(filter-out $(EXCL),$(call find, $(SRC)/, "*.cpp", "$(SRC)/lib/*"))
CXXOBJS := $(CXXSRCS:%.cpp=$(OBJ)/%.o)

CLEAR  = [0m
CYAN   = [1;36m
GREEN  = [1;32m
YELLOW = [1;33m
WHITE  = [1;37m

MAKEFLAGS = -j

FAST := $(filter fast,$(MAKECMDGOALS))

xoutofy = $(or $(eval PROCESSED := $(PROCESSED) .),$(info $(WHITE)[$(YELLOW)$(words $(PROCESSED))$(WHITE)] $1$(CLEAR)))

.PHONY: spider query pic debug fast git clean

fast: all

debug: all

all: spider query pic

spider: haut $(COBJS) $(CXXOBJS) $(OBJ)/$(SRC)/spider/main.o
	@$(call xoutofy,$(GREEN)Linking $(if $(FAST),fast,debug) $(SPIDEREXEC))
	$(CXX) $(if $(FAST),$(CXXFASTFLAGS),$(CXXFLAGS)) $(COBJS) $(CXXOBJS) $(OBJ)/$(SRC)/spider/main.o -o $(SPIDEREXEC) -lrt -lpthread -lhaut -lcurl -lstdc++fs $(LDIRS)

query: haut $(COBJS) $(CXXOBJS) $(OBJ)/$(SRC)/query/main.o
	@$(call xoutofy,$(GREEN)Linking $(if $(FAST),fast,debug) $(QUERYEXEC))
	$(CXX) $(if $(FAST),$(CXXFASTFLAGS),$(CXXFLAGS)) $(COBJS) $(CXXOBJS) $(OBJ)/$(SRC)/query/main.o -o $(QUERYEXEC) -lrt -lpthread -lhaut -lcurl -lstdc++fs $(LDIRS)

# Compiles piccompare executable
pic: $(PICOBJS) $(COBJS) $(CXXOBJS) $(OBJ)/$(SRC)/pic/main.o
	@$(call xoutofy,$(GREEN)Linking $(if $(FAST),fast,debug) $(PICEXEC))
	$(CXX) $(if $(FAST),$(CXXFASTFLAGS),$(CXXFLAGS)) $(PICOBJS) $(CXXOBJS) $(OBJ)/$(SRC)/pic/main.o -o $(PICEXEC) -ljpeg -lm -lstdc++fs

# Compiles haut library
haut: $(HAUTOBJS)
	@$(call xoutofy,$(YELLOW)Linking haut library)
	mkdir -p $(LIB)
	ar rcs $(LIB)/libhaut.a $(HAUTOBJS)

# Makes haut library warnings shut up!
$(OBJ)/$(SRC)/lib/haut-html/%.o: $(SRC)/lib/haut-html/%.c
	@$(call xoutofy,$(CYAN)Compiling $<)
	@mkdir -p $(dir $@)
	$(CC) $(if $(FAST),$(CFASTFLAGS),$(CFLAGS)) $(NOWARNINGS) -o $@ -c $<

# Makes sift library warnings shut up!
$(OBJ)/$(SRC)/lib/piccompare/%.o: $(SRC)/lib/piccompare/%.cpp
	@$(call xoutofy,$(CYAN)Compiling $<)
	@mkdir -p $(dir $@)
	$(CXX) $(if $(FAST),$(CXXFASTFLAGS),$(CXXFLAGS)) $(NOWARNINGS) -o $@ -c $<
$(OBJ)/$(SRC)/pic/main.o: $(SRC)/pic/main.cpp
	@$(call xoutofy,$(CYAN)Compiling $<)
	@mkdir -p $(dir $@)
	$(CXX) $(if $(FAST),$(CXXFASTFLAGS),$(CXXFLAGS)) $(NOWARNINGS) -o $@ -c $<

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
	@rm -rf $(OBJ) $(LIB) $(SPIDEREXEC) $(QUERYEXEC) $(PICEXEC)
	@echo Done!

git: clean
	git add .
	git commit
	git push

c: clean