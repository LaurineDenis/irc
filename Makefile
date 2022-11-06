NAME		= ircserv
OBJS		= $(SRCS:.cpp=.o)
HEADER		= include/Command.hpp include/Client.hpp include/Server.hpp include/Channel.hpp include/Irc.hpp include/ExecutionManager.hpp
SRCS		= src/Server.cpp src/Command.cpp src/Client.cpp src/Utils.cpp src/Channel.cpp src/Main.cpp src/CommandChannel.cpp src/CommandClient.cpp src/ExecutionManager.cpp src/CommandMode.cpp
RM			= rm -f
CC			= clang++
CFLAGS		= -Wall -Werror -Wextra -std=c++98

all:
					@$(MAKE) $(NAME)

$(NAME): $(OBJS) $(HEADER)
					@printf "$(ERASE)"
					$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -fsanitize=address -g3
					@echo "$(BOLD)$(GREEN)Compilation $(NAME) Succes !$(END)"

clean :
					@rm -rf $(OBJS)
					@echo "$(RED)Clean $(NAME) Succes !$(END)"

fclean :			clean
					@rm -f $(NAME)
					@echo "$(RED)Fclean $(NAME) Succes !$(END)"

re:					fclean all
