#include <stdlib.h>

const char* heimdal_version = "";

void rk_print_version(const char *progname)
{
    const char *package_list = heimdal_version;

    if(progname == NULL)
        progname = getprogname();

    if(*package_list == '\0')
        package_list = "no version information";
    fprintf(stderr, "%s (%s)\n", progname, package_list);
    fprintf(stderr, "Copyright 1995-2011 Kungliga Tekniska Högskolan\n");
#ifdef PACKAGE_BUGREPORT
    fprintf(stderr, "Send bug-reports to %s\n", PACKAGE_BUGREPORT);
#endif
}
