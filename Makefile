NAME = Server

CXX = c++
OBJ_DIR = .objs

CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -IIncludes
SRCS =  sources/Server.cpp \
		sources/Client.cpp \
		sources/Channel.cpp \
		sources/Tools.cpp \
		sources/main.cpp \
		sources/yolla.cpp \
		sources/Commands/Nick.cpp \
		sources/Commands/Join.cpp \
		sources/Commands/Privmsg.cpp \
		sources/Commands/user.cpp \
		sources/Commands/Quit.cpp \
		sources/Commands/pass.cpp \
		sources/Commands/Part.cpp

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
