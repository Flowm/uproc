NAME = cache-bench
SRCS = $(wildcard *.cc)
OBJS = $(subst .cc,.o,$(SRCS))

CXXFLAGS ?= -O3
CXXFLAGS += -std=c++14 -Wall -Wextra -pedantic -Ibenchmark/include/ -fno-omit-frame-pointer -Wno-unused-function
LDFLAGS = -Lbenchmark/bin/src/ -lbenchmark -lpthread

$(NAME): $(OBJS)
	$(CXX) $^ $(LDFLAGS) $(CXXFLAGS) -o $@

.PHONY: clean distclean run record

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(NAME)

run: $(NAME)
	./$(NAME)

bench: $(NAME)
	./$(NAME) --benchmark_out_format=csv --benchmark_out=results-cache-bench.csv

# use root access to assign such a niceness!
benchnice: $(NAME)
	sudo nice -n -10 ./$(NAME) --benchmark_out_format=csv --benchmark_out=results-cache-bench.csv
