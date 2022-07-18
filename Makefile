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
SRC_DIR		= srcs/
OBJ_DIR		= objs/
INC_DIR		= includes/

######################################
#            SOURCE FILES            #
######################################
SRC				=	\
					${addprefix class/, 	\
						${addprefix cmd/,	\
							DIE.cpp			\
							JOIN.cpp		\
							KICK.cpp		\
							KILL.cpp		\
							MODE.cpp		\
							NICK.cpp		\
							OPER.cpp		\
							PART.cpp		\
							PASS.cpp		\
							PING.cpp		\
							PRIVMSG.cpp		\
							QUIT.cpp		\
							SET.cpp			\
							USER.cpp		\
						}					\
						Channel.cpp			\
						Config.cpp			\
						Server.cpp			\
						User.cpp			\
					}						\
					main.cpp

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
	CXXFLAGS	+=	-DDEBUG
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

${OBJ_DIR}%.o: ${SRC_DIR}%.cpp
	@${MKDIR} ${@D}
	${CXX} -c ${OUTPUT_OPTION} ${CXXFLAGS} $<

clean:
	${RM} ${OBJ_DIR} ${NAME}

fclean:
	${RM} ${OBJ_DIR} ${NAME}

re: clean all

fre: fclean all

-include valgrind.mk
