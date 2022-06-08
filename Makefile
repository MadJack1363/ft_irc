NAME		=	ircserv

# --   Compilation flags  -- #
CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -MD -MMP -std=c++98

OBJ_DIR		=	objs
INC_DIR		=	includes
SRC_DIR		=	$(shell find srcs -type d)

vpath %.cpp $(foreach dir, $(SRC_DIR), $(dir):)

OBJS		=	$(addprefix $(OBJ_DIR)/, $(SRCS:%.cpp=%.o))
DEPS		=	$(OBJS:%.o=%.d)

INCS		=	color.h

SRCS		=	main.cpp		\


# --    Add DEBUG flags   -- #
DEBUG = $(shell env | grep DEBUG= | tr '=' ' ' | awk '{print $$2}')

ifeq ($(DEBUG), 1)
	CXXFLAGS += -g3
endif

# **************************************************************************** #

.PHONY: all
all : $(NAME)

# **************************************************************************** #

$(NAME): $(OBJS) $(INCS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

-include $(DEPS)
$(OBJ_DIR)/%.o: %.cpp $(INCS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR) :
	mkdir -p $@

# **************************************************************************** #

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR)

.PHONY: fclean
fclean: clean
	rm -f $(NAME)

.PHONY: re
re: fclean all
