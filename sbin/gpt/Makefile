# $NetBSD: Makefile,v 1.13 2014/12/29 16:27:06 christos Exp $
# $FreeBSD: src/sbin/gpt/Makefile,v 1.7 2005/09/01 02:49:20 marcel Exp $

PROG=	gpt
SRCS=	add.c biosboot.c create.c destroy.c gpt.c label.c map.c \
	migrate.c recover.c remove.c resize.c resizedisk.c \
	set.c show.c type.c unset.c gpt_uuid.c
MAN=	gpt.8

.if (${HOSTPROG:U} == "")
SRCS+= backup.c restore.c
LDADD+=	-lprop -lutil
DPADD+= ${LIBPROP} ${LIBUTIL}

.if ${USE_DRVCTL:Uno} == "yes"
CPPFLAGS+=-DUSE_DRVCTL
SRCS+=drvctl.c
.else
.PATH: ${.CURDIR}/../fsck
CPPFLAGS+=-I${.CURDIR}/../fsck
SRCS+=partutil.c
.endif
.endif


.include <bsd.prog.mk>
