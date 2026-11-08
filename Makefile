srcdir   := src
builddir := build
incdir   := include
libftdir := libft
testdir  := tests

CC       := gcc
CFLAGS   := -O2 -Wall -Wextra -Werror -fPIC -I$(incdir) -I$(libftdir)/$(incdir)
LDFLAGS  := -shared -lpthread

ifeq ($(HOSTTYPE),)
    HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME     := libft_malloc_$(HOSTTYPE).so
LINK     := libft_malloc.so

headers  := $(incdir)/malloc.h

routines := malloc
objs     := $(routines:%=$(builddir)/%.o)
libft    := $(libftdir)/libft.a

tests     := tst-malloc
test-bins := $(tests:%=$(builddir)/%)
test-outs := $(tests:%=$(builddir)/%.out)

.PHONY: all clean fclean re $(libft) tests check tests-clean

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

$(builddir)/tst-%: $(testdir)/tst-%.c $(testdir)/test-skeleton.c $(LINK) | $(builddir)
	$(CC) -Wall -Wextra -I$(incdir) -I$(testdir) $< -L. -lft_malloc -o $@

tests: $(NAME) $(LINK) $(test-bins)

check: $(NAME) $(LINK) $(test-outs)
	@echo "---------------------------------------"
	@echo " All tests executed successfully."
	@echo "---------------------------------------"

$(builddir)/%.out: $(builddir)/%
	LD_PRELOAD=./$(LINK) ./$< > $@ 2>&1 || (cat $@ && exit 1)

tests-clean:
	rm -f $(test-bins) $(test-outs)

clean: tests-clean
	$(MAKE) -C $(libftdir) clean
	rm -rf $(builddir)

fclean: clean
	$(MAKE) -C $(libftdir) fclean
	rm -f $(NAME) $(LINK)

re: fclean all
