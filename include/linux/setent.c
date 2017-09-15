#include <pwd.h>
#include <grp.h>

int setgroupent(int stayopen)
{
  setgrent();
  return 1;
}

int setpassent(int stayopen)
{
  setpwent();
  return 1;
}
