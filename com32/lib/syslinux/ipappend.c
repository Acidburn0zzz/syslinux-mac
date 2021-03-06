/* ----------------------------------------------------------------------- *
 *
 *   Copyright 2008 H. Peter Anvin - All Rights Reserved
 *
 *   Permission is hereby granted, free of charge, to any person
 *   obtaining a copy of this software and associated documentation
 *   files (the "Software"), to deal in the Software without
 *   restriction, including without limitation the rights to use,
 *   copy, modify, merge, publish, distribute, sublicense, and/or
 *   sell copies of the Software, and to permit persons to whom
 *   the Software is furnished to do so, subject to the following
 *   conditions:
 *
 *   The above copyright notice and this permission notice shall
 *   be included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *   OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------------------------------------------------------- */

/*
 * syslinux/ipappend.c
 *
 * Get ipappend strings
 */

#include <syslinux/config.h>
#include <klibc/compiler.h>
#include <com32.h>

struct syslinux_ipappend_strings __syslinux_ipappend_strings;
static const char *syslinux_ipappend_string_list[32];

void __constructor __syslinux_get_ipappend_strings(void)
{
    static com32sys_t reg;
    int i;

    reg.eax.w[0] = 0x000f;
    __intcall(0x22, &reg, &reg);

    if (!(reg.eflags.l & EFLAGS_CF)) {
	__syslinux_ipappend_strings.count = reg.ecx.w[0];
	__syslinux_ipappend_strings.ptr = syslinux_ipappend_string_list;
	for (i = 0; i < reg.ecx.w[0]; i++) {
	    syslinux_ipappend_string_list[i] =
		MK_PTR(reg.es,
		       *(uint16_t *) MK_PTR(reg.es, reg.ebx.w[0] + i * 2));
	}
    }
}
