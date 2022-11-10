ERASE		=	\033[2K\r
GREY		=	\033[30m
RED			=	\033[31m
GREEN		=	\033[32m
YELLOW		=	\033[33m
BLUE		=	\033[34m
PINK		=	\033[35m
CYAN		=	\033[36m
WHITE		=	\033[37m
BOLD		=	\033[1m
UNDER		=	\033[4m
SUR			=	\033[7m
END			=	\033[0m

NAME		= ircserv
OBJS		= $(SRCS:%.cpp=%.o)
HEADER		= include/Client.hpp include/Server.hpp include/Channel.hpp include/Irc.hpp include/ExecutionManager.hpp
SRCS		= src/Server.cpp src/Command.cpp src/Client.cpp src/Utils.cpp src/Channel.cpp src/Main.cpp src/CommandChannel.cpp src/CommandClient.cpp src/ExecutionManager.cpp src/CommandMode.cpp
RM			= rm -f
CC			= clang++
CFLAGS		= -Wall -Werror -Wextra -std=c++98 -I.

all:				$(NAME)

$(NAME):			$(OBJS) $(HEADER)
					@printf "$(ERASE)"
					@$(CC) $(CFLAGS) $(OBJS) -o $@ -fsanitize=address -g3
					@echo "$(BOLD)$(GREEN)Compilation $(NAME) Succes !$(END)"

%.o: %.cpp $(HEADER)
					@$(CC) $(CFLAGS) -o $@ -c $<
					@printf "$(ERASE)$(YELLOW)$@$(END)"

clean :
					@rm -rf $(OBJS)
					@echo "$(RED)Clean $(NAME) Succes !$(END)"

fclean :			clean
					@rm -f $(NAME)
					@echo "$(RED)Fclean $(NAME) Succes !$(END)"

re:					fclean all

.PHONY:				all clean fclean re
