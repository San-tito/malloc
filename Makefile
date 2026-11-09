srcdir   := src
builddir := build
incdir   := include
libsdir  := libs
libftdir := $(libsdir)/libft
testdir  := tests

CC       := cc
CFLAGS   := -Wall -Wextra -Werror -I$(incdir) -I$(libftdir)/$(incdir)
LDFLAGS  := -shared

ifeq ($(HOSTTYPE),)
    HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME     := libft_malloc_$(HOSTTYPE).so
LINK     := libft_malloc.so

headers  := $(incdir)/malloc.h

routines := malloc
objs     := $(routines:%=$(builddir)/%.o)

libft    := $(libftdir)/libft.a

tests     := test-malloc
test-bins := $(tests:%=$(builddir)/%)
test-outs := $(tests:%=$(builddir)/%.out)

.PHONY: all clean fclean re $(libft) tests check test-clean

all: $(NAME) $(LINK)

$(NAME): $(objs) $(libft)
	$(CC) $(objs) $(libft) $(LDFLAGS) -o $@

$(LINK): $(NAME)
	ln -sf $< $@

$(libft):
	$(MAKE) -C $(libftdir)

$(objs): $(builddir)/%.o: $(srcdir)/%.c $(headers) | $(builddir)
	$(CC) $(CFLAGS) -c $< -o $@

$(builddir):
	mkdir -p $(builddir)

$(builddir)/test-%: $(testdir)/test-%.c $(testdir)/test-skeleton.c $(LINK) | $(builddir)
	$(CC) -Wall -Wextra -Werror -I$(incdir) -I$(testdir) $< -L. -lft_malloc -o $@

tests: $(NAME) $(LINK) $(test-bins)

check: $(NAME) $(LINK) $(test-outs)
	@echo "---------------------------------------"
	@echo " All tests passed."
	@echo "---------------------------------------"

$(builddir)/%.out: $(builddir)/%
	LD_LIBRARY_PATH=. ./$< > $@ 2>&1 || (cat $@ && exit 1)

test-clean:
	rm -f $(test-bins) $(test-outs)

clean: test-clean
	$(MAKE) -C $(libftdir) clean
	rm -rf $(builddir)

fclean: clean
	$(MAKE) -C $(libftdir) fclean
	rm -f $(NAME) $(LINK)

re: fclean all
