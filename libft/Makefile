srcdir   := src
builddir := build
incdir   := include

CC       := gcc
CFLAGS   := -Wall -Wextra -Werror -I$(incdir)
AR       := ar
ARFLAGS  := rcs

NAME     := libft.a

headers  := $(incdir)/libft.h

srcs     := $(srcdir)/ft_memset.c

objs     := $(srcs:$(srcdir)/%.c=$(builddir)/%.o)

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
