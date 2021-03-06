#define LENGTH 32
/* skeinhl.c
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <phk@FreeBSD.org> wrote this file.  As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.   Poul-Henning Kamp
 * ----------------------------------------------------------------------------
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "skein.h"

char *
SKEIN256_End(SKEIN256_CTX *ctx, char *buf)
{
	int i;
	unsigned char digest[LENGTH];
	static const char hex[]="0123456789abcdef";

	if (!buf)
		buf = malloc(2*LENGTH + 1);
	if (!buf)
		return 0;
	SKEIN256_Final(digest, ctx);
	for (i = 0; i < LENGTH; i++) {
		buf[i+i] = hex[digest[i] >> 4];
		buf[i+i+1] = hex[digest[i] & 0x0f];
	}
	buf[i+i] = '\0';
	return buf;
}

char *
SKEIN256_Fd(int fd, char *buf)
{
	return SKEIN256_FdChunk(fd, buf, 0, 0);
}

char *
SKEIN256_FdChunk(int fd, char *buf, off_t ofs, off_t len)
{
	unsigned char buffer[16*1024];
	SKEIN256_CTX ctx;
	struct stat stbuf;
	int readrv, e;
	off_t remain;

	if (len < 0) {
		errno = EINVAL;
		return NULL;
	}

	SKEIN256_Init(&ctx);
	if (ofs != 0) {
		errno = 0;
		if (lseek(fd, ofs, SEEK_SET) != ofs ||
		    (ofs == -1 && errno != 0)) {
			readrv = -1;
			goto error;
		}
	}
	remain = len;
	readrv = 0;
	while (len == 0 || remain > 0) {
		if (len == 0 || remain > sizeof(buffer))
			readrv = read(fd, buffer, sizeof(buffer));
		else
			readrv = read(fd, buffer, remain);
		if (readrv <= 0) 
			break;
		SKEIN256_Update(&ctx, buffer, readrv);
		remain -= readrv;
	} 
error:
	if (readrv < 0)
		return NULL;
	return (SKEIN256_End(&ctx, buf));
}

char *
SKEIN256_File(const char *filename, char *buf)
{
	return (SKEIN256_FileChunk(filename, buf, 0, 0));
}

char *
SKEIN256_FileChunk(const char *filename, char *buf, off_t ofs, off_t len)
{
	char *ret;
	int e, fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return NULL;
	ret = SKEIN256_FdChunk(fd, buf, ofs, len);
	e = errno;
	close (fd);
	errno = e;
	return ret;
}

char *
SKEIN256_Data (const void *data, unsigned int len, char *buf)
{
	SKEIN256_CTX ctx;

	SKEIN256_Init(&ctx);
	SKEIN256_Update(&ctx,data,len);
	return (SKEIN256_End(&ctx, buf));
}

#ifdef WEAK_REFS
/* When building libmd, provide weak references. Note: this is not
   activated in the context of compiling these sources for internal
   use in libcrypt.
 */
#undef SKEIN256_End
__weak_reference(_libmd_SKEIN256_End, SKEIN256_End);
#undef SKEIN256_File
__weak_reference(_libmd_SKEIN256_File, SKEIN256_File);
#undef SKEIN256_FileChunk
__weak_reference(_libmd_SKEIN256_FileChunk, SKEIN256_FileChunk);
#undef SKEIN256_Data
__weak_reference(_libmd_SKEIN256_Data, SKEIN256_Data);
#endif
