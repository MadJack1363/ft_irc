# tester avec g++ +  -Weffc++ -pedantic
# tester avec scan-build-12 make
######################################
#              COMMANDS              #
######################################
# CXX					= g++
CXX					= c++
MKDIR				= mkdir -p
RM					= rm -rf

######################################
#             EXECUTABLE             #
######################################
NAME				= ircserv

#######################################
#             DIRECTORIES             #
#######################################
OBJ_DIR				= objs/
INC_DIR				= includes
SRC_DIR				= $(shell find srcs -type d)

vpath %.cpp $(foreach dir, $(SRC_DIR), $(dir):)
######################################
#            SOURCE FILES            #
######################################
SRC				=	\
					main.cpp	\
					getPort.cpp	\

######################################
#            OBJECT FILES            #
######################################
OBJ				= ${SRC:.cpp=.o}
OBJ				:= ${addprefix ${OBJ_DIR}, ${OBJ}}

DEP				= ${OBJ:.o=.d}

#######################################
#                FLAGS                #
#######################################
CXXFLAGS			= -Wall -Wextra -Werror -std=c++98 -I${INC_DIR}
CXXFLAGS			+= -MMD -MP
# CXXFLAGS			+= -Weffc++ -pedantic

LDFLAGS			=

ifeq (${LEAK}, 1)
	CXXFLAGS	+= -fsanitize=address -g3
	LDFLAGS		+= -fsanitize=address
endif

#######################################
#                RULES                #
#######################################
.PHONY: all clean fclean re

${NAME}: ${OBJ}
	${CXX} ${OUTPUT_OPTION} ${OBJ} ${LDFLAGS}

all: ${NAME}

-include ${DEP}

${OBJ_DIR}%.o: %.cpp | ${OBJ_DIR}
	${CXX} -c ${OUTPUT_OPTION} ${CXXFLAGS} $<

${OBJ_DIR}:
	${MKDIR} ${@D}

clean:
	${RM} ${OBJ_DIR}

fclean:
	${RM} ${OBJ_DIR} ${NAME}

re: fclean all
