/******************************************************************************
*                   HYPERSCRIPT Software Source Code.                         *
*                                                                             *
*          ***** Copyright: (c) 1994 Ab Initio Software                       *
*          ***** All rights reserved - Licensed Material.                     *
*          ***** Program property of Ab Initio Software                       *
*                                                                             *
******************************************************************************/
/*
 * Modifications:
 *
 *   $Log: signal.c,v $
 *   Revision 1.5  2007-07-09 05:39:00  bergsma
 *   TLOGV3
 *
 *   Revision 1.2  2002-11-14 01:40:45  bergsma
 *   Removed ^M occurances and added Modification section.
 *
 */

/******************* AUTOROUTER/HYPERSCRIPT INTERFACE ***********************/

#include "auto.h"	/* System Interface and Function Prototypes */

#ifndef AS_UNIX
#ifndef sigmask
#define sigmask(m)	(1 << ((m)-1))
#endif
#endif

/**********************	EXTERNAL GLOBAL VARIABLES ****************************/

/**********************	INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

Sigfunc * gHyp_signal_establish ( int signo, Sigfunc *func )
{
#ifdef AS_UNIX

  struct sigaction	act,oact ;
  
  act.sa_handler = (void (*)(int)) func ;
  sigemptyset ( &act.sa_mask ) ;
  act.sa_flags = 0 ;

  /* We don't ever want 'slow' system calls to be restarted */
#if defined ( AS_SOLARIS ) && defined ( SA_INTERRUPT )
  act.sa_flags |= SA_INTERRUPT ;	/* SunOs */
#endif

  if ( sigaction ( signo, &act, &oact ) == -1 )
    return (int (*)(int)) (SIG_ERR) ;
    
  return (int (*)(int)) (oact.sa_handler) ;
  
#else

  return (int (*)(int)) signal ( signo, (void (*)(int)) func ) ;
  
#endif
}

void gHyp_signal_block ( int signo )
{
#ifdef AS_UNIX
 sigset_t	newMask, oldMask ;
		
  sigemptyset ( &newMask ) ;
  sigaddset ( &newMask, signo ) ;
  if ( sigprocmask ( SIG_BLOCK, &newMask, &oldMask ) == -1 )
    gHyp_util_sysError ( "Failed to block signal %u", signo ) ;
    
#else
#ifndef AS_WINDOWS
  sigblock ( sigmask ( signo ) ) ;
#endif
#endif
  return ;
}

void gHyp_signal_unblock ( int signo )
{
#ifdef AS_UNIX

 sigset_t	newMask, oldMask ;
		
  sigemptyset ( &newMask ) ;
  sigaddset ( &newMask, signo ) ;
  if ( sigprocmask ( SIG_UNBLOCK, &newMask, &oldMask ) == -1 )
    gHyp_util_sysError ( "Failed to unblock signal %u", signo ) ;
  
#else
#ifndef AS_WINDOWS
  sigsetmask ( sigblock ( 0 ) & ~sigmask ( signo ) ) ; 
#endif
#endif
  return ;    
}
