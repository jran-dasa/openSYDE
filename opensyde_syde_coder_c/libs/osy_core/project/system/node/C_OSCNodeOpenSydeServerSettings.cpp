//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Data class for openSYDE server specific settings(implementation)

   Data class for openSYDE server specific settings

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     22.12.2016  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include "C_OSCNodeOpenSydeServerSettings.h"

#include "CSCLChecksums.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_types;
using namespace stw_opensyde_core;

/* -- Module Global Constants ---------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

/* -- Global Variables ----------------------------------------------------- */

/* -- Module Global Variables ---------------------------------------------- */

/* -- Module Global Function Prototypes ------------------------------------ */

/* -- Implementation ------------------------------------------------------- */

//-----------------------------------------------------------------------------
/*!
   \brief   Default constructor

   \created     22.12.2016  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
C_OSCNodeOpenSYDEServerSettings::C_OSCNodeOpenSYDEServerSettings(void)
{
   this->Initialize();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Initialize class content

   Clean up.

   \created     25.01.2018  STW/A.Stangl
*/
//-----------------------------------------------------------------------------
void C_OSCNodeOpenSYDEServerSettings::Initialize(void)
{
   u8_MaxClients = 1U;
   u8_MaxParallelTransmissions = 16U;
   s16_DPDDataBlockIndex = -1;
   u16_MaxMessageBufferTx = 512U;
   u16_MaxRoutingMessageBufferRx = 512U;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Calculates the hash value over all data

   The hash value is a 32 bit CRC value.

   \param[in,out] oru32_HashValue    Hash value with initial [in] value and result [out] value

   \created     21.03.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_OSCNodeOpenSYDEServerSettings::CalcHash(uint32 & oru32_HashValue) const
{
   stw_scl::C_SCLChecksums::CalcCRC32(&this->u8_MaxClients, sizeof(this->u8_MaxClients), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(&this->u8_MaxParallelTransmissions, sizeof(this->u8_MaxParallelTransmissions),
                                      oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(&this->s16_DPDDataBlockIndex, sizeof(this->s16_DPDDataBlockIndex),
                                      oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(&this->u16_MaxMessageBufferTx, sizeof(this->u16_MaxMessageBufferTx),
                                      oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(&this->u16_MaxRoutingMessageBufferRx, sizeof(this->u16_MaxRoutingMessageBufferRx),
                                      oru32_HashValue);
}