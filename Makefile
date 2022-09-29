NAME		= ircserv
OBJS		= $(SRCS:.cpp=.o)
HEADER		= include/Command.hpp include/User.hpp include/Server.hpp include/Channel.hpp include/Irc.hpp
SRCS		= src/Server.cpp src/Command.cpp src/User.cpp src/Utils.cpp src/Channel.cpp src/Main.cpp src/Command_channel.cpp src/Command_user.cpp
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
