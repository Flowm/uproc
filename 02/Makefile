NAME = cache-bench
SRCS = $(wildcard *.cc)
OBJS = $(subst .cc,.o,$(SRCS))

CPPFLAGS = -O3 -std=c++14 -stdlib=libc++ -Wall -pedantic -Werror -fno-omit-frame-pointer
LDFLAGS =
LIBS = -lbenchmark -lpthread

$(NAME): $(OBJS)
	$(CXX) $^ $(LDFLAGS) $(LIBS) -o $@

.PHONY: clean distclean run

clean:
	rm -f $(OBJS)

distclean: clean
	rm -f $(NAME)

run: $(NAME)
	./$(NAME)
