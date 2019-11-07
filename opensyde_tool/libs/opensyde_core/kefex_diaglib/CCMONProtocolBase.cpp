//-----------------------------------------------------------------------------
/*!
   \internal   (todo: remove this line for header files)
   \file
   \brief       short description (note: main module description should be in .cpp file)

   detailed description

   \implementation
   project     project name
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     dd.mm.yyyy  STW/J.Doe
   \endimplementation
*/
//-----------------------------------------------------------------------------
#include "precomp_headers.h" //pre-compiled headers
#ifdef __BORLANDC__          //putting the pragmas in the config-header will not work
#pragma hdrstop
#pragma package(smart_init)
#endif

#include "stwtypes.h"
#include "stwerrors.h"
#include "CCMONProtocolBase.h"

#include "CSCLString.h"

//---------------------------------------------------------------------------

using namespace stw_types;
using namespace stw_errors;
using namespace stw_cmon_protocol;
using namespace stw_scl;

//-----------------------------------------------------------------------------
/*!
   \brief    save protocol specific parameters to INI file

   Nothing to save by default.
   Overload if we have something to save (use protocol abbreviation as prefix for directives).

   \param[in,out]   orc_IniFile  opened INI file to write to
   \param[in]       orc_Section  section in INI file to write to

   \return
   C_NO_ERR  -> data added to INI file (or: nothing to save)
   else      -> error writing data
*/
//-----------------------------------------------------------------------------
sint32 C_CMONProtocolBase::SaveParamsToIni(C_SCLIniFile & orc_IniFile, const C_SCLString & orc_Section)
{
   (void)orc_IniFile;
   (void)orc_Section;
   return C_NO_ERR; //nothing to save by default
}

//-----------------------------------------------------------------------------
/*!
   \brief    load protocol specific parameters from INI file

   Nothing to load by default.
   Overload if we have something to load (use protocol abbreviation as prefix for directives).

   \param[in,out]   orc_IniFile  opened INI file to read from
   \param[in]       orc_Section  section in INI file to read from

   \return
   C_NO_ERR  -> data read from INI file (or: nothing to read)
   else      -> error reading data
*/
//-----------------------------------------------------------------------------
sint32 C_CMONProtocolBase::LoadParamsFromIni(C_SCLIniFile & orc_IniFile, const C_SCLString & orc_Section)
{
   (void)orc_IniFile;
   (void)orc_Section;
   return C_NO_ERR; //nothing to load by default
}

//-----------------------------------------------------------------------------
/*!
   \brief    convert 2 bytes to uint16 (interprets data in little endian order)

   \param[in]       oau8_Bytes   data to convert

   \return
   extracted value
*/
//-----------------------------------------------------------------------------
uint16 C_CMONProtocolBase::m_BytesToWordLowHigh(const uint8 oau8_Bytes[2])
{
   return (oau8_Bytes[0] + (static_cast<uint16>((static_cast<uint16>(oau8_Bytes[1])) << 8)));
}

//-----------------------------------------------------------------------------
/*!
   \brief    convert 2 bytes to uint16 (interprets data in big endian order)

   \param[in]       oau8_Bytes   data to convert

   \return
   extracted value
*/
//-----------------------------------------------------------------------------
uint16 C_CMONProtocolBase::m_BytesToWordHighLow(const uint8 oau8_Bytes[2])
{
   return (oau8_Bytes[1] + (static_cast<uint16>(static_cast<uint16>(oau8_Bytes[0]) << 8)));
}

//-----------------------------------------------------------------------------
/*!
   \brief    convert 4 bytes to uint32 (interprets data in little endian order)

   \param[in]       oau8_Bytes   data to convert

   \return
   extracted value
*/
//-----------------------------------------------------------------------------
uint32 C_CMONProtocolBase::m_BytesToDwordLowHigh(const uint8 oau8_Bytes[4])
{
   return static_cast<uint32>(oau8_Bytes[0] +
                              ((static_cast<uint32>(oau8_Bytes[1])) << 8) +
                              ((static_cast<uint32>(oau8_Bytes[2])) << 16) +
                              ((static_cast<uint32>(oau8_Bytes[3])) << 24));
}

//-----------------------------------------------------------------------------
/*!
   \brief    convert 4 bytes to uint32 (interprets data in big endian order)

   \param[in]       oau8_Bytes   data to convert

   \return
   extracted value
*/
//-----------------------------------------------------------------------------
uint32 C_CMONProtocolBase::m_BytesToDwordHighLow(const uint8 oau8_Bytes[4])
{
   return static_cast<uint32>(oau8_Bytes[3] +
                              ((static_cast<uint32>(oau8_Bytes[2])) << 8) +
                              ((static_cast<uint32>(oau8_Bytes[1])) << 16) +
                              ((static_cast<uint32>(oau8_Bytes[0])) << 24));
}

//-----------------------------------------------------------------------------
/*!
   \brief    convert value to string

   Converts passed value to a string.
   If "mq_Decimal" is true the result will be a decimal string representation of the value.
   Otherwise it will be a hex string representation of the value.
   No leading zeros will be inserted.

   \param[in]       ou32_Value   value to convert

   \return
   value in string format
*/
//-----------------------------------------------------------------------------
C_SCLString C_CMONProtocolBase::m_GetValueDecHex(const uint32 ou32_Value) const
{
   C_SCLString c_Text;

   if (mq_Decimal == true)
   {
      (void)c_Text.PrintFormatted("%u", ou32_Value);
   }
   else
   {
      (void)c_Text.PrintFormatted("%X", ou32_Value);
   }
   return c_Text;
}

//-----------------------------------------------------------------------------
/*!
   \brief    convert value to string

   Converts passed value to a string.
   If "mq_Decimal" is true the result will be a decimal string representation of the value.
   Otherwise it will be a hex string representation of the value.

   Decimal values will be filled with leading zeros to have a total length of 5 characters.
   Hex values will be prepended by a blank, then be filled with leading zeros to have a total length of 5 characters.

   \param[in]       ou16_Value   value to convert

   \return
   value in string format
*/
//-----------------------------------------------------------------------------
C_SCLString C_CMONProtocolBase::m_GetWordAsStringFormat(const uint16 ou16_Value) const
{
   C_SCLString c_Help;

   if (mq_Decimal == true)
   {
      (void)c_Help.PrintFormatted("%05d", ou16_Value);
   }
   else
   {
      (void)c_Help.PrintFormatted(" %04X", ou16_Value);
   }
   return c_Help;
}

//-----------------------------------------------------------------------------
/*!
   \brief    convert value to string

   Converts passed value to a string.
   If "mq_Decimal" is true the result will be a decimal string representation of the value.
   Otherwise it will be a hex string representation of the value.

   Decimal values will be filled with leading zeros to have a total length of 3 characters.
   Hex values will be prepended by a blank, then be filled with leading zeros to have a total length of 3 characters.

   \param[in]       ou8_Value   value to convert

   \return
   value in string format
*/
//-----------------------------------------------------------------------------
C_SCLString C_CMONProtocolBase::m_GetByteAsStringFormat(const uint8 ou8_Value) const
{
   C_SCLString c_Help;

   if (mq_Decimal == true)
   {
      (void)c_Help.PrintFormatted("%03d", ou8_Value);
   }
   else
   {
      (void)c_Help.PrintFormatted(" %02X", ou8_Value);
   }
   return c_Help;
}

//---------------------------------------------------------------------------

void C_CMONProtocolBase::SetDecimal(const bool oq_Decimal)
{
   mq_Decimal = oq_Decimal;
}

//---------------------------------------------------------------------------

C_CMONProtocolBase::C_CMONProtocolBase(void) :
   mq_Decimal(false)
{
}

//---------------------------------------------------------------------------

C_CMONProtocolBase::~C_CMONProtocolBase(void)
{
   //nothing to do by default
}

//---------------------------------------------------------------------------

C_CMONProtocolKEFEX_IVA::C_CMONProtocolKEFEX_IVA(void) :
   C_CMONProtocolBase()
{
#ifdef CMONPROTOCOL_ALLOW_RAMVIEW_PROJECT_MAPPING
   mpc_KFXLists = NULL;
#endif
   mu16_KFXListOffset = 0;
}

//---------------------------------------------------------------------------

#ifdef CMONPROTOCOL_ALLOW_RAMVIEW_PROJECT_MAPPING
void C_CMONProtocolKEFEX_IVA::SetVariableInfo(const stw_diag_lib::C_KFXVariableLists * const opc_Lists,
                                              const uint16 ou16_ListOffset)
{
   mpc_KFXLists       = opc_Lists;
   mu16_KFXListOffset = ou16_ListOffset;
}
#endif

//---------------------------------------------------------------------------

C_SCLString C_CMONProtocolKEFEX_IVA::m_KFXTextAndIndexToString(const charn * const opcn_Text,
                                                               const uint16 ou16_Index) const
{
   return (static_cast<C_SCLString>(opcn_Text) + " " + m_KFXIndexToString(ou16_Index));
}

//---------------------------------------------------------------------------

C_SCLString C_CMONProtocolKEFEX_IVA::m_KFXIndexToString(const uint16 ou16_Index, const bool oq_IsKEFEXVarIndex) const
{
   C_SCLString c_Help = "";

#ifdef CMONPROTOCOL_ALLOW_RAMVIEW_PROJECT_MAPPING
   uint16 u16_List;
   uint16 u16_Variable;
   if (oq_IsKEFEXVarIndex == true)
   {
      if (mpc_KFXLists != NULL)
      {
         stw_diag_lib::C_KFXVariableLists::UnpackIndex(ou16_Index, &u16_List, &u16_Variable);
         u16_List -= mu16_KFXListOffset;
         if (mpc_KFXLists->VariableExists(u16_List, u16_Variable) == true)
         {
            c_Help = "VAR " + mpc_KFXLists->operator [](u16_List).c_ListName + "." +
                     mpc_KFXLists->operator [](u16_List).VariableList[u16_Variable].c_Name;
         }
      }
   }
#else
   (void)oq_IsKEFEXVarIndex;
#endif

   if (c_Help == "")
   {
      if (mq_Decimal == true)
      {
         (void)c_Help.PrintFormatted("INDEX %d", ou16_Index);
      }
      else
      {
         (void)c_Help.PrintFormatted("INDEX %X", ou16_Index);
      }
   }
   return c_Help;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Convert serial number array to string

   Support of two serial number formats:
      1: format up to and including 2019. E.g: 05.123456.1001
      2: format from 2020. E.g: 200012345678

   \param[in]       opu8_SerialNumber     Pointer to first of six serial number array elements

   \return
   serial number string
*/
//----------------------------------------------------------------------------------------------------------------------
C_SCLString C_CMONProtocolBase::mh_SerialNumberToString(const stw_types::uint8 * const opu8_SerialNumber)
{
   C_SCLString c_Result;

   if (opu8_SerialNumber != NULL)
   {
      if (opu8_SerialNumber[0] < static_cast<uint8>(0x20))
      {
         //format up to and including 2019. E.g: 05.123456.1001
         c_Result.PrintFormatted("%02X.%02X%02X%02X.%02X%02X",
                                 opu8_SerialNumber[0], opu8_SerialNumber[1], opu8_SerialNumber[2], opu8_SerialNumber[3],
                                 opu8_SerialNumber[4], opu8_SerialNumber[5]);
      }
      else
      {
         //format from 2020. E.g: 200012345678
         c_Result.PrintFormatted("%02X%02X%02X%02X%02X%02X",
                                 opu8_SerialNumber[0], opu8_SerialNumber[1], opu8_SerialNumber[2], opu8_SerialNumber[3],
                                 opu8_SerialNumber[4], opu8_SerialNumber[5]);
      }
   }

   return c_Result;
}

//---------------------------------------------------------------------------
