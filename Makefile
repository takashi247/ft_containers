NAME		:= a.out

CXX			:= clang++

CXXFLAGS	:= -Wall -Wextra -Werror -MMD -MP -std=c++98 -pedantic

SRCSDIR		:= srcs/

OBJSDIR		:= objs/

SRCS		:= main.cpp

OBJS		:= $(addprefix $(OBJSDIR), $(SRCS:.cpp=.o))

DEPS		:= $(OBJS:.o=.d)

SRCS		:= $(addprefix $(SRCSDIR), $(SRCS))

VPATH		:= $(SRCSDIR)

INCLUDE		:= -I./include

RM			:= rm -rf

DEBUG		:= -g -fsanitize=address

SHADOW	:= -Wshadow

TEST		:= -D TEST=1

LEAKS   := -D LEAKS=1

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@

$(OBJSDIR)%.o: %.cpp
	@if [ ! -d $(OBJSDIR) ]; then mkdir $(OBJSDIR); fi
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) $(OBJSDIR)
	$(RM) *.dSYM

fclean:		clean
	$(RM) $(NAME)

re:			fclean all

debug:		CXXFLAGS += $(DEBUG)
debug:		re

shadow:		CXXFLAGS += $(SHADOW)
shadow:		re

test:			CXXFLAGS += $(TEST)
test:			re

leaks:		CXXFLAGS += $(TEST) $(LEAKS)
leaks:		re

-include $(DEPS)

.PHONY:		all clean fclean re debug shadow test leaks
