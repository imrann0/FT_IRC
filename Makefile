NAME = ircserv

CXX = c++
OBJ_DIR = .objs

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -IIncludes
SRCS =  \
		sources/Channel.cpp \
		sources/Client.cpp \
		sources/Server.cpp \
		sources/Tools.cpp \
		sources/main.cpp \
		sources/Commands/Privmsg.cpp \
		sources/Commands/Invite.cpp \
		sources/Commands/Topic.cpp \
		sources/Commands/Nick.cpp \
		sources/Commands/Join.cpp \
		sources/Commands/user.cpp \
		sources/Commands/Quit.cpp \
		sources/Commands/pass.cpp \
		sources/Commands/Mode.cpp \
		sources/Commands/kick.cpp \
		sources/Commands/Part.cpp \
		sources/Bot/Wordl.cpp \
		sources/Bot/wordDB.cpp

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

all: $(NAME)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "Server Created"

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
