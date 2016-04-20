/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support 
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 * - Neither the name of the copyright holders nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// Standard output methods for reporting debug information, warnings and
/// errors, which can be easily be turned on/off.
///
/// !Usage
/// -# Initialize the DBGU using TRACE_CONFIGURE() if you intend to eventually
///    disable ALL traces; otherwise use DBGU_Configure().
/// -# Uses the TRACE_DEBUG(), TRACE_INFO(), TRACE_WARNING(), TRACE_ERROR()
///    TRACE_FATAL() macros to output traces throughout the program.
/// -# Each type of trace has a level : Debug 5, Info 4, Warning 3, Error 2
///    and Fatal 1. Disable a group of traces by changing the value of 
///    TRACE_LEVEL during compilation; traces with a level bigger than TRACE_LEVEL 
///    are not generated. To generate no trace, use the reserved value 0.
/// -# Trace disabling can be static or dynamic. If dynamic disabling is selected
///    the trace level can be modified in runtime. If static disabling is selected
///    the disabled traces are not compiled.
///
/// !Trace level description
/// -# TRACE_DEBUG (5): Traces whose only purpose is for debugging the program, 
///    and which do not produce meaningful information otherwise.
/// -# TRACE_INFO (4): Informational trace about the program execution. Should  
///    enable the user to see the execution flow.
/// -# TRACE_WARNING (3): Indicates that a minor error has happened. In most case
///    it can be discarded safely; it may even be expected.
/// -# TRACE_ERROR (2): Indicates an error which may not stop the program execution, 
///    but which indicates there is a problem with the code.
/// -# TRACE_FATAL (1): Indicates a major error which prevents the program from going
///    any further.

//------------------------------------------------------------------------------

#ifndef TRACE_H
#define TRACE_H

//------------------------------------------------------------------------------
//         Headers
//------------------------------------------------------------------------------

#include <board.h>
#include <pio/pio.h>
#include <stdio.h>

// Select the trace interface
// (add usart.o file in makefile if Usart interface is selected)
#define TRACE_DBGU 1
//#define TRACE_USART_0 1
//#define TRACE_USART_1 1
//#define TRACE_USART_2 1

#if defined(TRACE_DBGU)
#include <dbgu/dbgu.h>
#else
#include <usart/usart.h>
#endif

//------------------------------------------------------------------------------
//         Global Definitions
//------------------------------------------------------------------------------

/// Softpack Version
#define SOFTPACK_VERSION "1.6"

#define TRACE_LEVEL_DEBUG      5
#define TRACE_LEVEL_INFO       4
#define TRACE_LEVEL_WARNING    3
#define TRACE_LEVEL_ERROR      2
#define TRACE_LEVEL_FATAL      1
#define TRACE_LEVEL_NO_TRACE   0

// By default, all traces are output except the debug one.
#if !defined(TRACE_LEVEL)    
#define TRACE_LEVEL TRACE_LEVEL_INFO
#endif

// By default, trace level is static (not dynamic)
#if !defined(DYN_TRACES)
#define DYN_TRACES 0
#endif

#if defined(NOTRACE)
#error "Error: NOTRACE has to be not defined !"
#endif

#undef NOTRACE
#if (DYN_TRACES==0)
    #if (TRACE_LEVEL == TRACE_LEVEL_NO_TRACE)
        #define NOTRACE
    #endif
#endif

//------------------------------------------------------------------------------
//         Global Macros
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// Initializes the trace for normal project
/// \param mode  DBGU mode.
/// \param baudrate  DBGU baudrate.
/// \param mck  Master clock frequency.
//------------------------------------------------------------------------------
#if defined(TRACE_DBGU)
    #define TRACE_CONFIGURE(mode, baudrate, mck) { \
        const Pin pinsDbgu[] = {PINS_DBGU}; \
        PIO_Configure(pinsDbgu, PIO_LISTSIZE(pinsDbgu)); \
        DBGU_Configure(mode, baudrate, mck); \
    }
#elif defined(TRACE_USART_0)
    #define TRACE_CONFIGURE(mode, baudrate, mck) { \
        const Pin pinsUsart[] = {PIN_USART0_TXD, PIN_USART0_RXD}; \
        AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_US0;\
        AT91C_BASE_US0->US_IDR = 0xFFFFFFFF;\
        PIO_Configure(pinsUsart, PIO_LISTSIZE(pinsUsart)); \
        USART_Configure(AT91C_BASE_US0,USART_MODE_ASYNCHRONOUS, baudrate, mck); \
        USART_SetTransmitterEnabled(AT91C_BASE_US0,1);\
        USART_SetReceiverEnabled(AT91C_BASE_US0,1);\
    }
#elif defined(TRACE_USART_1)
    #define TRACE_CONFIGURE(mode, baudrate, mck) { \
        const Pin pinsUsart[] = {PIN_USART1_TXD, PIN_USART1_RXD}; \
        AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_US1;\
        AT91C_BASE_US1->US_IDR = 0xFFFFFFFF;\
        PIO_Configure(pinsUsart, PIO_LISTSIZE(pinsUsart)); \
        USART_Configure(AT91C_BASE_US1,USART_MODE_ASYNCHRONOUS, baudrate, mck); \
        USART_SetTransmitterEnabled(AT91C_BASE_US1,1);\
        USART_SetReceiverEnabled(AT91C_BASE_US1,1);\
    }
#elif defined(TRACE_USART_2)
    #define TRACE_CONFIGURE(mode, baudrate, mck) { \
        const Pin pinsUsart[] = {PIN_USART2_TXD, PIN_USART2_RXD}; \
        AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_US2;\
        AT91C_BASE_US2->US_IDR = 0xFFFFFFFF;\
        PIO_Configure(pinsUsart, PIO_LISTSIZE(pinsUsart)); \
        USART_Configure(AT91C_BASE_US2,USART_MODE_ASYNCHRONOUS, baudrate, mck); \
        USART_SetTransmitterEnabled(AT91C_BASE_US2,1);\
        USART_SetReceiverEnabled(AT91C_BASE_US2,1);\
    }
#endif

//------------------------------------------------------------------------------
/// Initializes the trace for ISP project
/// \param mode  DBGU mode.
/// \param baudrate  DBGU baudrate.
/// \param mck  Master clock frequency.
//------------------------------------------------------------------------------
#if (TRACE_LEVEL==0) && (DYN_TRACES==0)
    #define TRACE_CONFIGURE_ISP(mode, baudrate, mck) {}
#elif defined(TRACE_DBGU)
    #define TRACE_CONFIGURE_ISP(mode, baudrate, mck) { \
        const Pin pinsDbgu[] = {PINS_DBGU}; \
        PIO_Configure(pinsDbgu, PIO_LISTSIZE(pinsDbgu)); \
        DBGU_Configure(mode, baudrate, mck); \
    }
#elif defined(TRACE_USART_0)
    #define TRACE_CONFIGURE_ISP(mode, baudrate, mck) { \
        const Pin pinsUsart[] = {PIN_USART0_TXD, PIN_USART0_RXD}; \
        AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_US0;\
        AT91C_BASE_US0->US_IDR = 0xFFFFFFFF;\
        PIO_Configure(pinsUsart, PIO_LISTSIZE(pinsUsart)); \
        USART_Configure(AT91C_BASE_US0,USART_MODE_ASYNCHRONOUS, baudrate, mck); \
        USART_SetTransmitterEnabled(AT91C_BASE_US0,1);\
        USART_SetReceiverEnabled(AT91C_BASE_US0,1);\
    }
#elif defined(TRACE_USART_1)
    #define TRACE_CONFIGURE_ISP(mode, baudrate, mck) { \
        const Pin pinsUsart[] = {PIN_USART1_TXD, PIN_USART1_RXD}; \
        AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_US1;\
        AT91C_BASE_US1->US_IDR = 0xFFFFFFFF;\
        PIO_Configure(pinsUsart, PIO_LISTSIZE(pinsUsart)); \
        USART_Configure(AT91C_BASE_US1,USART_MODE_ASYNCHRONOUS, baudrate, mck); \
        USART_SetTransmitterEnabled(AT91C_BASE_US1,1);\
        USART_SetReceiverEnabled(AT91C_BASE_US1,1);\
    }
#elif defined(TRACE_USART_2)
    #define TRACE_CONFIGURE_ISP(mode, baudrate, mck) { \
        const Pin pinsUsart[] = {PIN_USART2_TXD, PIN_USART2_RXD}; \
        AT91C_BASE_PMC->PMC_PCER = 1 << AT91C_ID_US2;\
        AT91C_BASE_US2->US_IDR = 0xFFFFFFFF;\
        PIO_Configure(pinsUsart, PIO_LISTSIZE(pinsUsart)); \
        USART_Configure(AT91C_BASE_US2,USART_MODE_ASYNCHRONOUS, baudrate, mck); \
        USART_SetTransmitterEnabled(AT91C_BASE_US2,1);\
        USART_SetReceiverEnabled(AT91C_BASE_US2,1);\
    }
#endif

//------------------------------------------------------------------------------
/// Macros TRACE_PutChar & TRACE_GetChar & TRACE_IsRxReady
//------------------------------------------------------------------------------
#if defined(TRACE_DBGU)    
    #define TRACE_PutChar(c)  DBGU_PutChar(c)
    #define TRACE_GetChar()   DBGU_GetChar()
    #define TRACE_IsRxReady() DBGU_IsRxReady()
#elif defined(TRACE_USART_0)
    #define TRACE_PutChar(c)  USART_PutChar(AT91C_BASE_US0, c)
    #define TRACE_GetChar()   USART_GetChar(AT91C_BASE_US0)
    #define TRACE_IsRxReady() USART_IsRxReady(AT91C_BASE_US0)
#elif defined(TRACE_USART_1)
    #define TRACE_PutChar(c)  USART_PutChar(AT91C_BASE_US1, c)
    #define TRACE_GetChar()   USART_GetChar(AT91C_BASE_US1)
    #define TRACE_IsRxReady() USART_IsRxReady(AT91C_BASE_US1)
#elif defined(TRACE_USART_2)
    #define TRACE_PutChar(c)  USART_PutChar(AT91C_BASE_US2, c)
    #define TRACE_GetChar()   USART_GetChar(AT91C_BASE_US2)
    #define TRACE_IsRxReady() USART_IsRxReady(AT91C_BASE_US2)
#endif

//------------------------------------------------------------------------------
/// Outputs a formatted string using <printf> if the log level is high
/// enough. Can be disabled by defining TRACE_LEVEL=0 during compilation.
/// \param format  Formatted string to output.
/// \param ...  Additional parameters depending on formatted string.
//------------------------------------------------------------------------------
#if defined(NOTRACE)

// Empty macro
#define TRACE_DEBUG(...)      { }
#define TRACE_INFO(...)       { }
#define TRACE_WARNING(...)    { }               
#define TRACE_ERROR(...)      { }
#define TRACE_FATAL(...)      { while(1); }

#define TRACE_DEBUG_WP(...)   { }
#define TRACE_INFO_WP(...)    { }
#define TRACE_WARNING_WP(...) { }
#define TRACE_ERROR_WP(...)   { }
#define TRACE_FATAL_WP(...)   { while(1); }

#elif (DYN_TRACES == 1)

// Trace output depends on traceLevel value
#define TRACE_DEBUG(...)      { if (traceLevel >= TRACE_LEVEL_DEBUG)   { printf("-D- " __VA_ARGS__); } }
#define TRACE_INFO(...)       { if (traceLevel >= TRACE_LEVEL_INFO)    { printf("-I- " __VA_ARGS__); } }
#define TRACE_WARNING(...)    { if (traceLevel >= TRACE_LEVEL_WARNING) { printf("-W- " __VA_ARGS__); } }
#define TRACE_ERROR(...)      { if (traceLevel >= TRACE_LEVEL_ERROR)   { printf("-E- " __VA_ARGS__); } }
#define TRACE_FATAL(...)      { if (traceLevel >= TRACE_LEVEL_FATAL)   { printf("-F- " __VA_ARGS__); while(1); } }

#define TRACE_DEBUG_WP(...)   { if (traceLevel >= TRACE_LEVEL_DEBUG)   { printf(__VA_ARGS__); } }
#define TRACE_INFO_WP(...)    { if (traceLevel >= TRACE_LEVEL_INFO)    { printf(__VA_ARGS__); } }
#define TRACE_WARNING_WP(...) { if (traceLevel >= TRACE_LEVEL_WARNING) { printf(__VA_ARGS__); } }
#define TRACE_ERROR_WP(...)   { if (traceLevel >= TRACE_LEVEL_ERROR)   { printf(__VA_ARGS__); } }
#define TRACE_FATAL_WP(...)   { if (traceLevel >= TRACE_LEVEL_FATAL)   { printf(__VA_ARGS__); while(1); } }

#else

// Trace compilation depends on TRACE_LEVEL value
#if (TRACE_LEVEL >= TRACE_LEVEL_DEBUG)
#define TRACE_DEBUG(...)      { printf("-D- " __VA_ARGS__); }
#define TRACE_DEBUG_WP(...)   { printf(__VA_ARGS__); }
#else
#define TRACE_DEBUG(...)      { }
#define TRACE_DEBUG_WP(...)   { }
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_INFO)
#define TRACE_INFO(...)       { printf("-I- " __VA_ARGS__); }
#define TRACE_INFO_WP(...)    { printf(__VA_ARGS__); }
#else
#define TRACE_INFO(...)       { }
#define TRACE_INFO_WP(...)    { }
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_WARNING)
#define TRACE_WARNING(...)    { printf("-W- " __VA_ARGS__); }
#define TRACE_WARNING_WP(...) { printf(__VA_ARGS__); }
#else
#define TRACE_WARNING(...)    { }
#define TRACE_WARNING_WP(...) { }
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_ERROR)
#define TRACE_ERROR(...)      { printf("-E- " __VA_ARGS__); }
#define TRACE_ERROR_WP(...)   { printf(__VA_ARGS__); }
#else
#define TRACE_ERROR(...)      { }
#define TRACE_ERROR_WP(...)   { }
#endif

#if (TRACE_LEVEL >= TRACE_LEVEL_FATAL)
#define TRACE_FATAL(...)      { printf("-F- " __VA_ARGS__); while(1); }
#define TRACE_FATAL_WP(...)   { printf(__VA_ARGS__); while(1); }
#else
#define TRACE_FATAL(...)      { while(1); }
#define TRACE_FATAL_WP(...)   { while(1); }
#endif

#endif


//------------------------------------------------------------------------------
//         Exported variables
//------------------------------------------------------------------------------
// Depending on DYN_TRACES, traceLevel is a modifable runtime variable
// or a define
#if !defined(NOTRACE) && (DYN_TRACES == 1)
    extern unsigned int traceLevel;
#endif

//------------------------------------------------------------------------------
//         Global Functions
//------------------------------------------------------------------------------

extern void          TRACE_DumpFrame(unsigned char *pFrame, unsigned int size);

extern void          TRACE_DumpMemory(unsigned char *pBuffer, unsigned int size, unsigned int address);

extern unsigned char TRACE_GetInteger(unsigned int *pValue);

extern unsigned char TRACE_GetIntegerMinMax(unsigned int *pValue, unsigned int min, unsigned int max);

extern unsigned char TRACE_GetHexa32(unsigned int *pValue);

#endif //#ifndef TRACE_H

