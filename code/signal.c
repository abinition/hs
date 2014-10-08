/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 2002 Abinition (TM), Inc                      !
!          ***** Program property of Abinition, Inc                           !
!          ***** All rights reserved - Licensed Material.                     !
!
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** Program property of Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!                                                                             !
!*****************************************************************************/

/*
 *  This program is dual-licensed: either;
 *
 *  Under the terms of the GNU General Public License version 3 as 
 *  published by the Free Software Foundation. For the terms of this 
 *  license, see licenses/gplv3.md or <http://www.gnu.org/licenses/>;
 *
 *  Under the terms of the Commercial License as set out in 
 *  licenses/commercial.md
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License or Commerical License for more details.
 */

/*
 * Modifications:
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
