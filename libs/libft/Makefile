srcdir   := src
builddir := build
incdir   := include

CC       := cc
CFLAGS   := -Wall -Wextra -Werror -I$(incdir)
AR       := ar
ARFLAGS  := rcs

NAME     := libft.a

headers  := $(incdir)/libft.h

routines := memset
objs     := $(routines:%=$(builddir)/%.o)

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(objs)
	$(AR) $(ARFLAGS) $@ $^

$(objs): $(builddir)/%.o: $(srcdir)/%.c $(headers) | $(builddir)
	$(CC) $(CFLAGS) -c $< -o $@

$(builddir):
	mkdir -p $(builddir)

clean:
	rm -rf $(builddir)

fclean: clean
	rm -f $(NAME)

re: fclean all
