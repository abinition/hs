/*
 * Modifications:
 *
 * $Log: mem.h,v $
 * Revision 1.3  2005-03-16 23:54:37  bergsma
 * V 3.5.1 - fixes for use with DECC compiler.
 *
 * Revision 1.2  2002/11/14 01:40:45  bergsma
 * Removed ^M occurances and added Modification section.
 *
 *
 */

/* Let memory shrink (be free'd) when free and realloc are used */
unsigned long __noshrink = 0 ;

/* Shrink chunk size is 16K */
size_t __minshrink = 16384;

/* Don't shrink unless heap size exceeds 5MB (16387/.003) */
double __minshrinkfactor = 0.003;

/* Grow HEAP to at least 48K */
size_t __mingrow = 49152;

/* or (15MB * .1) == 1.5 MB */ 
double __mingrowfactor = 0.1;

/* No need to swap memory that is physical, swap only virtual */
unsigned long __madvisor = 1;

/* HyperScript uses a lot of small 1-15 byte buffers */
unsigned long __small_buff = 1;

/* Use defaults. See malloc man pages */
int __fast_free_max = 13;
unsigned long __sbrk_override = 0;
unsigned long __taso_mode = 0;
