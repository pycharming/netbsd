#	$NetBSD: Makefile.inc,v 1.13 2012/05/09 21:59:10 christos Exp $

.if !defined(BIND9_MAKEFILE_INC)
BIND9_MAKEFILE_INC=yes

#NAMED_DEBUG=1

USE_FORT?= yes	# network client/server

WARNS?=	1

CWARNFLAGS.clang+=	-Wno-unused-value -Wno-parentheses

.include <bsd.own.mk>

.if ${MKCRYPTO} == "no"
NAMED_USE_OPENSSL?=no
.else
NAMED_USE_OPENSSL?=yes
.endif

.if exists(${NETBSDSRCDIR}/sys/sys/atomic.h)
NAMED_USE_PTHREADS?=yes
.else
NAMED_USE_PTHREADS?=no
.endif

IDIST=		${NETBSDSRCDIR}/external/bsd/bind/dist
BIND_SRCDIR=	${NETBSDSRCDIR}/external/bsd/bind
BIND_HTMLDIR=	/usr/share/doc/html/bind9

.include "${IDIST}/version"

VERSION=${MAJORVER}.${MINORVER}.${PATCHVER}${RELEASETYPE}${RELEASEVER}
SYSCONFDIR=/etc
LOCALSTATEDIR=/var

CPPFLAGS+=-I${BIND_SRCDIR}/include \
	-I${IDIST} \
	-I${IDIST}/lib/dns/include \
	-I${IDIST}/lib/isc/include -I${IDIST}/lib/isc/unix/include \
	-I${IDIST}/lib/bind9/include \
	-I${IDIST}/lib/isccfg/include \
	-I${IDIST}/lib/isccc/include \
	-I${IDIST}/lib/lwres/include -I${IDIST}/lib/lwres/unix/include \
	-DNS_LOCALSTATEDIR=\"${LOCALSTATEDIR}\" \
	-DNS_SYSCONFDIR=\"${SYSCONFDIR}\" \
	-DSESSION_KEYFILE=\"${LOCALSTATEDIR}/run/named/session.key\" \
	-DVERSION=\"${VERSION}\" -DBIND9

.if (${USE_INET6} != "no")
CPPFLAGS+=	-DWANT_IPV6
.endif

.if defined(HAVE_GCC)
COPTS+=	-Wno-pointer-sign
.endif

.if defined(NAMED_DEBUG)
DBG=-g3 -gstabs
.endif

.if !defined(LIB) || empty(LIB)
# NOTE: the order of these libraries is important...
.if defined(NAMED_DEBUG)
LDADD+=		-lbind9_g -ldns_g -llwres_g -lisccfg_g -lisccc_g -lisc_g
.else
LDADD+=		-lbind9 -ldns -llwres -lisccfg -lisccc -lisc
DPADD+=		${LIBBIND9} ${LIBDNS} ${LIBLWRES}
DPADD+=		${LIBISCCFG} ${LIBISCCC} ${LIBISC}
.endif
.else
CPPFLAGS+= -DLIBINTERFACE=${LIBINTERFACE} \
	   -DLIBREVISION=${LIBREVISION} -DLIBAGE=${LIBAGE}
.endif
#CPPFLAGS+= -DUSE_MEMIMPREGISTER -DUSE_APPIMPREGISTER -DUSE_SOCKETIMPREGISTER \
#    -DUSE_TIMERIMPREGISTER

.if ${NAMED_USE_PTHREADS} == "yes"
# XXX: Not ready yet
# CPPFLAGS+=	-DISC_PLATFORM_USE_NATIVE_RWLOCKS
CPPFLAGS+=	-DISC_PLATFORM_USETHREADS
.if !defined (LIB) || empty(LIB)
LDADD+= -lpthread
DPADD+= ${LIBPTHREAD}
.else
LIBDPLIBS+=      pthread  ${NETBSDSRCDIR}/lib/libpthread
.endif
.endif

.if ${NAMED_USE_OPENSSL} == "yes"
CPPFLAGS+=-DOPENSSL
.if !defined (LIB) || empty(LIB)
LDADD+= -lcrypto
DPADD+= ${LIBCRYPTO}
.else
.if exists(${NETBSDSRCDIR}/crypto/external/bsd/openssl/lib/libcrypto)
LIBDPLIBS+=	crypto ${NETBSDSRCDIR}/crypto/external/bsd/openssl/lib/libcrypto
.else
LIBDPLIBS+=	crypto ${NETBSDSRCDIR}/lib/libcrypto
.endif
.endif
.endif

.if ${NAMED_USE_PTHREADS} == "yes"
CPPFLAGS+=-DISC_PLATFORM_USETHREADS -I${IDIST}/lib/isc/pthreads/include
.else
CPPFLAGS+=-I${IDIST}/lib/isc/nothreads/include
.endif

.if exists(${.PARSEDIR}/../Makefile.inc)
.include "${.PARSEDIR}/../Makefile.inc"
.endif
.endif