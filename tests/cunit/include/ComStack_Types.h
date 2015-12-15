/*******************************************************************
 * File Creator:    Alborz Sedaghat Amoli - alborz.sedaghat@qrtech.se
 * 					Björn Siby - bjorn.siby@qrtech.se
 * File Created: 	2008-11-24
 * Description:
 *      See Autosar documentation:
 *      Specification of DIO Driver V2.2.2
 * $History$:
 *
 * Version 0.5 2011-02-07 Edited by Robert Lavin for HVFE.
 *
 *******************************************************************/


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
