NAME = cache-bench
SRCS = $(wildcard *.cc)
OBJS = $(subst .cc,.o,$(SRCS))

CPPFLAGS = -O3 -std=c++14 -stdlib=libc++ -Wall -pedantic -Werror -fno-omit-frame-pointer
LDFLAGS =
LIBS = -lbenchmark -lpthread

$(NAME): $(OBJS)
	$(CXX) $^ $(LDFLAGS) $(LIBS) -o $@

.PHONY: clean distclean run record

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(NAME)

run: $(NAME)
	./$(NAME)

record: $(NAME)
	sudo nice -n -10 ./$(NAME) --benchmark_out_format=csv --benchmark_out=results-cache-bench.csv
