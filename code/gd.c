/*****************************************************************************!
!                HyperScript Software Source Code.                            !
!                                                                             !
!          ***** Copyright: (c) 1994 Ab Initio Software                       !
!          ***** All rights reserved - Licensed Material.                     !
!          ***** Program property of Ab Initio Software                       !
!                                                                             !
!*****************************************************************************/

/*
 * Modifications:
 *
 *
 * Modified:
 *
 * $Log: gd.c,v $
 * Revision 1.17  2010-06-26 06:33:48  bergsma
 * Init
 *
 * Revision 1.16  2010-05-27 03:57:38  bergsma
 * Various fixes
 *
 * Revision 1.15  2010-05-05 04:56:35  bergsma
 * Added CGI form-data support
 *
 * Revision 1.14  2010-03-05 06:12:58  bergsma
 * Variables defined outside of AS_GD section
 *
 * Revision 1.13  2009-08-11 21:18:20  bergsma
 * Expanded functionality of gd_open
 *
 * Revision 1.12  2009-04-09 19:57:21  bergsma
 * Fixing issues with HTTP and buffer sizing
 *
 * Revision 1.11  2009-03-24 09:31:37  bergsma
 * Perforrmance improvement (5x) for function lookup.
 *
 * Revision 1.10  2009-03-23 19:58:13  bergsma
 * Perforrmance improvement (5x) for function lookup.
 *
 * Revision 1.9  2009-03-22 19:21:02  bergsma
 * no message
 *
 * Revision 1.8  2009-03-13 07:48:16  bergsma
 * GD refinements.
 * Added BUILD_VERSION
 *
 * Revision 1.7  2009-03-08 16:17:45  bergsma
 * First successfull hs/gd compile&link
 *
 * Revision 1.6  2009-03-08 01:19:32  bergsma
 * Finished CODING
 *
 * Revision 1.5  2009-03-07 21:27:32  bergsma
 * gHyp_data_getAll needs additional handle argument
 *
 * Revision 1.4  2009-03-06 18:03:51  bergsma
 * CVS update
 *
 * Revision 1.3  2009-03-06 04:13:14  bergsma
 * Adding GD functionality
 *
 * Revision 1.2  2009-03-02 15:55:58  bergsma
 * no message
 *
 * Revision 1.1  2009-03-01 22:24:51  bergsma
 * no message
 *
 */

/********************** AUTOROUTER INTERFACE ********************************/

#include "auto.h"       /* System Interface and Function Prototypes */

/********************** EXTERNAL FUNCTION DECLARATIONS ***********************/

/********************** EXTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL GLOBAL VARIABLES ****************************/

/********************** INTERNAL OBJECT STRUCTURES ***************************/

/********************** FUNCTION DEFINITIONS ********************************/




#ifdef AS_GD
#ifdef WIN32
#include <gd.h>
#include <gdfontt.h>
#include <gdfonts.h>
#include <gdfontmb.h>
#include <gdfontl.h>
#include <gdfontg.h>
#else
#include <gd/gd.h>
#include <gd/gdfontt.h>
#include <gd/gdfonts.h>
#include <gd/gdfontmb.h>
#include <gd/gdfontl.h>
#include <gd/gdfontg.h>
#endif
#endif

void gHyp_gd_query ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: gd_query ()
   *    Returns TRUE or FALSE
   *
   	  
	     gdFree
	     gdFTUseFontConfig
	     gdFontCacheSetup
	     gdFontCacheShutdown
	     gdFontGetLarge
	     gdFontGetMediumBold
	     gdFontGetSmall
	     gdFontGetTiny
	     gdFontGetW
	     gdFontGetH
	     gdFontGiant
	     gdFontCacheSetup
	     gdFontCacheShutdown
	   
	      gdImageAlpha
	      gdImageAlphaBlending
	      gdImageArc
	      gdImageBlue
	      gdImageBoundsSafe
	      gdImageChar
	      gdImageCharUp

	    gdImageColorAllocate
	    gdImageColorAllocateAlpha
	    gdImageColorClosest
	    gdImageColorClosestAlpha
	    gdImageColorClosestHWB
	    gdImageColorDeallocate
	    gdImageColorExact
	    gdImageColorResolve
	    gdImageColorResolveAlpha
	    gdImageColorTransparent
	    gdImageColorsTotal

	    gdImageCompare
	    gdImageCopy
	    gdImageCopyMerge
	    gdImageCopyMergeGray
	    gdImageCopyResampled
	    gdImageCopyResized
	    gdImageCopyRotated
	    gdImageDashedLine  


	    gdImageFill
	    gdImageFillToBorder
	    gdImageFilledArc
	    gdImageFilledEllipse
	    gdImageFilledPolygon
	    gdImageFilledRectangle
	    gdImageGetClip
	    gdImageGetInterlaced
	    gdImageGetPixel
	    gdImageGetTransparent
	    gdImageGifPtr

	    gdImageGreen
	    gdImageInterlace
	    gdImageJpeg
	    gdImageJpegPtr
	    gdImageLine
	    gdImageOpenPolygon
	    gdImagePaletteCopy
	    gdImagePng
	    gdImagePngPtr
	    gdImagePolygon
	    gdImageRectangle
	    gdImageRed
	    gdImageSX
	    gdImageSY
	    gdImageSaveAlpha

	  gdImageSetAntiAliased
	  gdImageSetAntiAliasedDontBlend
	  gdImageSetBrush
	  gdImageSetClip
	  gdImageSetPixel
	  gdImageSetStyle
	  gdImageSetThickness
	  gdImageSetTile
	  gdImageSharpen
	  gdImageSquareToCircle

	    gdImageString
	    gdImageString16
	    gdImageStringFT
	    gdImageStringFTCircle
	    gdImageStringFTEx
	    gdImageStringTTF
	    gdImageStringUp
	    gdImageStringUp16
	    gdImageTrueColor
	    gdTrueColor
	    gdTrueColorAlpha
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame
    *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse
    *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pResult = NULL;

    void
      *result_handle=NULL ;
    int
      result_int=0 ;

    char
      result_str[VALUE_SIZE+1] ;

    sLOGICAL
      result_boolean=TRUE,
      boolean_result=FALSE,
      int_result=FALSE,
      string_result=FALSE,
      handle_result=FALSE ;


#ifdef AS_GD

    sLOGICAL
      list_result=FALSE ;

    sData
      *pData,
      *pData1=NULL,
      *pData2=NULL,
      *pData3=NULL,
      *pData4=NULL,
      *pData5=NULL,
      *pData6=NULL,
      *pData7=NULL,
      *pData8=NULL,
      *pData9=NULL,
      *pData10=NULL,
      *pData11=NULL,
      *pValue ;

    int
      functionStrLen,
      argCount = gHyp_parse_argCount ( pParse ),
      i,n,
      arg1_int,
      arg2_int,
      arg3_int,
      arg4_int,
      arg5_int,
      arg6_int,
      arg7_int,
      arg8_int,
      arg9_int,
      arg10_int,
      arg11_int,
      arg1_ss,
      arg2_ss,
      arg3_ss,
      arg4_ss,
      arg5_ss,
      arg6_ss,
      arg7_ss,
      arg8_ss,
      arg9_ss,
      arg10_ss,
      arg11_ss,
      arg1_len,
      arg2_len,
      arg3_len,
      arg4_len,
      arg5_len,
      arg6_len,
      arg7_len,
      arg8_len,
      arg9_len,
      arg10_len,
      arg11_len ;

    unsigned long
      arg1_ulong,
      arg2_ulong,
      arg3_ulong,
      arg4_ulong,
      arg5_ulong,
      arg6_ulong,
      arg7_ulong,
      arg8_ulong,
      arg9_ulong,
      arg10_ulong,
      arg11_ulong ;

    double
      arg1_double,
      arg2_double,
      arg3_double,
      arg4_double,
      arg5_double,
      arg6_double,
      arg7_double,
      arg8_double,
      arg9_double,
      arg10_double,
      arg11_double ;

    char
      functionStr[VALUE_SIZE+1],
      *err,
      arg1_string[VALUE_SIZE+1],
      arg2_string[VALUE_SIZE+1],
      arg3_string[VALUE_SIZE+1],
      arg4_string[VALUE_SIZE+1],
      arg5_string[VALUE_SIZE+1],
      arg6_string[VALUE_SIZE+1],
      arg7_string[VALUE_SIZE+1],
      arg8_string[VALUE_SIZE+1],
      arg9_string[VALUE_SIZE+1],
      arg10_string[VALUE_SIZE+1],
      arg11_string[VALUE_SIZE+1];

    sLOGICAL
      arg1_bool,
      arg2_bool,
      arg3_bool,
      arg4_bool,
      arg5_bool,
      arg6_bool,
      arg7_bool,
      arg8_bool,
      arg9_bool,
      arg10_bool,
      arg11_bool ;

    sBYTE
      arg1_tokenType,
      arg2_tokenType,
      arg3_tokenType,
      arg4_tokenType,
      arg5_tokenType,
      arg6_tokenType,
      arg7_tokenType,
      arg8_tokenType,
      arg9_tokenType,
      arg10_tokenType,
      arg11_tokenType,
      arg1_dataType,
      arg2_dataType,
      arg3_dataType,
      arg4_dataType,
      arg5_dataType,
      arg6_dataType,
      arg7_dataType,
      arg8_dataType,
      arg9_dataType,
      arg10_dataType,
      arg11_dataType ;

    void
      *imagePtr,
      *arg1_handle,
      *arg2_handle,
      *arg3_handle,
      *arg4_handle,
      *arg5_handle,
      *arg6_handle,
      *arg7_handle,
      *arg8_handle,
      *arg9_handle,
      *arg10_handle,
      *arg11_handle ;

    gdImagePtr
      im = NULL;

#define MAX_POINTS 100
    gdPoint 
      points[MAX_POINTS];

    if ( argCount < 2 || argCount > 13  ) 
      gHyp_instance_error ( pAI, STATUS_ARGUMENT, 
	"Invalid arguments. Usage: gd_query ( functionStr, gdImagePtr [, arg1 [,arg2 [,arg3, ... [,arg11]]]] )" ) ;


    pResult = gHyp_data_new ( NULL ) ;

    while ( argCount-- ) {

      if ( argCount == 0 ) {

        /* Get function argument */
        pData = gHyp_stack_popRvalue ( pStack, pAI ) ; 
        functionStrLen = gHyp_data_getStr ( pData, 
				            functionStr, 
					    VALUE_SIZE, 
					    gHyp_data_getSubScript(pData),
					    TRUE ) ;
      }

      else if ( argCount == 1 ) {

        /* Get gdImagePtr */
        pData = gHyp_stack_popRvalue ( pStack, pAI ) ;

        im = (gdImagePtr) gHyp_data_getHandle ( pData,
					gHyp_data_getSubScript(pData),
					TRUE ) ;
      }

      else if ( argCount == 2 ) {

        pData1 = gHyp_stack_popRvalue ( pStack, pAI ) ;
        arg1_ss = gHyp_data_getSubScript(pData1) ;
	gHyp_data_getAll ( pData1, 
			   &arg1_tokenType, 
			   &arg1_dataType, 
			   &arg1_int,
			   &arg1_ulong,
			   &arg1_double, 
			   &arg1_bool, 
			   &arg1_handle, 
			   arg1_string,
			   &arg1_len,
			   VALUE_SIZE,
			   arg1_ss,
			   TRUE ) ;
      }
      else if ( argCount == 3 ) {

        pData2 = gHyp_stack_popRvalue ( pStack, pAI ) ;
        arg2_ss = gHyp_data_getSubScript(pData2) ;
	gHyp_data_getAll ( pData2, 
			   &arg2_tokenType, 
			   &arg2_dataType, 
			   &arg2_int,
			   &arg2_ulong,
			   &arg2_double, 
			   &arg2_bool, 
			   &arg2_handle, 
			   arg2_string,
			   &arg2_len,
			   VALUE_SIZE,
			   arg2_ss,
			   TRUE ) ;
      }
      else if ( argCount == 4 ) {

        pData3 = gHyp_stack_popRvalue ( pStack, pAI ) ;
        arg3_ss = gHyp_data_getSubScript(pData3) ;
	gHyp_data_getAll ( pData3, 
			   &arg3_tokenType, 
			   &arg3_dataType, 
			   &arg3_int,
			   &arg3_ulong,
			   &arg3_double, 
			   &arg3_bool, 
			   &arg3_handle, 
			   arg3_string,
			   &arg3_len,
			   VALUE_SIZE,
			   arg3_ss,
			   TRUE ) ;
      }      
      else if ( argCount == 5 ) {

        pData4 = gHyp_stack_popRvalue ( pStack, pAI ) ;
        arg4_ss = gHyp_data_getSubScript(pData4) ;
	gHyp_data_getAll ( pData4, 
			   &arg4_tokenType, 
			   &arg4_dataType, 
			   &arg4_int,
			   &arg4_ulong,
			   &arg4_double, 
			   &arg4_bool, 
			   &arg4_handle, 
			   arg4_string,
			   &arg4_len,
			   VALUE_SIZE,
			   arg4_ss,
			   TRUE ) ;
      }      
      else if ( argCount == 6 ) {

        pData5 = gHyp_stack_popRvalue ( pStack, pAI ) ;
        arg5_ss = gHyp_data_getSubScript(pData5) ;
	gHyp_data_getAll ( pData5, 
			   &arg5_tokenType, 
			   &arg5_dataType, 
			   &arg5_int,
			   &arg5_ulong,
			   &arg5_double, 
			   &arg5_bool,
			   &arg5_handle, 
			   arg5_string,
			   &arg5_len,
			   VALUE_SIZE,
			   arg5_ss,
			   TRUE ) ;
      }
      else if ( argCount == 7 ) {

        pData6 = gHyp_stack_popRvalue ( pStack, pAI ) ;
        arg6_ss = gHyp_data_getSubScript(pData6) ;
	gHyp_data_getAll ( pData6, 
			   &arg6_tokenType, 
			   &arg6_dataType, 
			   &arg6_int,
			   &arg6_ulong,
			   &arg6_double, 
			   &arg6_bool, 
			   &arg6_handle, 
			   arg6_string,
			   &arg6_len,
			   VALUE_SIZE,
			   arg6_ss,
			   TRUE ) ;
      }
      else if ( argCount == 8 ) {

        pData7 = gHyp_stack_popRvalue ( pStack, pAI ) ;
        arg7_ss = gHyp_data_getSubScript(pData7) ;
	gHyp_data_getAll ( pData7, 
			   &arg7_tokenType, 
			   &arg7_dataType, 
			   &arg7_int,
			   &arg7_ulong,
			   &arg7_double, 
			   &arg7_bool, 
			   &arg7_handle, 
			   arg7_string,
			   &arg7_len,
			   VALUE_SIZE,
			   arg7_ss,
			   TRUE ) ;
      }
      else if ( argCount == 9 ) {

        pData8 = gHyp_stack_popRvalue ( pStack, pAI ) ;
        arg8_ss = gHyp_data_getSubScript(pData8) ;
	gHyp_data_getAll ( pData8, 
			   &arg8_tokenType, 
			   &arg8_dataType, 
			   &arg8_int,
			   &arg8_ulong,
			   &arg8_double, 
			   &arg8_bool, 
			   &arg8_handle, 
			   arg8_string,
			   &arg8_len,
			   VALUE_SIZE,
			   arg8_ss,
			   TRUE ) ;
      }
      else if ( argCount == 10 ) {
        pData9 = gHyp_stack_popRvalue ( pStack, pAI ) ;
        arg9_ss = gHyp_data_getSubScript(pData9) ;
	gHyp_data_getAll ( pData9, 
			   &arg9_tokenType, 
			   &arg9_dataType, 
			   &arg9_int,
			   &arg9_ulong,
			   &arg9_double, 
			   &arg9_bool, 
			   &arg9_handle, 
			   arg9_string,
			   &arg9_len,
			   VALUE_SIZE,
			   arg9_ss,
			   TRUE ) ;
      }
      else if ( argCount == 11 ) {
        pData10 = gHyp_stack_popRvalue ( pStack, pAI ) ;
        arg10_ss = gHyp_data_getSubScript(pData10) ;
	gHyp_data_getAll ( pData10, 
			   &arg10_tokenType, 
			   &arg10_dataType, 
			   &arg10_int,
			   &arg10_ulong,
			   &arg10_double, 
			   &arg10_bool, 
			   &arg10_handle, 
			   arg10_string,
			   &arg10_len,
			   VALUE_SIZE,
			   arg10_ss,
			   TRUE ) ;
      }
      else if ( argCount == 12 ) { 
        pData11 = gHyp_stack_popRvalue ( pStack, pAI ) ;
        arg11_ss = gHyp_data_getSubScript(pData11) ;
	gHyp_data_getAll ( pData11, 
			   &arg11_tokenType, 
			   &arg11_dataType, 
			   &arg11_int,
			   &arg11_ulong,
			   &arg11_double, 
			   &arg11_bool, 
			   &arg11_handle, 
			   arg11_string,
			   &arg11_len,
			   VALUE_SIZE,
			   arg11_ss,
			   TRUE ) ;
      }
    }

    /* Old style speed improvements.  Please put all GD functions
     * in alphabetical order in their proper groups or you will 
     * muck things up.  We start with the middle of the alphabetically
     * listed functions, which is at "gdGetImageEtch", there is
     * no such function btw.
     *
     * We do this 3 levels deep, then simply go through each
     * group of about 10-15 choices. 
     *
     * There are 83 functions currently.  (((83/2)/2)/2) = 10.
     * 
     * Therefore, the maximum number of strcmp needed to reach 
     * a function is 3 + 10/2 or 8.
     *
     * Without this additonal feature, it would take on average
     * 83 /2 = 41 choices.
     * 
     * That's a speed improvement of 5 times.
     *
     * WORTH IT, YOUNG FELLA.
     *
     */

    /*gHyp_util_debug("Executing gd function %s",functionStr ) ;*/

      /* Use of points.
       *
       * The HS structure is defined like the following:

      list points = {
	list p1 = {
  	  int x = 50,
	  int y = 0
	},
	list p2 = {
	  int x = 99,
	  int y = 99
	},
	list p2 = {
	  int x = 0,
	  int y = 99
	}
      } ;
      *
      */

    if ( strcmp ( functionStr, "gdImageEtch" ) < 0 ) {

      if ( strcmp ( functionStr, "gdImageColor" ) < 0 ) {
        
	if ( strcmp ( functionStr, "gdImageAbinition" ) < 0 ) {
      
	  /* 
	     gdFree
	     gdFTUseFontConfig
	     gdFontCacheSetup
	     gdFontCacheShutdown
	     gdFontGetLarge
	     gdFontGetMediumBold
	     gdFontGetSmall
	     gdFontGetTiny
	     gdFontGetW
	     gdFontGetH
	     gdFontGiant
	     gdFontCacheSetup
	     gdFontCacheShutdown
	   */

	  if ( strcmp ( functionStr, "gdFree" ) == 0 ) {
	
	    gdFree ( arg1_handle ) ;
	    boolean_result = TRUE ;

	  }
	  else if ( strcmp ( functionStr, "gdFTUseFontConfig" ) == 0 ) {

	    result_int = gdFTUseFontConfig(
	      arg1_int /* FLAG */
	    ); 
	    int_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdFontGetSmall" ) == 0 ) {
      
	    result_handle = gdFontGetSmall();
	    handle_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdFontGetLarge" ) == 0 ) {
     
	    result_handle = gdFontGetLarge();
	    handle_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdFontGetMediumBold" ) == 0 ) {
     
	    result_handle = gdFontGetMediumBold();
	    handle_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdFontGetGiant" ) == 0 ) {
     
	    result_handle = gdFontGetGiant();
	    handle_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdFontGetTiny" ) == 0 ) {
     
	    result_handle = gdFontGetTiny();
	    handle_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdFontGetW" ) == 0 ) {
     
	    result_int = ((gdFontPtr) arg1_handle)->w ;
	    int_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdFontGetH" ) == 0 ) {
     
	    result_int = ((gdFontPtr) arg1_handle)->h ;
	    int_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdFontGetTiny" ) == 0 ) {
     
	    result_handle = gdFontGetTiny();
	    handle_result = TRUE ;
	  }
	  /*
	  else if ( strcmp ( functionStr, "gdFontCacheSetup" ) == 0 ) {

	    result_int = gdFontCacheSetup() ;
	    int_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdFontCacheShutdown" ) == 0 ) {

	    gdFontCacheShutdown() ;
	    int_result = TRUE ;
	  }
	  ***/

	}
	else {

	  /*
	      gdImageAlpha
	      gdImageAlphaBlending
	      gdImageArc
	      gdImageBlue
	      gdImageBoundsSafe
	      gdImageChar
	      gdImageCharUp
	   */
	  if ( strcmp ( functionStr, "gdImageAlpha" ) == 0 ) {

	    result_int = gdImageAlpha( 
	      im,  /* GD Image Ptr */ 
	      arg1_int  /* COLOR */
	      ) ;
	    int_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageAlphaBlending" ) == 0 ) {
      
	    gdImageAlphaBlending(
	      im,  /* GD Image Ptr */ 
	      arg1_int  /* BLENDING */
	      );
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageArc" ) == 0 ) {

	    gdImageArc(
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* CX */
	      arg2_int, /* CY */
	      arg3_int, /* W */
	      arg4_int, /* H */
	      arg5_int, /* S */
	      arg6_int, /* E */
	      arg7_int  /* COLOR */
	    ) ;
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageBlue" ) == 0 ) {
     
	    result_int = gdImageBlue ( 
	      im,  /* GD Image Ptr */ 
	      arg1_int /* C */
	      ) ;
	    int_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageBoundsSafe" ) == 0 ) {
      
	    result_int = gdImageBoundsSafe ( 
	      im,  /* GD Image Ptr */ 
	      arg1_int,   /* X */
	      arg2_int    /* Y */
	    ) ;
	    int_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageChar" ) == 0 ) {
     
	    gdImageChar(
	      im,  /* GD Image Ptr */ 
	      (gdFontPtr) arg1_handle,  /* FONT */
	      arg2_int, /* X */
	      arg3_int, /* Y */
	      arg4_int, /* C */
	      arg5_int  /* COLOR */
	      );
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageCharUp" ) == 0 ) {
      
	    gdImageCharUp(
	      im,  /* GD Image Ptr */ 
	      (gdFontPtr) arg1_handle,  /* FONT */ 
	      arg2_int, /* X */
	      arg3_int, /* Y */
	      arg4_int, /* C */
	      arg5_int  /* COLOR */
	    );
	    boolean_result = TRUE ;
	  }
	}
      }
      else {

	if ( strcmp (  functionStr, "gdImageCom" ) < 0 ) {

	  /*
	    gdImageColorAllocate
	    gdImageColorAllocateAlpha
	    gdImageColorClosest
	    gdImageColorClosestAlpha
	    gdImageColorClosestHWB
	    gdImageColorDeallocate
	    gdImageColorExact
	    gdImageColorResolve
	    gdImageColorResolveAlpha
	    gdImageColorTransparent
	    gdImageColorsTotal
	   */

	  if ( strcmp ( functionStr, "gdImageColorAllocate" ) == 0 ) {
	      result_int = gdImageColorAllocate ( 
		im,  /* GD Image Ptr */ 
		arg1_int, /* R */
		arg2_int, /* G */
		arg3_int /* B */
		) ; 
	      int_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageColorTransparent" ) == 0 ) {

	    gdImageColorTransparent ( 
	      im,  /* GD Image Ptr */
	      arg1_int /* COLOR */
	    ) ;
	    boolean_result = TRUE ;

	  } 
	  else if ( strcmp ( functionStr, "gdImageColorDeallocate" ) == 0 ) {

	    gdImageColorDeallocate ( 
	      im,  /* GD Image Ptr */
	      arg1_int /* COLOR */
	    ) ;
	    boolean_result = TRUE ;

	  } 
	  else if ( strcmp ( functionStr, "gdImageColorAllocateAlpha" ) == 0 ) {
	      result_int = gdImageColorAllocateAlpha ( 
		im,  /* GD Image Ptr */ 
		arg1_int, /* R */
		arg2_int, /* G */
		arg3_int, /* B */
		arg4_int  /* ALPHA */
		) ; 
	      int_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageColorClosest" ) == 0 ) {

	    result_int = gdImageColorClosest ( 
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* R */
	      arg2_int, /* G */
	      arg3_int  /* B */
	      ) ;
	    int_result = TRUE ;

	  }
	  else if ( strcmp ( functionStr, "gdImageColorClosestAlpha" ) == 0 ) {

	    result_int = gdImageColorClosestAlpha ( 
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* R */
	      arg2_int, /* G */
	      arg3_int, /* B */
	      arg4_int  /* ALPHA */
	      ) ;
	    int_result = TRUE ;

	  }    
	  else if ( strcmp ( functionStr, "gdImageColorClosestHWB" ) == 0 ) {

	    result_int = gdImageColorClosestHWB ( 
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* R */
	      arg2_int, /* G */
	      arg3_int  /* B */
	      ) ;
	    int_result = TRUE ;

	  } 
	  else if ( strcmp ( functionStr, "gdImageColorExact" ) == 0 ) {

	    result_int = gdImageColorExact ( 
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* R */
	      arg2_int, /* G */
	      arg3_int  /* B */
	      ) ;
	    int_result = TRUE ;

	  } 
	  else if ( strcmp ( functionStr, "gdImageColorResolve" ) == 0 ) {

	    result_int = gdImageColorResolve ( 
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* R */
	      arg2_int, /* G */
	      arg3_int  /* B */
	      ) ;
	    int_result = TRUE ;

	  } 
	  else if ( strcmp ( functionStr, "gdImageColorResolveAlpha" ) == 0 ) {

	    result_int = gdImageColorResolveAlpha ( 
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* R */
	      arg2_int, /* G */
	      arg3_int, /* B */
	      arg4_int  /* ALPHA */
		      ) ;
	    int_result = TRUE ;

	  }
	  else if ( strcmp ( functionStr, "gdImageColorsTotal" ) == 0 ) {

	    result_int = gdImageColorsTotal ( 
	      im  /* GD Image Ptr */ 
	    ) ;
	    int_result = TRUE ;

	  } 
	}
	else {
          
	  /* 
	    gdImageCompare
	    gdImageCopy
	    gdImageCopyMerge
	    gdImageCopyMergeGray
	    gdImageCopyResampled
	    gdImageCopyResized
	    gdImageCopyRotated
	    gdImageDashedLine     
	   */
	  
	  if ( strcmp ( functionStr, "gdImageCompare" ) == 0 ) {

	    result_int = gdImageCompare ( 
	      im,  /* GD Image Ptr */
	      (gdImagePtr) arg1_handle /*SRC */
	      ) ;

	    int_result = TRUE ;
	  }
          
          else if ( strcmp ( functionStr, "gdImageCopy" ) == 0 ) {

	    gdImageCopy( 
	      im,  /* GD Image Ptr */ 
	      (gdImagePtr) arg1_handle, /* SRC */ 
	      arg2_int, /* dstX */
	      arg3_int, /* dstY */
	      arg4_int, /* srcX */
	      arg5_int, /* srcY */
	      arg6_int, /* W */
	      arg7_int /* H */
	      ) ;
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageCopyResized" ) == 0 ) {
	
	    gdImageCopyResized( 
	      im,  /* GD Image Ptr */ 
	      (gdImagePtr) arg1_handle, /* SRC */
	      arg2_int, /* dstX */
	      arg3_int, /* dstY */
	      arg4_int, /* srcX */
	      arg5_int, /* srcY */
	      arg6_int, /* destW */
	      arg7_int, /* destH */
	      arg8_int, /* srcW */
	      arg9_int  /* srcH */
	    ) ;
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageCopyResampled" ) == 0 ) {
	
	    gdImageCopyResampled( 
	      im,  /* GD Image Ptr */ 
	      (gdImagePtr) arg1_handle, /* SRC */
	      arg2_int, /* dstX */
	      arg3_int, /* dstY */
	      arg4_int, /* srcX */
	      arg5_int, /* srcY */
	      arg6_int, /* destW */
	      arg7_int, /* destH */
	      arg8_int, /* srcW */
	      arg9_int  /* srcH */
	    ) ;
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageCopyRotated" ) == 0 ) {

	    gdImageCopyRotated( 
	      im,  /* GD Image Ptr */ 
	      (gdImagePtr) arg1_handle, /* SRC */	    
	      arg2_double, /* dstX */
	      arg3_double, /* dstY */
	      arg4_int, /* srcX */
	      arg5_int, /* srcY */ 	    
	      arg6_int, /* srcW */	    
	      arg7_int, /* srcH */
	      arg8_int /* ANGLE */		    
	      ) ;
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageCopyMerge" ) == 0 ) {

	    gdImageCopyMerge(   
	      im,  /* GD Image Ptr */ 
	      (gdImagePtr) arg1_handle, /* SRC */
	      arg2_int, /* dstX */
	      arg3_int, /* dstY */
	      arg4_int, /* srcX */
	      arg5_int, /* srcY */
	      arg6_int, /* W */
	      arg7_int, /* H */
	      arg8_int  /* PCT */
	      ) ;
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageCopyMergeGray" ) == 0 ) {
	
	    gdImageCopyMergeGray(   
  	      im,  /* GD Image Ptr */ 
	      (gdImagePtr) arg1_handle, /* SRC */
	      arg2_int, /* dstX */
	      arg3_int, /* dstY */
	      arg4_int, /* srcX */
	      arg5_int, /* srcY */
	      arg6_int, /* W */
	      arg7_int, /* H */
	      arg8_int /* PCT */
	      ) ;
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageDashedLine" ) == 0 ) {
      
	    gdImageDashedLine( 
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* X1 */
	      arg2_int, /* Y1 */
	      arg3_int, /* X2 */
	      arg4_int, /* Y2 */
	      arg5_int  /* COLOR */
	    ) ;
	    boolean_result = TRUE ;

	  }
	}	
      }
    }
    else {

      if ( strcmp (  functionStr, "gdImageSet") < 0 ) {
	 
	if ( strcmp ( functionStr, "gdImageGray" ) < 0 ) { 
	
	  /* 
	    gdImageFill
	    gdImageFillToBorder
	    gdImageFilledArc
	    gdImageFilledEllipse
	    gdImageFilledPolygon
	    gdImageFilledRectangle
	    gdImageGetClip
	    gdImageGetInterlaced
	    gdImageGetPixel
	    gdImageGetTransparent
	    gdImageGifPtr
	   */

	  if ( strcmp ( functionStr, "gdImageFill" ) == 0 ) {
      
	    gdImageFill( 
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* X */
	      arg2_int, /* Y */
	      arg3_int  /* COLOR */
	    ) ;
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageFillToBorder" ) == 0 ) {
        
	    gdImageFillToBorder( 
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* X */
	      arg2_int, /* Y */
	      arg3_int, /* BORDER */
	      arg4_int  /* COLOR */ 
	      ) ;
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageFilledArc" ) == 0 ) {

	    gdImageFilledArc(
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* CX */
	      arg2_int, /* CY */
	      arg3_int, /* W */
	      arg4_int, /* H */
	      arg5_int, /* S */
	      arg6_int, /* E */
	      arg7_int, /* COLOR */
	      arg8_int  /* STYLE */
	    );
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageFilledEllipse" ) == 0 ) {

	    gdImageFilledEllipse(
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* CX */
	      arg2_int, /* CY */
	      arg3_int, /* W */
	      arg4_int, /* H */
	      arg5_int  /* COLOR */
	      );
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageFilledPolygon" ) == 0 ) {

	    /* Get # of points */
	    n = MIN ( arg2_int, MAX_POINTS ) ;
	    for ( i=0; i<n; i++ ) {

	      pValue = gHyp_data_getChildBySS ( pData1, i ) ;
	      pValue = gHyp_data_getChildByName ( pValue, "x" ) ;
	      points[i].x = gHyp_data_getInt ( pValue, 0, TRUE ) ;

	      pValue = gHyp_data_getChildBySS ( pData1, i ) ;
	      pValue = gHyp_data_getChildByName ( pValue, "y" ) ;
	      points[i].y = gHyp_data_getInt ( pValue, 0, TRUE ) ;
	    }

	    gdImageFilledPolygon(
	      im,  /* GD Image Ptr */ 
	      points, /* POINTS */ 
	      arg2_int, /* POINTS_TOTAL */
	      arg3_int  /* COLOR */
	    );
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageFilledRectangle" ) == 0 ) {
      
	    gdImageFilledRectangle(
	      im,  /* GD Image Ptr */ 
	      arg1_int,   /* X1 */
	      arg2_int,   /* Y1 */
	      arg3_int,   /* X2 */
	      arg4_int,   /* Y2 */
	      arg5_int    /* COLOR */
	    ); 
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageGetInterlaced" ) == 0 ) {

	    result_int = gdImageGetInterlaced ( 
	      im  /* GD Image Ptr */ 
	    ) ;
	    int_result = TRUE ;

	  } 
	  else if ( strcmp ( functionStr, "gdImageGetTransparent" ) == 0 ) {

	    result_int = gdImageGetTransparent ( 
	     im  /* GD Image Ptr */ 
	    ) ;
	    int_result = TRUE ;

	  } 

	  else if ( strcmp ( functionStr, "gdImageGetClip" ) == 0 ) {
      
	    gdImageGetClip(
	      im,  /* GD Image Ptr */ 
	      (int*) gHyp_data_buffer(pData1,0),  /* PTR to X1 */
	      (int*) gHyp_data_buffer(pData2,0),  /* PTR to Y1 */
	      (int*) gHyp_data_buffer(pData3,0),  /* PTR to X2 */
	      (int*) gHyp_data_buffer(pData4,0)   /* PTR to Y2 */
	      );
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageGetPixel" ) == 0 ) {
      
	    result_int = gdImageGetPixel ( 
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* X */
	      arg2_int  /* Y */
	    ) ;
	    int_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageGifPtr" ) == 0 ) {
	
	    imagePtr = gdImageGifPtr ( 
	      im,  /* GD Image Ptr */
	      &n /* Pointer to size */
	    ) ;

	    /* Create a HS data structure and copy the results */
	    gHyp_data_setVariable ( pResult, "_data_", TYPE_STRING ) ;
	    gHyp_util_breakStream ( (char*) imagePtr, n, pResult, FALSE ) ;

	    /* Get rid of the memory allocated by GD */
	    gdFree ( imagePtr ) ;

	    list_result = TRUE ;
	  }
	}
	else {

	  /*
	    gdImageGreen
	    gdImageInterlace
	    gdImageJpeg
	    gdImageJpegPtr
	    gdImageLine
	    gdImageOpenPolygon
	    gdImagePaletteCopy
	    gdImagePng
	    gdImagePngPtr
	    gdImagePolygon
	    gdImageRectangle
	    gdImageRed
	    gdImageSX
	    gdImageSY
	    gdImageSaveAlpha
	   */

	  if ( strcmp ( functionStr, "gdImageGreen" ) == 0 ) {
     
	    result_int = gdImageGreen ( 
	      im,  /* GD Image Ptr */ 
	      arg1_int /* C */
	    ) ;
	    int_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageInterlace" ) == 0 ) {
	
	    gdImageInterlace ( 
	      im,  /* GD Image Ptr */
	      arg1_int /* INTERLACE */
	      ) ;
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageJpeg" ) == 0 ) {
	
	    gdImageJpeg ( 
	      im,  /* GD Image Ptr */
	      (FILE*) arg1_handle, /* FILE NAME */
	      arg2_int /* QUALITY */
	      ) ;
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageJpegPtr" ) == 0 ) {
	
	    imagePtr = gdImageJpegPtr ( 
	      im,  /* GD Image Ptr */
	      &n, /* Pointer to size */
	      arg1_int /* QUALITY */
	      ) ;

	    /* Create a HS data structure and copy the results */
	    gHyp_data_setVariable ( pResult, "_data_", TYPE_STRING ) ;
	    gHyp_util_breakStream ( (char*) imagePtr, n, pResult, FALSE ) ;

	    /* Get rid of the memory allocated by GD */
	    gdFree ( imagePtr ) ;

	    list_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageLine" ) == 0 ) {
      
	    gdImageLine( 
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* X1 */
	      arg2_int, /* Y1 */
	      arg3_int, /* X2 */
	      arg4_int, /* Y2 */
	      arg5_int  /* COLOR */
	      ); 
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageOpenPolygon" ) == 0 ) {
      
	    /* Get # of points */
	    n = MIN ( arg2_int, MAX_POINTS ) ;
	    for ( i=0; i<n; i++ ) {

	      pValue = gHyp_data_getChildBySS ( pData1, i ) ;
	      pValue = gHyp_data_getChildByName ( pValue, "x" ) ;
	      points[i].x = gHyp_data_getInt ( pValue, 0, TRUE ) ;

	      pValue = gHyp_data_getChildBySS ( pData1, i ) ;
	      pValue = gHyp_data_getChildByName ( pValue, "y" ) ;
	      points[i].y = gHyp_data_getInt ( pValue, 0, TRUE ) ;
	    }

	    gdImageOpenPolygon(
	      im,  /* GD Image Ptr */ 
	      points, /* POINTS */
	      arg2_int, /* POINTS_TOTAL */
	      arg3_int  /*COLOR */
	      );
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImagePaletteCopy" ) == 0 ) {
	
	    gdImagePaletteCopy ( 
	      im,  /* GD Image Ptr */ 
	      (gdImagePtr) arg1_handle /* SRC */
	      ) ;
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImagePng" ) == 0 ) {
	
	    gdImagePng ( 
	      im,  /* GD Image Ptr */
	      (FILE*) arg1_handle /* FILE NAME */
	      ) ;
	    boolean_result = TRUE ;
	  }

          else if ( strcmp ( functionStr, "gdImagePngPtr" ) == 0 ) {
	
	    imagePtr = gdImagePngPtr ( 
	      im,  /* GD Image Ptr */
	      &n  /* Pointer to size */
	      ) ;

	    /* Create a HS data structure and copy the results */
	    gHyp_data_setVariable ( pResult, "_data_", TYPE_STRING ) ;
	    gHyp_util_breakStream ( (char*) imagePtr, n, pResult, FALSE ) ;

	    /* Get rid of the memory allocated by GD */
	    gdFree ( imagePtr ) ;

	    list_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImagePolygon" ) == 0 ) {

	    /* Get # of points */
	    n = MIN ( arg2_int, MAX_POINTS ) ;
	    for ( i=0; i<n; i++ ) {

	      pValue = gHyp_data_getChildBySS ( pData1, i ) ;
	      pValue = gHyp_data_getChildByName ( pValue, "x" ) ;
	      points[i].x = gHyp_data_getInt ( pValue, 0, TRUE ) ;

	      pValue = gHyp_data_getChildBySS ( pData1, i ) ;
	      pValue = gHyp_data_getChildByName ( pValue, "y" ) ;
	      points[i].y = gHyp_data_getInt ( pValue, 0, TRUE ) ;
	    }

	    gdImagePolygon(
	      im,  /* GD Image Ptr */ 
	      points,   /* POINTS */
	      arg2_int, /* POINTS_TOTAL */
	      arg3_int  /* COLOR */
	      ); 
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageRectangle" ) == 0 ) {

	    gdImageRectangle(
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* X1 */
	      arg2_int, /* Y1 */
	      arg3_int, /* X2 */
	      arg4_int, /* Y2 */
	      arg5_int  /* COLOR */
	      );
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageRed" ) == 0 ) {
     
	    result_int = gdImageRed ( 
	      im,  /* GD Image Ptr */ 
	      arg1_int /* C */
	      ) ;
	    int_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageSX" ) == 0 ) {
     
	    result_int = gdImageSX ( im ) ;
	    int_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageSY" ) == 0 ) {
      
	    result_int = gdImageSY ( im ) ;
	    int_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageSaveAlpha" ) == 0 ) {
      
	    gdImageSaveAlpha( 
	      im,  /* GD Image Ptr */ 
	      arg1_int  /* SAVE_FLAG */
	    ) ;
	    boolean_result = TRUE ;
	  }
	}
      }
      else {

	if ( strcmp ( functionStr, "gdImageStrawberry" ) < 0 ) { 

	  /*
	  gdImageSetAntiAliased
	  gdImageSetAntiAliasedDontBlend
	  gdImageSetBrush
	  gdImageSetClip
	  gdImageSetPixel
	  gdImageSetStyle
	  gdImageSetThickness
	  gdImageSetTile
	  gdImageSharpen
	  gdImageSquareToCircle
	  */

	  if ( strcmp ( functionStr, "gdImageSetAntiAliased" ) == 0 ) {
      
	    gdImageSetAntiAliased( 
	      im,  /* GD Image Ptr */ 
	      arg1_int  /* COLOR */
    	    ) ;
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageSetAntiAliasedDontBlend" ) == 0 ) {
      
	    gdImageSetAntiAliasedDontBlend(
	      im,  /* GD Image Ptr */ 
	      arg1_int,  /* COLOR */
	      arg2_int /* Don't blend */
	      ) ;
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageSetBrush" ) == 0 ) {
    
	    gdImageSetBrush( 
	      im,  /* GD Image Ptr */ 
	      (gdImagePtr) arg1_handle /* BRUSH */
	    ) ;
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageSetTile" ) == 0 ) {
      
	    gdImageSetTile(
	      im,  /* GD Image Ptr */ 
	      (gdImagePtr) arg1_handle /* TILE */
	    );
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageSetStyle" ) == 0 ) {
      
	    gdImageSetStyle(
	      im,  /* GD Image Ptr */ 
	      (int*) gHyp_data_buffer(pData1,0), /* POINTER TO ARRAY */
	      gHyp_data_getCount(pData1)  /* LENGTH OF ARRAY */
	    ) ;
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageSetThickness" ) == 0 ) {
      
	    gdImageSetThickness(
	      im,  /* GD Image Ptr */ 
	      arg1_int  /* THICKNESS */
	    );  
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageSetPixel" ) == 0 ) {
      
	    gdImageSetPixel( 
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* X */
	      arg2_int, /* Y */
	      arg3_int /* COLOR */
	    );
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageSetClip" ) == 0 ) {
      
	    gdImageSetClip(
	      im,  /* GD Image Ptr */ 
	      arg1_int,   /* X1 */
	      arg2_int,   /* Y1 */
	      arg3_int,   /* X2 */
	      arg4_int);  /* Y2 */
	    boolean_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdImageSquareToCircle" ) == 0 ) {
	
	    gdImageSquareToCircle ( 
	      im,  /* GD Image Ptr */ 
	      arg1_int /* RADIUS */
	      ) ;
	    boolean_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageSharpen" ) == 0 ) {
	
	    gdImageSharpen (  
	      im,  /* GD Image Ptr */ 
	      arg1_int /* PCT */
	      ) ;
	    boolean_result = TRUE ;
	  }
	}
	else {

	  /*
	    gdImageString
	    gdImageString16
	    gdImageStringFT
	    gdImageStringFTCircle
	    gdImageStringFTEx
	    gdImageStringTTF
	    gdImageStringTTF
	    gdImageStringUp
	    gdImageStringUp16
	    gdImageTrueColor
	    gdTrueColor
	    gdTrueColorAlpha
	  */

	  if ( strcmp ( functionStr, "gdImageString" ) == 0 ) {
      
	    gdImageString(
	      im,  /* GD Image Ptr */ 
	      (gdFontPtr) arg1_handle, 
	      arg2_int, /* X */ 
	      arg3_int, /* Y */
	      (unsigned char*) arg4_string, /* TEXT */
	      arg5_int  /* COLOR */
	    );
	    boolean_result = TRUE ;
	  }
	  /*
	   * UNICODE
	  else if ( strcmp ( functionStr, "gdImageString16" ) == 0 ) {
      
	    gdImageString16(
	      im,  * GD Image Ptr * 
	      (gdFontPtr) arg1_handle, 
	      arg2_int, * X *
	      arg3_int, * Y *
	      (unsigned char*) arg4_string, * TEXT *
	      arg5_int * COLOR *
	    );
	  }
	  ***/
	  else if ( strcmp ( functionStr, "gdImageStringUp" ) == 0 ) {
      
	    gdImageStringUp(
	      im,  /* GD Image Ptr */ 
	      (gdFontPtr) arg1_handle, 
	      arg2_int, /* X */
	      arg3_int,  /* Y */
	      (unsigned char*) arg4_string, /* TEXT */
	      arg5_int  /* COLOR */
	      );
	    boolean_result = TRUE ;
	  }
	  /***
	   * UNICODE
	   *
	  else if ( strcmp ( functionStr, "gdImageStringUp16" ) == 0 ) {
     
	    gdImageStringUp16(
	      im,  * GD Image Ptr * 
	      (gdFontPtr) arg1_handle, 
	      arg2_int, * X *
	      arg3_int, * Y *
	      arg4_string, * TEXT *
	      arg5_int * COLOR *
	      );
	  }
	  *****/

	  else if ( strcmp ( functionStr, "gdImageStringFT" ) == 0 ) {
     
            /*gHyp_util_debug("gdImageStringFT");*/
	    err = gdImageStringFT ( 
	        im,  /* GD Image Ptr */ 
		(int*) gHyp_data_buffer(pData1,0), /* BRECT */
		arg2_int,	/* FG */
		arg3_string, /* FONTNAME */
		arg4_double, /* PTSIZE */
		arg5_double, /* ANGLE */
		arg6_int, /* X */
		arg7_int, /* Y */
		arg8_string /* TEXT */
                ) ;
            if ( err )
  	      strcpy ( result_str, err ) ;
            else
              strcpy ( result_str, "$ACK" ) ;

	    string_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageStringFTEx" ) == 0 ) {
	    /* Not supported */
	    gHyp_instance_warning ( pAI, "GD function %s is not supported",functionStr);
	    boolean_result = TRUE ;
	    result_boolean = FALSE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageStringFTCircle" ) == 0 ) {

	    err = gdImageStringFTCircle ( 
	      im,  /* GD Image Ptr */ 
	      arg1_int, /* CX */
	      arg2_int,	/* CY */
	      arg3_double, /* RADIUS */
	      arg4_double, /* TEXTRADIUS */
	      arg5_double, /* FILLPORTION */
	      arg6_string, /* FONT */
	      arg7_double, /* POINTS */
	      arg8_string, /* TOP */
	      arg9_string, /* BOTTOM */
	      arg10_int	    /* FGCOLOR */
	      ) ;
            if ( err )
  	      strcpy ( result_str, err ) ;
            else
              strcpy ( result_str, "$ACK" ) ;

	    string_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdImageStringTTF" ) == 0 ) {

	    err = gdImageStringTTF ( 
	        im,  /* GD Image Ptr */ 
		(int*) gHyp_data_buffer(pData1,0), /* BRECT */ 
		arg2_int,	/* FG */
		arg3_string, /* FONTNAME */
		arg4_double, /* PTSIZE */
		arg5_double, /* ANGLE */
		arg6_int, /* X */
		arg7_int, /* Y */
		arg8_string /* TEXT */ 
	      ) ;
            if ( err )
  	      strcpy ( result_str, err ) ;
            else
              strcpy ( result_str, "$ACK" ) ;

	     string_result = TRUE ;
	  }
	  else if ( strcmp ( functionStr, "gdTrueColor" ) == 0 ) {

	    result_int = gdTrueColor ( 
	      arg1_int, /* RED */
	      arg2_int, /* GREEN */
	      arg3_int  /* BLUE */
	    ) ;
	    int_result = TRUE ;
	  } 

	  else if ( strcmp ( functionStr, "gdImageTrueColor" ) == 0 ) {
      
	    result_int = gdImageTrueColor ( im ) ;
	    int_result = TRUE ;
	  }

	  else if ( strcmp ( functionStr, "gdTrueColorAlpha" ) == 0 ) {

	    result_int = gdTrueColorAlpha ( 
	      arg1_int, /* RED */
	      arg2_int, /* GREEN */
	      arg3_int, /* BLUE */
	      arg4_int  /* ALPHA */
	    ) ;
	    int_result = TRUE ;
	  }
	}
      }
    } 

    if ( !int_result &&
	 !handle_result &&
	 !string_result &&
         !boolean_result &&
	 !list_result ) {

      gHyp_instance_warning ( pAI, "GD function %s is not supported",functionStr);
      boolean_result = TRUE ;
      result_boolean = FALSE ;

    }

#endif    /* from AS_GD way up above  */

    if ( int_result )
      gHyp_data_setInt ( pResult, result_int ) ;
    else if ( handle_result )
      gHyp_data_setHandle ( pResult, result_handle ) ;
    else if ( string_result )
      gHyp_data_setStr ( pResult, result_str ) ;
    else if ( boolean_result ) 
      gHyp_data_setBool ( pResult, result_boolean ) ;

    gHyp_stack_push ( pStack, pResult ) ;

    return ;
  }
}

void gHyp_gd_open ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: gd_open ()
   *    Returns TRUE or FALSE
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame
    *pFrame = gHyp_instance_frame ( pAI ) ;

  sParse
    *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData,
      *pResult ;

    int
      n,
      width,
      height,
      argCount = gHyp_parse_argCount ( pParse ) ;

    char value[VALUE_SIZE+1] ;

    FILE 
      *fp ;

#ifdef AS_GD
    gdImagePtr  im = NULL;

#else
    void *im = NULL ;
#endif


    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount !=1 && argCount != 2 )
      gHyp_instance_error (
               pAI, STATUS_ARGUMENT,
               "Invalid arguments. Usage: gd_open ( [fileSpec | width, height ] )");

    if ( argCount == 1 ) {

      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      n = gHyp_data_getStr ( pData, 
			   value, 
			   VALUE_SIZE,
			   gHyp_data_getSubScript(pData),
			   TRUE ) ;
      value[n] = '\0' ;
      fp = fopen ( value, "rb" ) ;

      if ( !fp ) gHyp_instance_error ( pAI, STATUS_IO, "Failed to open %s", value ) ;
 
#ifdef AS_GD
      if ( strstr ( value, "png" ) || strstr ( value, "PNG" ) )
        im = gdImageCreateFromPng( fp ) ;
      else if ( strstr ( value, "jpg" ) || strstr ( value, "JPG" ) )
        im = gdImageCreateFromJpeg( fp ) ;
#else
      im = NULL ;
#endif

      fclose ( fp ) ;
    }
    else {
      /* Get height */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      height = gHyp_data_getInt ( pData, gHyp_data_getSubScript(pData), TRUE ) ;

      /* Get width */
      pData = gHyp_stack_popRvalue ( pStack, pAI ) ;
      width = gHyp_data_getInt ( pData,gHyp_data_getSubScript(pData), TRUE ) ;

#ifdef AS_GD
      /* Create and initialize a gdImagePtr structure */
      im = gdImageCreate( width, height );

#endif
    }
    pResult = gHyp_data_new ( NULL ) ;
    gHyp_data_setHandle ( pResult, (void*) im ) ;
    gHyp_stack_push ( pStack, pResult ) ;
    return ;
  }
}

void gHyp_gd_close ( sInstance *pAI, sCode *pCode, sLOGICAL isPARSE )
{
  /* Description:
   *
   *    PARSE or EXECUTE the built-in function: gd_close ( handle )
   *    Returns TRUE or FALSE
   *
   * Arguments:
   *
   *    pAI                                                     [R]
   *    - pointer to instance object
   *
   *    pCode                                                   [R]
   *    - pointer to code object
   *
   * Return value:
   *
   *    none
   *
   */
  sFrame
    *pFrame = gHyp_instance_frame ( pAI ) ;
  sParse
    *pParse = gHyp_frame_parse ( pFrame ) ;

  if ( isPARSE )

    gHyp_parse_operand ( pParse, pCode, pAI ) ;

  else {

    sStack
      *pStack = gHyp_frame_stack ( pFrame ) ;

    sData
      *pData ;

    int
      argCount = gHyp_parse_argCount ( pParse ) ;

#ifdef AS_GD
    gdImagePtr   im;
#endif

    /* Assume success */
    gHyp_instance_setStatus ( pAI, STATUS_ACKNOWLEDGE ) ;

    if ( argCount != 1 )
      gHyp_instance_error ( pAI, STATUS_ARGUMENT,
        "Invalid arguments. Usage: gd_close ( handle )" ) ;

    /* Get the handle name for gd image */
    pData = gHyp_stack_popRvalue ( pStack, pAI ) ;

#ifdef AS_GD
      im = (gdImagePtr) gHyp_data_getHandle ( pData, gHyp_data_getSubScript(pData), TRUE);
      gdImageDestroy (im) ;
#endif

    gHyp_instance_pushSTATUS ( pAI, pStack ) ;
    return ;
  }
}
