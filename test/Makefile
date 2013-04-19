CXX = g++
CXXWARNINGS = -Wall -Wextra -Werror -Wno-unused-parameter
CXXSTYLE = -MMD -O3
CXXFLAGS = $(CXXWARNINGS) $(CXXSTYLE)

SOURCES = $(shell find . -type f -name '*.cpp')
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDS = $(SOURCES:.cpp=.d)

LDFLAGS = /usr/local/lib/libboost_program_options.a

EXEC = combencode

#######################################

all: $(EXEC)
depend: $(DEPENDS)
clean:
	@rm -f $(DEPENDS) $(OBJECTS) $(EXEC)

$(EXEC): $(OBJECTS)
	@echo "Linking    $@..."
	@$(CXX) -o $@ $(OBJECTS) $(LDFLAGS) 

%.o : %.cpp
	@echo "Compiling  $<..."
	@${CXX} -c ${CXXFLAGS} $< -o $@

-include $(DEPENDS)

