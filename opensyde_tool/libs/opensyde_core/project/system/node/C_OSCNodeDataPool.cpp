//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Data class for a complete data pool (implementation)

   Data class for a complete data pool

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

#include "stwerrors.h"
#include "C_OSCUtils.h"
#include "C_OSCNodeDataPool.h"
#include "CSCLChecksums.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_types;
using namespace stw_opensyde_core;
using namespace stw_errors;

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
C_OSCNodeDataPool::C_OSCNodeDataPool(void) :
   e_Type(eDIAG),
   c_Name("NewDataPool"),
   c_Comment(""),
   s32_RelatedDataBlockIndex(-1),
   q_IsSafety(false),
   u32_NvMStartAddress(0),
   u32_NvMSize(1000),
   c_Lists()
{
   this->au8_Version[0] = 0;
   this->au8_Version[1] = 0;
   this->au8_Version[2] = 0;
   if ((this->e_Type == eDIAG) || (this->e_Type == eNVM))
   {
      c_Lists.resize(1);
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Calculates the hash value over all data

   The hash value is a 32 bit CRC value.
   It is not endian-safe, so it should only be used on the same system it is created on.

   \param[in,out] oru32_HashValue    Hash value with initial [in] value and result [out] value

   \created     21.03.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_OSCNodeDataPool::CalcHash(uint32 & oru32_HashValue) const
{
   stw_scl::C_SCLChecksums::CalcCRC32(&this->e_Type, sizeof(this->e_Type), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(this->c_Name.c_str(), this->c_Name.Length(), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(&this->au8_Version[0], sizeof(this->au8_Version), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(this->c_Comment.c_str(), this->c_Comment.Length(), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(&this->s32_RelatedDataBlockIndex, sizeof(this->s32_RelatedDataBlockIndex),
                                      oru32_HashValue);
   // pc_RelatedApplication is dynamic
   stw_scl::C_SCLChecksums::CalcCRC32(&this->q_IsSafety, sizeof(this->q_IsSafety), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(&this->u32_NvMStartAddress, sizeof(this->u32_NvMStartAddress), oru32_HashValue);
   stw_scl::C_SCLChecksums::CalcCRC32(&this->u32_NvMSize, sizeof(this->u32_NvMSize), oru32_HashValue);

   for (uint32 u32_Counter = 0U; u32_Counter < this->c_Lists.size(); ++u32_Counter)
   {
      this->c_Lists[u32_Counter].CalcHash(oru32_HashValue);
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Calculates the hash value over data pool definition

   The hash value is a 32 bit CRC value.
   Only essential data are covered.
   It is endian-safe, so it may be used on systems with different endianness.

   \param[in,out] oru32_HashValue    Hash value with initial [in] value and result [out] value

   \created     30.08.2017  STW/U.Roesch
*/
//-----------------------------------------------------------------------------
void C_OSCNodeDataPool::CalcDefinitionHash(uint32 & oru32_HashValue) const
{
   uint8 au8_Data[4];

   stw_scl::C_SCLChecksums::CalcCRC32(&this->e_Type, 1U, oru32_HashValue);

   au8_Data[0] = static_cast<uint8>(this->u32_NvMStartAddress);
   au8_Data[1] = static_cast<uint8>(this->u32_NvMStartAddress >> 8);
   au8_Data[2] = static_cast<uint8>(this->u32_NvMStartAddress >> 16);
   au8_Data[3] = static_cast<uint8>(this->u32_NvMStartAddress >> 24);
   stw_scl::C_SCLChecksums::CalcCRC32(&au8_Data[0], sizeof(this->u32_NvMStartAddress), oru32_HashValue);

   au8_Data[0] = static_cast<uint8>(this->u32_NvMSize);
   au8_Data[1] = static_cast<uint8>(this->u32_NvMSize >> 8);
   au8_Data[2] = static_cast<uint8>(this->u32_NvMSize >> 16);
   au8_Data[3] = static_cast<uint8>(this->u32_NvMSize >> 24);
   stw_scl::C_SCLChecksums::CalcCRC32(&au8_Data[0], sizeof(this->u32_NvMSize), oru32_HashValue);

   for (uint32 u32_ListCount = 0U; u32_ListCount < this->c_Lists.size(); u32_ListCount++)
   {
      stw_scl::C_SCLChecksums::CalcCRC32(&this->c_Lists[u32_ListCount].q_NvMCRCActive, 1U, oru32_HashValue);

      for (uint32 u32_ElementCount = 0U; u32_ElementCount < this->c_Lists[u32_ListCount].c_Elements.size();
           u32_ElementCount++)
      {
         const uint32 u32_Size = this->c_Lists[u32_ListCount].c_Elements[u32_ElementCount].GetSizeByte();
         const uint8 u8_Type = static_cast<uint8>(this->c_Lists[u32_ListCount].c_Elements[u32_ElementCount].GetType());
         au8_Data[0] = static_cast<uint8>(u32_Size);
         au8_Data[1] = static_cast<uint8>(u32_Size >> 8);
         au8_Data[2] = static_cast<uint8>(u32_Size >> 16);
         au8_Data[3] = static_cast<uint8>(u32_Size >> 24);
         stw_scl::C_SCLChecksums::CalcCRC32(&au8_Data[0], sizeof(u32_Size), oru32_HashValue);
         stw_scl::C_SCLChecksums::CalcCRC32(&u8_Type, sizeof(u8_Type), oru32_HashValue);
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Move list in data pool

   \param[in] oru32_Start  Start index
   \param[in] oru32_Target Target index

   \created     26.01.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OSCNodeDataPool::MoveList(const stw_types::uint32 & oru32_Start, const stw_types::uint32 & oru32_Target)
{
   if ((oru32_Start < this->c_Lists.size()) && (oru32_Target < this->c_Lists.size()))
   {
      //Copy physically
      const C_OSCNodeDataPoolList c_ListData = this->c_Lists[oru32_Start];
      //Erase
      this->c_Lists.erase(this->c_Lists.begin() + oru32_Start);
      //Insert
      this->c_Lists.insert(this->c_Lists.begin() + oru32_Target, c_ListData);
      RecalculateAddress();
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Recalculate data pool list addresses

   \created     26.01.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OSCNodeDataPool::RecalculateAddress(void)
{
   uint32 u32_Offset = this->u32_NvMStartAddress;

   for (uint32 u32_ItList = 0; u32_ItList < this->c_Lists.size(); ++u32_ItList)
   {
      C_OSCNodeDataPoolList & rc_List = this->c_Lists[u32_ItList];
      rc_List.u32_NvMStartAddress = u32_Offset;
      rc_List.RecalculateAddress();
      u32_Offset += rc_List.u32_NvMSize;
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get number of bytes occupied by variables

   \return
   Number of bytes occupied by variables

   \created     22.02.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
uint32 C_OSCNodeDataPool::GetNumBytesUsed(void) const
{
   uint32 u32_Retval = 0;

   for (uint32 u32_ItList = 0; u32_ItList < this->c_Lists.size(); ++u32_ItList)
   {
      const C_OSCNodeDataPoolList & rc_NodeDataPoolList = this->c_Lists[u32_ItList];
      u32_Retval += rc_NodeDataPoolList.GetNumBytesUsed();
   }
   return u32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get number of bytes not occupied by variables

   \return
   Number of bytes not occupied by variables

   \created     22.02.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
sint32 C_OSCNodeDataPool::GetFreeBytes(void) const
{
   return static_cast<sint32>(static_cast<sint64>(u32_NvMSize) - static_cast<sint64>(GetNumBytesUsed()));
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get number of bytes occupied by lists

   \return
   Number of bytes occupied by lists

   \created     22.02.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
uint32 C_OSCNodeDataPool::GetListsSize(void) const
{
   uint32 u32_Retval = 0;

   for (uint32 u32_ItList = 0; u32_ItList < this->c_Lists.size(); ++u32_ItList)
   {
      const C_OSCNodeDataPoolList & rc_NodeDataPoolList = this->c_Lists[u32_ItList];
      u32_Retval += rc_NodeDataPoolList.u32_NvMSize;
   }
   return u32_Retval;
}

//-----------------------------------------------------------------------------
/*!
   \brief   Check error for specified list

   \param[in]  oru_ListIndex             Node data pool list index
   \param[out] opq_NameConflict          Name conflict
   \param[out] opq_NameInvalid           Name not usable as variable
   \param[out] opq_UsageInvalid          Usage over 100.0%
   \param[out] opq_OutOfDataPool         List out of data pool borders
   \param[out] opq_DataSetsInvalid       One or more elements are invalid
   \param[out] opq_ElementsInvalid       One or more elements are invalid
   \param[out] opc_InvalidDataSetIndices Indices of invalid data sets
   \param[out] opc_InvalidElementIndices Indices of invalid elements

   \return
   C_NO_ERR   No error
   C_CONFIG   Name conflict
   C_NOACT    Name not usable as variable
   C_RANGE    Usage over 100 percent

   \created     23.02.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_OSCNodeDataPool::CheckErrorList(const uint32 & oru_ListIndex, bool * const opq_NameConflict,
                                       bool * const opq_NameInvalid, bool * const opq_UsageInvalid,
                                       bool * const opq_OutOfDataPool, bool * const opq_DataSetsInvalid,
                                       bool * const opq_ElementsInvalid,
                                       std::vector<uint32> * const opc_InvalidDataSetIndices,
                                       std::vector<uint32> * const opc_InvalidElementIndices) const
{
   //Init
   if (oru_ListIndex < this->c_Lists.size())
   {
      const C_OSCNodeDataPoolList & rc_CheckedList = this->c_Lists[oru_ListIndex];
      //Check variable name
      if (opq_NameInvalid != NULL)
      {
         if (C_OSCUtils::h_CheckValidCName(rc_CheckedList.c_Name) == false)
         {
            *opq_NameInvalid = true;
         }
         else
         {
            *opq_NameInvalid = false;
         }
      }
      //Name conflict
      if (opq_NameConflict != NULL)
      {
         *opq_NameConflict = false;
         for (uint32 u32_ItElement = 0; u32_ItElement < this->c_Lists.size(); ++u32_ItElement)
         {
            if (u32_ItElement != oru_ListIndex)
            {
               const C_OSCNodeDataPoolList & rc_List = this->c_Lists[u32_ItElement];
               if (rc_CheckedList.c_Name.LowerCase() == rc_List.c_Name.LowerCase())
               {
                  *opq_NameConflict = true;
               }
            }
         }
      }

      //Size
      if (opq_UsageInvalid != NULL)
      {
         //Usage
         if (rc_CheckedList.u32_NvMSize < rc_CheckedList.GetNumBytesUsed())
         {
            *opq_UsageInvalid = true;
         }
         else
         {
            *opq_UsageInvalid = false;
         }
      }
      if (opq_OutOfDataPool != NULL)
      {
         uint32 u32_NvmSizeReserved = 0;
         //Check reserved size
         for (uint32 u32_ItList = 0; u32_ItList <= oru_ListIndex; ++u32_ItList)
         {
            const C_OSCNodeDataPoolList & rc_CurrentList = this->c_Lists[u32_ItList];
            u32_NvmSizeReserved += rc_CurrentList.u32_NvMSize;
         }
         if (u32_NvmSizeReserved <= this->u32_NvMSize)
         {
            *opq_OutOfDataPool = false;
         }
         else
         {
            *opq_OutOfDataPool = true;
         }
      }
      //Check data sets
      if (opq_DataSetsInvalid != NULL)
      {
         bool q_NameConflict;
         bool q_NameInvalid;
         *opq_DataSetsInvalid = false;
         for (uint32 u32_ItDataSet = 0;
              (u32_ItDataSet < rc_CheckedList.c_DataSets.size()) &&
              ((*opq_DataSetsInvalid == false) || (opc_InvalidDataSetIndices != NULL));
              ++u32_ItDataSet)
         {
            q_NameConflict = false;
            q_NameInvalid = false;
            rc_CheckedList.CheckErrorDataSet(u32_ItDataSet, &q_NameConflict, &q_NameInvalid);
            if ((q_NameConflict == true) || (q_NameInvalid == true))
            {
               *opq_DataSetsInvalid = true;
               if (opc_InvalidDataSetIndices != NULL)
               {
                  opc_InvalidDataSetIndices->push_back(u32_ItDataSet);
               }
            }
         }
      }
      //Check elements
      if (opq_ElementsInvalid != NULL)
      {
         bool q_NameConflict;
         bool q_NameInvalid;
         bool q_MinOverMax;
         bool q_DataSetInvalid;
         *opq_ElementsInvalid = false;
         for (uint32 u32_ItElement = 0;
              (u32_ItElement < rc_CheckedList.c_Elements.size()) &&
              ((*opq_ElementsInvalid == false) || (opc_InvalidElementIndices != NULL));
              ++u32_ItElement)
         {
            q_NameConflict = false;
            q_NameInvalid = false;
            q_MinOverMax = false;
            q_DataSetInvalid = false;
            rc_CheckedList.CheckErrorElement(u32_ItElement, &q_NameConflict, &q_NameInvalid, &q_MinOverMax,
                                             &q_DataSetInvalid, NULL);
            if ((((q_NameConflict == true) || (q_NameInvalid == true)) || (q_MinOverMax == true)) ||
                (q_DataSetInvalid == true))
            {
               *opq_ElementsInvalid = true;
               if (opc_InvalidElementIndices != NULL)
               {
                  opc_InvalidElementIndices->push_back(u32_ItElement);
               }
            }
         }
      }
   }
   else
   {
      if (opq_NameConflict != NULL)
      {
         *opq_NameConflict = false;
      }
      if (opq_NameInvalid != NULL)
      {
         *opq_NameInvalid = false;
      }
      if (opq_UsageInvalid != NULL)
      {
         *opq_UsageInvalid = false;
      }
      if (opq_DataSetsInvalid != NULL)
      {
         *opq_DataSetsInvalid = false;
      }
      if (opq_ElementsInvalid != NULL)
      {
         *opq_ElementsInvalid = false;
      }
   }
}