#pragma once

#include <sys/types.h>

/*
 * to select alternate encoding format
 */
#define	VIS_OCTAL	0x0001	/* use octal \ddd format */
#define	VIS_CSTYLE	0x0002	/* use \[nrft0..] where appropiate */

/*
 * to alter set of characters encoded (default is to encode all
 * non-graphic except space, tab, and newline).
 */
#define	VIS_SP		0x0004	/* also encode space */
#define	VIS_TAB		0x0008	/* also encode tab */
#define	VIS_NL		0x0010	/* also encode newline */
#define	VIS_WHITE	(VIS_SP | VIS_TAB | VIS_NL)
#define	VIS_SAFE	0x0020	/* only encode "unsafe" characters */

/*
 * other
 */
#define	VIS_NOSLASH	0x0040	/* inhibit printing '\' */
#define	VIS_HTTP1808	0x0080	/* http-style escape % hex hex */
#define	VIS_HTTPSTYLE	0x0080	/* http-style escape % hex hex */
#define	VIS_GLOB	0x0100	/* encode glob(3) magic characters */
#define	VIS_MIMESTYLE	0x0200	/* mime-style escape = HEX HEX */
#define	VIS_HTTP1866	0x0400	/* http-style &#num; or &string; */
#define	VIS_NOESCAPE	0x0800	/* don't decode `\' */
#define	_VIS_END	0x1000	/* for unvis */
#define	VIS_SHELL	0x2000	/* encode shell special characters [not glob] */
#define	VIS_META	(VIS_WHITE | VIS_GLOB | VIS_SHELL)
#define	VIS_NOLOCALE	0x4000	/* encode using the C locale */

/*
 * unvis return codes
 */
#define	UNVIS_VALID	 1	/* character valid */
#define	UNVIS_VALIDPUSH	 2	/* character valid, push back passed char */
#define	UNVIS_NOCHAR	 3	/* valid sequence, no character produced */
#define	UNVIS_SYNBAD	-1	/* unrecognized escape sequence */
#define	UNVIS_ERROR	-2	/* decoder in unknown state (unrecoverable) */

/*
 * unvis flags
 */
#define	UNVIS_END	_VIS_END	/* no more characters */

#include <sys/cdefs.h>

__BEGIN_DECLS
char	*vis(char *, int, int, int);
char	*nvis(char *, size_t, int, int, int);

char	*svis(char *, int, int, int, const char *);
char	*snvis(char *, size_t, int, int, int, const char *);

int	strvis(char *, const char *, int);
int	stravis(char **, const char *, int);
int	strnvis(char *, size_t, const char *, int);

int	strsvis(char *, const char *, int, const char *);
int	strsnvis(char *, size_t, const char *, int, const char *);

int	strvisx(char *, const char *, size_t, int);
int	strnvisx(char *, size_t, const char *, size_t, int);
int 	strenvisx(char *, size_t, const char *, size_t, int, int *);

int	strsvisx(char *, const char *, size_t, int, const char *);
int	strsnvisx(char *, size_t, const char *, size_t, int, const char *);
int	strsenvisx(char *, size_t, const char *, size_t , int, const char *,
    int *);

int	strunvis(char *, const char *);
int	strnunvis(char *, size_t, const char *);

int	strunvisx(char *, const char *, int);
int	strnunvisx(char *, size_t, const char *, int);

#ifndef __LIBC12_SOURCE__
int	unvis(char *, int, int *, int);
#endif
__END_DECLS
