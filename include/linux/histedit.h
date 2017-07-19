#pragma once

#include_next <histedit.h>

// the linux libedit packages don't provide this, lets just use the normal one
// we won't be using that sh interactively anyway so it should be fine
#define _el_fn_sh_complete _el_fn_complete
