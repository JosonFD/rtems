/**
 * @file
 *
 * @ingroup RTEMSScoreAssert
 *
 * @brief Information for the Assert Handler
 */

/*
 * Copyright (c) 2013-2014 embedded brains GmbH.  All rights reserved.
 *
 *  embedded brains GmbH
 *  Dornierstr. 4
 *  82178 Puchheim
 *  Germany
 *  <rtems@embedded-brains.de>
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rtems.org/license/LICENSE.
 */

#ifndef _RTEMS_SCORE_ASSERT_H
#define _RTEMS_SCORE_ASSERT_H

#include <rtems/score/basedefs.h>

/**
 * @defgroup RTEMSScoreAssert Assert Handler
 *
 * @ingroup RTEMSScore
 *
 * @brief Support for Assert Statements
 *
 * @{
 */

#if defined( RTEMS_DEBUG )
  #include <assert.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
 * @brief Assertion similar to assert() controlled via RTEMS_DEBUG instead of
 * NDEBUG.
 */
#if defined( RTEMS_DEBUG )

  /**
   * @brief Macro with method name used in assert output
   *
   * Given the variations in compilers and standards, we have to poke a bit.
   *
   * @note This is based on the code in newlib's assert.h.
   */
  #ifndef __RTEMS_ASSERT_FUNCTION
    /* Use g++'s demangled names in C++.  */
    #if defined __cplusplus && defined __GNUC__
      #define __RTEMS_ASSERT_FUNCTION __PRETTY_FUNCTION__

    /* C99 requires the use of __func__.  */
    #elif __STDC_VERSION__ >= 199901L
      #define __RTEMS_ASSERT_FUNCTION __func__

    /* Older versions of gcc don't have __func__ but can use __FUNCTION__.  */
    #elif __GNUC__ >= 2
      #define __RTEMS_ASSERT_FUNCTION __FUNCTION__

    /* failed to detect __func__ support.  */
    #else
      #define __RTEMS_ASSERT_FUNCTION ((char *) 0)
    #endif
  #endif /* !__RTEMS_ASSERT_FUNCTION */

  #if !defined( RTEMS_SCHEDSIM )
    /* normal build is newlib. */

    void __assert_func(const char *, int, const char *, const char *)
      RTEMS_NO_RETURN;

    #define _Assert( _e ) \
       ( ( _e ) ? \
         ( void ) 0 : \
           __assert_func( __FILE__, __LINE__, __RTEMS_ASSERT_FUNCTION, #_e ) )

  #elif defined(__linux__)
    /* Scheduler simulator has only beed tested on glibc. */
    #define _Assert( _e ) \
     ( ( _e ) ? \
       ( void ) 0 : \
         __assert_fail( #_e, __FILE__, __LINE__, __RTEMS_ASSERT_FUNCTION ) )
  #else
    #error "Implement RTEMS assert support for this C Library"
  #endif

#else
  #define _Assert( _e ) ( ( void ) 0 )
#endif

/**
 * @brief Like _Assert(), but only armed if RTEMS_SMP is defined.
 */
#if defined( RTEMS_SMP )
  #define _SMP_Assert( _e ) _Assert( _e )
#else
  #define _SMP_Assert( _e ) ( ( void ) 0 )
#endif

/**
 * @brief Returns true if thread dispatching is allowed.
 *
 * Thread dispatching can be repressed via _Thread_Disable_dispatch() or
 * _ISR_Local_disable().
 */
#if defined( RTEMS_DEBUG )
  bool _Debug_Is_thread_dispatching_allowed( void );
#endif

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */

#endif /* _RTEMS_SCORE_ASSERT_H */
