# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yolee <yolee@student.42seoul.kr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/31 01:14:02 by yolee             #+#    #+#              #
#    Updated: 2023/04/01 15:24:56 by yolee            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv
CC = c++
CXXFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -MMD

SRCS = Server.cpp \
	SocketData.cpp \
	utils.cpp \
	webserv.cpp \

OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.d)

all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(CXXFLAGS) $(CPPFLAGS) $(OBJS) -o $(NAME)

.cpp.o :
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

clean :
	$(RM) $(OBJS)
	$(RM) $(DEPS)

fclean :
	$(MAKE) clean
	$(RM) $(NAME)

re :
	$(MAKE) fclean
	$(MAKE) all
	
.PHONY : all clean fclean re

-include $(DEPS)


	