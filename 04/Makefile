NAME = toupper-bench
SRCS = $(wildcard *.cc)
OBJS = $(subst .cc,.o,$(SRCS))

CXXFLAGS ?= -O3
CXXFLAGS += -std=c++14 -Wall -Wextra -pedantic -fno-omit-frame-pointer

GBENCH_DIR := $(firstword $(wildcard benchmark/*))
ifeq (,$(GBENCH_DIR))
	# Use system google benchmark lib
	LDFLAGS += -lbenchmark
else
	# Use local google benchmark lib
	CXXFLAGS += -Ibenchmark/include/
	LDFLAGS += -Lbenchmark/bin/src/ -lbenchmark
endif
LDFLAGS += -lpthread


$(NAME): $(OBJS)
	$(CXX) $^ $(LDFLAGS) $(CXXFLAGS) -o $@


.PHONY: clean distclean run bench benchnice

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(NAME)

run: $(NAME)
	./$(NAME)

BENCHARGS = --benchmark_out_format=csv --benchmark_out=data/results-toupper-$(shell uname -m).csv
bench: $(NAME)
	./$(NAME) $(BENCHARGS)

benchnice: $(NAME)
	sudo nice -n -10 ./$(NAME) $(BENCHARGS)
