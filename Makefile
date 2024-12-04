NAME = Server

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -g 
SRCS =  Server.cpp Client.cpp Channel.cpp Tools.cpp main.cpp \
		Commands/Nick.cpp \
		Commands/Join.cpp \
		Commands/Privmsg.cpp \
		Commands/user.cpp \
		Commands/Quit.cpp

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
