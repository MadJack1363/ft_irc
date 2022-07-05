######################################
#              COMMANDS              #
######################################
CXX			= c++
MKDIR		= mkdir -p
RM			= rm -rf

######################################
#             EXECUTABLE             #
######################################
NAME		= ircserv

#######################################
#             DIRECTORIES             #
#######################################
OBJ_DIR		= objs/
INC_DIR		= includes/
SRC_DIR		= $(shell find srcs -type d)

vpath %.cpp $(foreach dir, $(SRC_DIR), $(dir):)

######################################
#            SOURCE FILES            #
######################################
SRC				=	\
					main.cpp	\
					Server.cpp	\
					User.cpp	\
					Channel.cpp

######################################
#            OBJECT FILES            #
######################################
OBJ			= ${SRC:.cpp=.o}
OBJ			:= ${addprefix ${OBJ_DIR}, ${OBJ}}

DEP			= ${OBJ:.o=.d}

#######################################
#                FLAGS                #
#######################################
CXXFLAGS	=	-c
CXXFLAGS	+=	-Wall -Wextra# -Werror
CXXFLAGS	+=	-Wshadow
CXXFLAGS	+=	-std=c++98
CXXFLAGS	+=	-MMD -MP
CXXFLAGS	+=	-I${INC_DIR}
# CXXFLAGS	+=	-Weffc++ -pedantic

LDFLAGS		=

ifeq (${DEBUG}, 1)
	CXXFLAGS	+=	-g
	# CXXFLAGS	+=	-fstandalone-debug
	CXXFLAGS	+=	-DDEBUG
else \
ifeq (${DEBUG}, 2)
	CXXFLAGS	+=	-g
	CXXFLAGS	+=	-DDBG
	CXXFLAGS	+=	-fsanitize=address
	LDFLAGS		+=	-fsanitize=address
endif

#######################################
#                RULES                #
#######################################
.PHONY: all clean fclean re fre

${NAME}: ${OBJ}
	${CXX} ${OUTPUT_OPTION} ${OBJ} ${LDFLAGS}

all: ${NAME}

-include ${DEP}

${OBJ_DIR}%.o: %.cpp | ${OBJ_DIR}
	${CXX} -c ${OUTPUT_OPTION} ${CXXFLAGS} $<

${OBJ_DIR}:
	${MKDIR} ${@D}

clean:
	${RM} ${OBJ_DIR} ${NAME}

fclean:
	${RM} ${OBJ_DIR} ${NAME}

re: clean all

fre: fclean all

-include valgrind.mk
