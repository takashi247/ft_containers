NAME		:= a.out

CXX			:= c++

CXXFLAGS	:= -Wall -Wextra -Werror -MMD -MP -std=c++98 -pedantic

CXXFLAGS_SUB	:= -Wall -Wextra -Werror -std=c++98 -pedantic

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

TEST		:= -D TEST=1

LEAKS   := -D LEAKS=1

STL			:= -D STL=1

TIME		:= -D TIME=1

all:		$(NAME)

$(NAME):	$(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@

$(OBJSDIR)%.o: %.cpp
	@if [ ! -d $(OBJSDIR) ]; then mkdir $(OBJSDIR); fi
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) $(OBJSDIR)
	$(RM) *.dSYM
	$(RM) *.txt

fclean:		clean
	$(RM) $(NAME)

re:			fclean all

debug:		CXXFLAGS += $(DEBUG)
debug:		re

stl:			CXXFLAGS += $(STL)
stl:			re

test:
	make -B CXXFLAGS="$(CXXFLAGS)"
	./$(NAME) > ft_out.txt 2> ft_err.txt
	make -B CXXFLAGS="$(CXXFLAGS) $(STL)"
	./$(NAME) > stl_out.txt 2> stl_err.txt
	diff ft_out.txt stl_out.txt
	cat ft_err.txt
	cat stl_err.txt

time:
	make -B CXXFLAGS="$(CXXFLAGS) $(TIME)"
	./$(NAME) > ft_out.txt 2> ft_err.txt
	make -B CXXFLAGS="$(CXXFLAGS) $(TIME) $(STL)"
	./$(NAME) > stl_out.txt 2> stl_err.txt
#diff retuns 1 if there are any differences
	diff ft_out.txt stl_out.txt
	cat ft_err.txt
	cat stl_err.txt

subject:		srcs/main_subject.cpp
	$(CXX) $(CXXFLAGS_SUB) $(INCLUDE) $<
	./$(NAME) 42 > ft_out.txt 2> ft_err.txt
	$(CXX) $(CXXFLAGS_SUB) $(STL) $(INCLUDE) $<
	./$(NAME) 42 > stl_out.txt 2> stl_err.txt
	diff ft_out.txt stl_out.txt
	cat ft_err.txt
	cat stl_err.txt

leaks:		CXXFLAGS += $(LEAKS)
leaks:		test

-include $(DEPS)

.PHONY:		all clean fclean re debug time test leaks stl
