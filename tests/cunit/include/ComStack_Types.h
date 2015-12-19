/*
 * Copyright (c) 2012 QRTECH AB Gothenburg Sweden. All Rights Reserved.
 *
 * This document is provided to you in a non-exclusive agreement, and
 * shall not without QRTECH AB’s written permission be copied, imparted
 * to a third party or be used for any other unauthorized purpose.
 * QRTECH retain the right to reuse or re-license for other purposes.
 *
 * The document is provided "AS IS", WITHOUT WARRANTY and is only
 * supported by QRTECH in its complete original distribution.
 *
 * QRTECH AB  (http://www.qrtech.se)
 * Mejerigatan 1
 * 412 76 Göteborg
 * SWEDEN
 * Tel: +46 31 773 76 00
 *
 */

#ifndef COMSTACK_TYPES_H_
#define COMSTACK_TYPES_H_



/*------------------------------------------------------------------------------
 * Includes
 *----------------------------------------------------------------------------*/
#include "Std_Types.h"

/*------------------------------------------------------------------------------
 * Published parameters
 *----------------------------------------------------------------------------*/
#define COMSTACKTYPE_AR_MAJOR_VERSION 0x02
#define COMSTACKTYPE_AR_MINOR_VERSION 0x00
#define COMSTACKTYPE_AR_PATCH_VERSION 0x01

/*------------------------------------------------------------------------------
 * Defines
 *----------------------------------------------------------------------------*/
#define NTFRSLT_OK 0x00
#define NTFRSLT_E_NOT_OK 0x01
#define NTFRSLT_E_TIMEOUT_A 0x02
#define NTFRSLT_E_TIMEOUT_Bs 0x03
#define NTFRSLT_E_TIMEOUT_Cr 0x04
#define NTFRSLT_E_WRONG_SN 0x05
#define NTFRSLT_E_INVALID_FS 0x06
#define NTFRSLT_E_UNEXP_PDU 0x07
#define NTFRSLT_E_WFT_OVRN 0x08
#define NTFRSLT_E_NO_BUFFER 0x09

#define BUSTRCV_NO_ERROR 0x00
#define BUSTRCV_E_ERROR 0x01
/* own codes example */
#define BUSTRCV_E_FR_LISTEN 0xFE

/*------------------------------------------------------------------------------
 * Typedefs
 *----------------------------------------------------------------------------*/
typedef uint32 PduIdType;
typedef uint16 PduLengthType;

typedef uint8_least PduDataType; /* special */

typedef uint16 NotifResultType;
typedef uint16 BusTrcvErrorType;

typedef struct
{
    PduDataType  *SduDataPtr;
	PduLengthType SduLength;
} PduInfoType;

typedef enum {
	BUFREQ_OK = 0,
	BUFREQ_E_NOT_OK,
	BUFREQ_E_BUSY,
	BUFREQ_E_OVFL
} BufReq_ReturnType;


#endif /*COMSTACK_TYPES_H_*/
