VG_OPT	=	--leak-check=full
VG_OPT	+=	--show-leak-kinds=all
VG_OPT	+=	--track-fds=yes
VG_OPT	+=	--log-file=ircserv.log

valgrind:
	${MAKE} ${NAME} DEBUG=1
	$@ ${VG_OPT} ./${NAME} ${PORT} ${PASSWORD}

.PHONY: valgrind
