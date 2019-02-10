#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jye <marvin@42.fr>                         +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/02/01 17:45:46 by jye               #+#    #+#              #
#    Updated: 2019/02/10 16:18:58 by jye              ###   ########.fr        #
#                                                                              #
#******************************************************************************#

CC = gcc
CFLAGS = -Wall -Wextra #-Werror

SRCS = addsection addsymbole dumpbin dumptext ft_qsort init_machfile \
	   init_machheader nm printsym

ONAME_1 = $(addsuffix .nm.o, $(SRCS))
ONAME_2 = $(addsuffix .ot.o, $(SRCS))
CNAME = $(addsuffix .c, $(SRCS))

OFOLDER = obj
HFOLDER = src
CFOLDER = src

CWPATH = $(addprefix $(CFOLDER)/, $(CNAME))
OWPATH_1 = $(addprefix $(OFOLDER)/, $(ONAME_1))
OWPATH_2 = $(addprefix $(OFOLDER)/, $(ONAME_2))

NAME1 = ft_nm
NAME2 = ft_otool

all: ft_printf/libftprintf.a libft/libft.a $(NAME1) $(NAME2)

$(NAME1): $(OWPATH_1)
	$(CC) -o $@ $^ -Lft_printf -lftprintf -Llibft -lft

$(NAME2): $(OWPATH_2)
	$(CC) -o $@ $^ -Lft_printf -lftprintf -Llibft -lft

ft_printf/libftprintf.a:
	make -C ft_printf

libft/libft.a:
	make -C libft

clean:
	make -C ft_printf clean
	make -C libft clean
	rm -rf $(OFOLDER)

fclean: clean
	make -C ft_printf fclean
	make -C libft fclean
	rm -rf $(NAME1) $(NAME2)

re: fclean all

$(OFOLDER)/%.ot.o : $(CFOLDER)/%.c
	@mkdir -p $(OFOLDER)
	$(CC) $(CFLAGS) -DOTOOL -Ilibft -Ift_printf/includes -c $^ -o $@

$(OFOLDER)/%.nm.o : $(CFOLDER)/%.c
	@mkdir -p $(OFOLDER)
	$(CC) $(CFLAGS) -DNM -Ilibft -Ift_printf/includes -c $^ -o $@

#.PHONY: are
