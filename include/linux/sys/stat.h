#pragma once
#include_next <sys/stat.h>

/*
 * Definitions of flags stored in file flags word.
 *
 * Super-user and owner changeable flags.
 */
#define	UF_SETTABLE	0x0000ffff	/* mask of owner changeable flags */
#define	UF_NODUMP	0x00000001	/* do not dump file */
#define	UF_IMMUTABLE	0x00000002	/* file may not be changed */
#define	UF_APPEND	0x00000004	/* writes to file may only append */
#define	UF_OPAQUE	0x00000008	/* directory is opaque wrt. union */
#define	UF_NOUNLINK	0x00000010	/* file may not be removed or renamed */
/*
 * These two bits are defined in MacOS X.  They are not currently used in
 * FreeBSD.
 */
#if 0
#define	UF_COMPRESSED	0x00000020	/* file is compressed */
#define	UF_TRACKED	0x00000040	/* renames and deletes are tracked */
#endif

#define	UF_SYSTEM	0x00000080	/* Windows system file bit */
#define	UF_SPARSE	0x00000100	/* sparse file */
#define	UF_OFFLINE	0x00000200	/* file is offline */
#define	UF_REPARSE	0x00000400	/* Windows reparse point file bit */
#define	UF_ARCHIVE	0x00000800	/* file needs to be archived */
#define	UF_READONLY	0x00001000	/* Windows readonly file bit */
/* This is the same as the MacOS X definition of UF_HIDDEN. */
#define	UF_HIDDEN	0x00008000	/* file is hidden */

/*
 * Super-user changeable flags.
 */
#define	SF_SETTABLE	0xffff0000	/* mask of superuser changeable flags */
#define	SF_ARCHIVED	0x00010000	/* file is archived */
#define	SF_IMMUTABLE	0x00020000	/* file may not be changed */
#define	SF_APPEND	0x00040000	/* writes to file may only append */
#define	SF_NOUNLINK	0x00100000	/* file may not be removed or renamed */
#define	SF_SNAPSHOT	0x00200000	/* snapshot inode */
