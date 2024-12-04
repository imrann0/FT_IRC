NAME = Server

CXX = c++
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
		sources/Commands/Part.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
