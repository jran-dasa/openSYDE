//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Model for message generator signals table (implementation)

   Model for message generator signals table

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include <limits>

#include "stwtypes.h"
#include "TGLUtils.h"
#include "stwerrors.h"
#include "constants.h"
#include "C_GtGetText.h"
#include "C_Uti.h"
#include "C_CamDbHandler.h"
#include "C_SdTooltipUtil.h"
#include "C_CamGenSigUtil.h"
#include "C_CamProHandler.h"
#include "C_OSCLoggingHandler.h"
#include "C_CamGenSigTableModel.h"
#include "C_SdNdeDpContentUtil.h"
#include "C_OSCNodeDataPoolContentUtil.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_tgl;
using namespace stw_types;
using namespace stw_errors;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_core;
using namespace stw_opensyde_gui_logic;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Default constructor

   \param[in,out]  opc_Parent    Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_CamGenSigTableModel::C_CamGenSigTableModel(QObject * const opc_Parent) :
   QAbstractTableModel(opc_Parent),
   mu32_MessageIndex(0UL)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Handle signal reload on external trigger
*/
//----------------------------------------------------------------------------------------------------------------------
void C_CamGenSigTableModel::TriggerSignalReload(void)
{
   //Trigger find message!
   this->SetMessage(this->mu32_MessageIndex);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Set current message index

   \param[in]  ou32_Message   Message index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_CamGenSigTableModel::SetMessage(const uint32 ou32_Message)
{
   this->beginResetModel();
   this->mu32_MessageIndex = ou32_Message;
   const C_CamProMessageData * const pc_Message = C_CamProHandler::h_GetInstance()->GetMessageConst(
      this->mu32_MessageIndex);
   if (pc_Message != NULL)
   {
      //Find message: should not hurt to search both DBC and OSY as only one should know the file
      C_CamDbHandler::h_GetInstance()->FindDbcMessage(pc_Message->c_DataBaseFilePath.c_str(),
                                                      pc_Message->c_Name.c_str());
      C_CamDbHandler::h_GetInstance()->FindOsyMessage(pc_Message->c_DataBaseFilePath.c_str(),
                                                      pc_Message->c_Name.c_str());
   }
   this->endResetModel();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Update the message DLC

   \param[in]  ou32_MessageIndex    Message index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_CamGenSigTableModel::UpdateMessageDLC(const uint32 ou32_MessageIndex)
{
   if (ou32_MessageIndex == this->mu32_MessageIndex)
   {
      this->beginResetModel();
      this->endResetModel();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get header data

   \param[in]  osn_Section       Section
   \param[in]  oe_Orientation    Orientation
   \param[in]  osn_Role          Role

   \return
   Header string
*/
//----------------------------------------------------------------------------------------------------------------------
QVariant C_CamGenSigTableModel::headerData(const sintn osn_Section, const Qt::Orientation oe_Orientation,
                                           const sintn osn_Role) const
{
   QVariant c_Retval = QAbstractTableModel::headerData(osn_Section, oe_Orientation, osn_Role);

   if (oe_Orientation == Qt::Orientation::Horizontal)
   {
      const C_CamGenSigTableModel::E_Columns e_Col = h_ColumnToEnum(osn_Section);
      if (osn_Role == static_cast<sintn>(Qt::DisplayRole))
      {
         switch (e_Col)
         {
         case eNAME:
            c_Retval = C_GtGetText::h_GetText("Name");
            break;
         case eBIT_POS:
            c_Retval = C_GtGetText::h_GetText("SB");
            break;
         case eRAW:
            c_Retval = C_GtGetText::h_GetText("RAW Value");
            break;
         case ePHYSICAL:
            c_Retval = C_GtGetText::h_GetText("Phys Value");
            break;
         case eUNIT:
            c_Retval = C_GtGetText::h_GetText("Unit");
            break;
         default:
            tgl_assert(false);
            break;
         }
      }
      else if (osn_Role == msn_USER_ROLE_TOOL_TIP_HEADING)
      {
         switch (e_Col)
         {
         case eNAME:
            c_Retval = C_GtGetText::h_GetText("Name");
            break;
         case eBIT_POS:
            c_Retval = C_GtGetText::h_GetText("Start Bit");
            break;
         case eRAW:
            c_Retval = C_GtGetText::h_GetText("Raw Value");
            break;
         case ePHYSICAL:
            c_Retval = C_GtGetText::h_GetText("Physical Value");
            break;
         case eUNIT:
            c_Retval = C_GtGetText::h_GetText("Unit");
            break;
         default:
            tgl_assert(false);
            break;
         }
      }
      else if (osn_Role == static_cast<sintn>(Qt::TextAlignmentRole))
      {
         c_Retval = static_cast<QVariant>(Qt::AlignLeft | Qt::AlignVCenter);
      }
      else if (osn_Role == msn_USER_ROLE_TOOL_TIP_CONTENT)
      {
         switch (e_Col)
         {
         case eNAME:
            c_Retval = C_GtGetText::h_GetText("CAN message signal name");
            break;
         case eBIT_POS:
            c_Retval = C_GtGetText::h_GetText("CAN message signal start bit position");
            break;
         case eRAW:
            c_Retval = C_GtGetText::h_GetText("CAN message signal raw value (= as seen on CAN)");
            break;
         case ePHYSICAL:
            c_Retval = C_GtGetText::h_GetText("CAN message signal physical value (=interpreted/scaled value)");
            break;
         case eUNIT:
            c_Retval = C_GtGetText::h_GetText("CAN message signal unit  (=interpreted/scaled value unit)");
            break;
         default:
            tgl_assert(false);
            break;
         }
      }
      else
      {
         //No data in default case
      }
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get table column count

   \param[in]  orc_Parent  Parent

   \return
   Column count
*/
//----------------------------------------------------------------------------------------------------------------------
sintn C_CamGenSigTableModel::columnCount(const QModelIndex & orc_Parent) const
{
   stw_types::sintn sn_Retval = 0;
   if (!orc_Parent.isValid())
   {
      //For table parent should always be invalid
      if (this->m_CheckInterpretedMode())
      {
         sn_Retval = 5;
      }
      else
      {
         sn_Retval = 3;
      }
   }
   return sn_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get table row count

   Compare with file header description.

   \param[in]  orc_Parent  Parent

   \return
   Row count
*/
//----------------------------------------------------------------------------------------------------------------------
sintn C_CamGenSigTableModel::rowCount(const QModelIndex & orc_Parent) const
{
   sintn sn_Retval;

   if (orc_Parent.isValid() == false)
   {
      if (this->m_CheckInterpretedMode())
      {
         const C_OSCCanMessage * const pc_OsyMessage = m_GetMessageInterpretedOsy();
         const C_CieConverter::C_CIECanMessage * const pc_DbcMessage = m_GetMessageInterpretedDbc();
         if ((pc_OsyMessage != NULL) || (pc_DbcMessage != NULL))
         {
            const bool q_IsMultiplexed = C_CamGenSigTableModel::mh_IsMultiplexed(pc_OsyMessage, pc_DbcMessage);
            if (q_IsMultiplexed)
            {
               const uint16 u16_MuxValue = C_CamGenSigTableModel::m_GetMultiplexerValue(pc_OsyMessage, pc_DbcMessage);
               sn_Retval = C_CamGenSigTableModel::mh_GetNumRowsForMuxValue(pc_OsyMessage, pc_DbcMessage, u16_MuxValue);
            }
            else
            {
               if (pc_OsyMessage != NULL)
               {
                  sn_Retval = pc_OsyMessage->c_Signals.size();
               }
               else if (pc_DbcMessage != NULL)
               {
                  sn_Retval = pc_DbcMessage->c_Signals.size();
               }
               else
               {
                  // should not occur
                  sn_Retval = 0;
               }
            }
         }
         else
         {
            //Default
            sn_Retval = 0;
         }
      }
      else
      {
         const C_CamProMessageData * const pc_Message = C_CamProHandler::h_GetInstance()->GetMessageConst(
            this->mu32_MessageIndex);
         if (pc_Message != NULL)
         {
            sn_Retval = static_cast<sintn>(pc_Message->u16_Dlc);
         }
         else
         {
            sn_Retval = 0;
         }
      }
   }
   else
   {
      // Qt documentation: When implementing a table based model, rowCount() should return 0 when the parent is valid.
      sn_Retval = 0;
   }
   return sn_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get data at index

   \param[in]  orc_Index   Index
   \param[in]  osn_Role    Data role

   \return
   Data
*/
//----------------------------------------------------------------------------------------------------------------------
QVariant C_CamGenSigTableModel::data(const QModelIndex & orc_Index, const sintn osn_Role) const
{
   QVariant c_Retval;

   if ((orc_Index.isValid() == true) && (orc_Index.row() >= 0))
   {
      const C_CamGenSigTableModel::E_Columns e_Col = h_ColumnToEnum(orc_Index.column());
      if (osn_Role == static_cast<sintn>(Qt::ForegroundRole))
      {
         switch (e_Col) //lint !e788  //not all constants explicitly handled on purpose
         {
         case eRAW:
         case ePHYSICAL:
            c_Retval = mc_STYLE_GUIDE_COLOR_1;
            break;
         default:
            c_Retval = mc_STYLE_GUIDE_COLOR_8;
            break;
         }
      }
      else
      {
         const uint32 u32_Index = this->m_TranslateRowToIndex(orc_Index.row());
         if (this->m_CheckInterpretedMode())
         {
            if (e_Col == eRAW)
            {
               c_Retval = m_HandleColRawInterpreted(u32_Index, osn_Role);
            }
            else if (e_Col == ePHYSICAL)
            {
               c_Retval = m_HandleColPhysicalInterpreted(u32_Index, osn_Role);
            }
            else
            {
               if ((osn_Role == static_cast<sintn>(Qt::DisplayRole)) || (osn_Role == static_cast<sintn>(Qt::EditRole)))
               {
                  const C_OSCCanSignal * pc_OsySignal;
                  const C_OSCNodeDataPoolListElement * pc_OsySignalCommon;
                  switch (e_Col)
                  {
                  case eNAME:
                     pc_OsySignalCommon = m_GetSignalInterpretedOsyCommon(u32_Index);
                     pc_OsySignal = m_GetSignalInterpretedOsy(u32_Index);
                     if ((pc_OsySignalCommon != NULL) && (pc_OsySignal != NULL))
                     {
                        QString c_Name = pc_OsySignalCommon->c_Name.c_str();
                        if (pc_OsySignal->e_MultiplexerType == C_OSCCanSignal::eMUX_MULTIPLEXER_SIGNAL)
                        {
                           c_Name = static_cast<QString>("%1 (Multiplexer)").arg(c_Name);
                        }
                        c_Retval = c_Name;
                     }
                     else
                     {
                        const C_CieConverter::C_CIECanSignal * const pc_DbcSignal =
                           m_GetSignalInterpretedDbc(u32_Index);
                        if (pc_DbcSignal != NULL)
                        {
                           QString c_Name = pc_DbcSignal->c_Element.c_Name.c_str();
                           if (pc_DbcSignal->e_MultiplexerType == C_OSCCanSignal::eMUX_MULTIPLEXER_SIGNAL)
                           {
                              c_Name = static_cast<QString>("%1 (Multiplexer)").arg(c_Name);
                           }
                           c_Retval = c_Name;
                        }
                        else
                        {
                           //Default
                        }
                     }
                     break;
                  case eBIT_POS:
                     pc_OsySignal = m_GetSignalInterpretedOsy(u32_Index);
                     if (pc_OsySignal != NULL)
                     {
                        c_Retval = pc_OsySignal->u16_ComBitStart;
                     }
                     else
                     {
                        const C_CieConverter::C_CIECanSignal * const pc_DbcSignal =
                           m_GetSignalInterpretedDbc(u32_Index);
                        if (pc_DbcSignal != NULL)
                        {
                           c_Retval = pc_DbcSignal->u16_ComBitStart;
                        }
                        else
                        {
                           //Default
                        }
                     }
                     break;
                  case eRAW:
                     //handled elsewhere
                     tgl_assert(false);
                     break;
                  case ePHYSICAL:
                     //handled elsewhere
                     tgl_assert(false);
                     break;
                  case eUNIT:
                     pc_OsySignalCommon = m_GetSignalInterpretedOsyCommon(u32_Index);
                     if (pc_OsySignalCommon != NULL)
                     {
                        c_Retval = pc_OsySignalCommon->c_Unit.c_str();
                     }
                     else
                     {
                        const C_CieConverter::C_CIECanSignal * const pc_DbcSignal =
                           m_GetSignalInterpretedDbc(u32_Index);
                        if (pc_DbcSignal != NULL)
                        {
                           c_Retval = pc_DbcSignal->c_Element.c_Unit.c_str();
                        }
                        else
                        {
                           //Default
                        }
                     }
                     break;
                  default:
                     tgl_assert(false);
                     break;
                  }
               }
               else if (osn_Role == msn_USER_ROLE_TOOL_TIP_HEADING)
               {
                  if (e_Col == eNAME)
                  {
                     //Return signal name
                     c_Retval = this->data(orc_Index, static_cast<sintn>(Qt::DisplayRole));
                  }
                  else
                  {
                     //Default: no tool tip
                  }
               }
               else if (osn_Role == msn_USER_ROLE_TOOL_TIP_CONTENT)
               {
                  if (e_Col == eNAME)
                  {
                     C_OSCCanSignal c_OsySignal;
                     C_OSCCanMessage c_OsyMessage;
                     C_OSCNodeDataPoolListElement c_OsySignalCommon;
                     const C_OSCCanSignal * pc_OsySignal;
                     const C_OSCCanMessage * pc_OsyMessage;
                     const C_OSCNodeDataPoolListElement * pc_OsySignalCommon;

                     //Signal
                     pc_OsySignal = m_GetSignalInterpretedOsy(u32_Index);
                     if (pc_OsySignal != NULL)
                     {
                        c_OsySignal = *pc_OsySignal;
                     }
                     else
                     {
                        const C_CieConverter::C_CIECanSignal * const pc_DbcSignal =
                           m_GetSignalInterpretedDbc(u32_Index);
                        if (pc_DbcSignal != NULL)
                        {
                           c_OsySignal = C_CamGenSigUtil::h_ConvertDBCToOSY(*pc_DbcSignal);
                        }
                     }
                     //Signal common
                     pc_OsySignalCommon = m_GetSignalInterpretedOsyCommon(u32_Index);
                     if (pc_OsySignalCommon != NULL)
                     {
                        c_OsySignalCommon = *pc_OsySignalCommon;
                     }
                     else
                     {
                        const C_CieConverter::C_CIECanSignal * const pc_DbcSignal =
                           m_GetSignalInterpretedDbc(u32_Index);
                        if (pc_DbcSignal != NULL)
                        {
                           c_OsySignalCommon = C_CamGenSigUtil::h_ConvertDBCToOSY(pc_DbcSignal->c_Element);
                        }
                     }
                     //Message
                     pc_OsyMessage = m_GetMessageInterpretedOsy();
                     if (pc_OsyMessage != NULL)
                     {
                        c_OsyMessage = *pc_OsyMessage;
                     }
                     else
                     {
                        const C_CieConverter::C_CIECanMessage * const pc_DbcMessage = m_GetMessageInterpretedDbc();
                        if (pc_DbcMessage != NULL)
                        {
                           c_OsyMessage = C_CamGenSigUtil::h_ConvertDBCToOSY(*pc_DbcMessage);
                        }
                     }
                     //Tool tip
                     c_Retval = C_SdTooltipUtil::h_GetToolTipContentSignal(c_OsySignal, c_OsyMessage, c_OsySignalCommon,
                                                                           "", "");
                  }
                  else
                  {
                     //Default: no tool tip
                  }
               }
               else
               {
                  //Default
               }
            }
         }
         else
         {
            if ((osn_Role == static_cast<sintn>(Qt::DisplayRole)) || (osn_Role == static_cast<sintn>(Qt::EditRole)))
            {
               const C_CamProMessageData * pc_Message;
               switch (e_Col) //lint !e788  //not all constants explicitly handled on purpose
               {
               case eNAME:
                  c_Retval = static_cast<QString>("Byte %1").arg(u32_Index);
                  break;
               case eBIT_POS:
                  c_Retval = static_cast<uint64>(u32_Index) * 8ULL;
                  break;
               case eRAW:
                  pc_Message = C_CamProHandler::h_GetInstance()->GetMessageConst(this->mu32_MessageIndex);
                  if ((pc_Message != NULL) && (u32_Index < 8UL))
                  {
                     if (osn_Role == static_cast<sintn>(Qt::DisplayRole))
                     {
                        c_Retval = C_Uti::h_GetValueAsHex(static_cast<uint64>(pc_Message->c_Bytes[u32_Index]), 2);
                     }
                     else
                     {
                        c_Retval = static_cast<uint64>(pc_Message->c_Bytes[u32_Index]);
                     }
                  }
                  break;
               default:
                  //Not necessary for not interpreted mode
                  break;
               }
            }
            else if (osn_Role == msn_USER_ROLE_INTERACTION_ELEMENT_TYPE)
            {
               if (e_Col == eRAW)
               {
                  c_Retval = static_cast<sintn>(eURIEL_LINE_EDIT);
               }
               else
               {
                  c_Retval = static_cast<sintn>(eURIEL_NONE);
               }
            }
            else if (osn_Role == msn_USER_ROLE_INTERACTION_USE_MIN_VALUE)
            {
               if (e_Col == eRAW)
               {
                  c_Retval = true;
               }
               else
               {
                  c_Retval = false;
               }
            }
            else if (osn_Role == msn_USER_ROLE_INTERACTION_MINIMUM_VALUE)
            {
               if (e_Col == eRAW)
               {
                  c_Retval = static_cast<QString>("0x00");
               }
               else
               {
                  //None
               }
            }
            else if (osn_Role == msn_USER_ROLE_INTERACTION_USE_MAX_VALUE)
            {
               if (e_Col == eRAW)
               {
                  c_Retval = true;
               }
               else
               {
                  c_Retval = false;
               }
            }
            else if (osn_Role == msn_USER_ROLE_INTERACTION_MAXIMUM_VALUE)
            {
               if (e_Col == eRAW)
               {
                  c_Retval = static_cast<QString>("0xFF");
               }
               else
               {
                  //None
               }
            }
            else
            {
               //Use default
            }
         }
      }
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Set data at index

   \param[in]  orc_Index   Index
   \param[in]  orc_Value   New data
   \param[in]  osn_Role    Data role

   \return
   true  success
   false failure
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_CamGenSigTableModel::setData(const QModelIndex & orc_Index, const QVariant & orc_Value, const sintn osn_Role)
{
   bool q_Retval = false;

   if (data(orc_Index, osn_Role) != orc_Value)
   {
      if ((orc_Index.isValid() == true) && (orc_Index.row() >= 0))
      {
         const uint32 u32_Index = this->m_TranslateRowToIndex(orc_Index.row());
         const C_CamGenSigTableModel::E_Columns e_Col = h_ColumnToEnum(orc_Index.column());
         //Deactivate message as it is sent now
         Q_EMIT (this->SigTriggerModelUpdateCyclicMessage(this->mu32_MessageIndex, false));
         if (this->m_CheckInterpretedMode())
         {
            const C_OSCNodeDataPoolListElement * const pc_OsySignalCommon =
               m_GetSignalInterpretedOsyCommon(u32_Index);
            C_OSCCanSignal c_Signal;
            const C_CieConverter::C_CIECanSignal * const pc_DbcSignal = m_GetSignalInterpretedDbc(u32_Index);
            //Init appropriate type
            C_OSCNodeDataPoolContent c_Value = mh_GetInitialValue(pc_OsySignalCommon, pc_DbcSignal);
            //Handle check box
            if (this->flags(orc_Index).testFlag(Qt::ItemIsUserCheckable))
            {
               bool q_Val;
               const Qt::CheckState e_CheckedState = static_cast<Qt::CheckState>(orc_Value.toInt());
               if (e_CheckedState == Qt::Checked)
               {
                  q_Val = true;
               }
               else
               {
                  q_Val = false;
               }
               C_CamGenSigTableModel::mh_SetBoolInContent(c_Value, q_Val);
            }
            else
            {
               //Handle generic spin box
               float64 f64_Factor;
               float64 f64_Offset;
               //Handle current scaling values
               if (e_Col == C_CamGenSigTableModel::ePHYSICAL)
               {
                  if (pc_OsySignalCommon != NULL)
                  {
                     f64_Factor = pc_OsySignalCommon->f64_Factor;
                     f64_Offset = pc_OsySignalCommon->f64_Offset;
                  }
                  else if (pc_DbcSignal != NULL)
                  {
                     //Value table needs raw value
                     if (pc_DbcSignal->c_ValueDescription.size() > 0UL)
                     {
                        f64_Factor = 1.0;
                        f64_Offset = 0.0;
                     }
                     else
                     {
                        f64_Factor = pc_DbcSignal->c_Element.f64_Factor;
                        f64_Offset = pc_DbcSignal->c_Element.f64_Offset;
                     }
                  }
                  else
                  {
                     f64_Factor = 1.0;
                     f64_Offset = 0.0;
                  }
               }
               else
               {
                  f64_Factor = 1.0;
                  f64_Offset = 0.0;
               }
               //Convert to content (unscaled)
               C_SdNdeDpContentUtil::h_SetDataVariableFromGenericWithScaling(orc_Value, c_Value, f64_Factor,
                                                                             f64_Offset, 0UL);
            }

            //Convert to one format
            if (pc_DbcSignal != NULL)
            {
               c_Signal = C_CamGenSigUtil::h_ConvertDBCToOSY(*pc_DbcSignal);
            }
            else
            {
               const C_OSCCanSignal * const pc_OsySignal = m_GetSignalInterpretedOsy(u32_Index);
               if (pc_OsySignal != NULL)
               {
                  c_Signal = *pc_OsySignal;
               }
            }
            if (c_Signal.e_MultiplexerType == C_OSCCanSignal::eMUX_MULTIPLEXER_SIGNAL)
            {
               this->beginResetModel();
            }
            //Handle encoding
            if (this->m_SetSignalFromOsyValue(c_Signal, c_Value) == C_NO_ERR)
            {
               q_Retval = true;
               if (c_Signal.e_MultiplexerType != C_OSCCanSignal::eMUX_MULTIPLEXER_SIGNAL)
               {
                  //Signal the other column raw->notify physical & vice versa
                  if (e_Col == C_CamGenSigTableModel::ePHYSICAL)
                  {
                     const QModelIndex c_Index =
                        this->index(orc_Index.row(),
                                    C_CamGenSigTableModel::h_EnumToColumn(C_CamGenSigTableModel::eRAW));
                     Q_EMIT (this->dataChanged(c_Index, c_Index, QVector<stw_types::sintn>() << osn_Role));
                  }
                  else
                  {
                     const QModelIndex c_Index =
                        this->index(orc_Index.row(),
                                    C_CamGenSigTableModel::h_EnumToColumn(C_CamGenSigTableModel::ePHYSICAL));
                     Q_EMIT (this->dataChanged(c_Index, c_Index, QVector<stw_types::sintn>() << osn_Role));
                  }
               }
            }
            else
            {
               tgl_assert(false);
            }
            if (c_Signal.e_MultiplexerType == C_OSCCanSignal::eMUX_MULTIPLEXER_SIGNAL)
            {
               // reset all now unused signals
               this->m_ResetUnusedMultiplexedSignals();
               this->endResetModel();
               Q_EMIT (this->SigResetPermanentEditors());
            }
         }
         else
         {
            if (e_Col == C_CamGenSigTableModel::eRAW)
            {
               bool q_Ok;
               C_CamProMessageData::E_GenericUint32DataSelector e_Selector;
               switch (u32_Index)
               {
               case 0UL:
                  e_Selector = C_CamProMessageData::eGUIDS_DB0;
                  break;
               case 1UL:
                  e_Selector = C_CamProMessageData::eGUIDS_DB1;
                  break;
               case 2UL:
                  e_Selector = C_CamProMessageData::eGUIDS_DB2;
                  break;
               case 3UL:
                  e_Selector = C_CamProMessageData::eGUIDS_DB3;
                  break;
               case 4UL:
                  e_Selector = C_CamProMessageData::eGUIDS_DB4;
                  break;
               case 5UL:
                  e_Selector = C_CamProMessageData::eGUIDS_DB5;
                  break;
               case 6UL:
                  e_Selector = C_CamProMessageData::eGUIDS_DB6;
                  break;
               case 7UL:
                  e_Selector = C_CamProMessageData::eGUIDS_DB7;
                  break;
               default:
                  e_Selector = C_CamProMessageData::eGUIDS_DB0;
                  break;
               }
               if (C_CamProHandler::h_GetInstance()->
                   SetMessageUint32Value(this->mu32_MessageIndex, e_Selector,
                                         static_cast<uint32>(orc_Value.toULongLong(&q_Ok))) == C_NO_ERR)
               {
                  if (q_Ok == true)
                  {
                     q_Retval = true;
                  }
               }
            }
         }
         if (q_Retval == true)
         {
            //lint -e{1793} Qt example
            Q_EMIT (this->dataChanged(orc_Index, orc_Index, QVector<stw_types::sintn>() << osn_Role));
            Q_EMIT (this->SigUpdateMessageData(this->mu32_MessageIndex));
         }
         //Trigger sending of updated message
         Q_EMIT (this->SigTriggerModelUpdateCyclicMessage(this->mu32_MessageIndex, true));
      }
   }
   else
   {
      //Even though the data is the same the display format might have to change
      //so its better to indicate a data change anyways
      //lint -e{1793} Qt example
      Q_EMIT (this->dataChanged(orc_Index, orc_Index, QVector<stw_types::sintn>() << osn_Role));
   }
   return q_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get flags for item

   \param[in]  orc_Index   Item

   \return
   Flags for item
*/
//----------------------------------------------------------------------------------------------------------------------
Qt::ItemFlags C_CamGenSigTableModel::flags(const QModelIndex & orc_Index) const
{
   Qt::ItemFlags c_Retval = Qt::NoItemFlags;
   if (orc_Index.isValid() == true)
   {
      const C_CamGenSigTableModel::E_Columns e_Col = h_ColumnToEnum(orc_Index.column());
      switch (e_Col) //lint !e788  //not all columns handled explicitly
      {
      case eRAW:
         c_Retval = c_Retval | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
         //Handle checkable flag
         if (orc_Index.row() >= 0)
         {
            const uint32 u32_Index = this->m_TranslateRowToIndex(orc_Index.row());
            const C_OSCNodeDataPoolListElement * const pc_OsySignalCommon = m_GetSignalInterpretedOsyCommon(u32_Index);
            const C_OSCCanSignal * const pc_OsySignal = m_GetSignalInterpretedOsy(u32_Index);
            if ((pc_OsySignal != NULL) && (pc_OsySignalCommon != NULL))
            {
               const C_OSCNodeDataPoolContent c_Min = C_CamGenSigTableModel::mh_GetBorderValue(
                  pc_OsySignalCommon->c_MinValue,
                  pc_OsySignal->u16_ComBitLength,
                  true);
               const C_OSCNodeDataPoolContent c_Max = C_CamGenSigTableModel::mh_GetBorderValue(
                  pc_OsySignalCommon->c_MinValue,
                  pc_OsySignal->u16_ComBitLength,
                  false);
               if (m_CheckUseCheckBox(c_Min, c_Max))
               {
                  c_Retval = c_Retval | Qt::ItemIsUserCheckable;
               }
            }
            else
            {
               const C_CieConverter::C_CIECanSignal * const pc_DbcSignal = m_GetSignalInterpretedDbc(u32_Index);
               if (pc_DbcSignal != NULL)
               {
                  const C_OSCNodeDataPoolContent c_Min = C_CamGenSigTableModel::mh_GetBorderValue(
                     pc_DbcSignal->c_Element.c_MinValue, pc_DbcSignal->u16_ComBitLength, true);
                  const C_OSCNodeDataPoolContent c_Max = C_CamGenSigTableModel::mh_GetBorderValue(
                     pc_DbcSignal->c_Element.c_MinValue, pc_DbcSignal->u16_ComBitLength, false);
                  if (m_CheckUseCheckBox(c_Min, c_Max))
                  {
                     c_Retval = c_Retval | Qt::ItemIsUserCheckable;
                  }
               }
            }
         }
         break;
      case ePHYSICAL:
         c_Retval = c_Retval | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
         break;
      default:
         //No additional flags
         break;
      }
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Column to enum conversion

   \param[in]  os32_Column    Column

   \return
   Enum value
*/
//----------------------------------------------------------------------------------------------------------------------
C_CamGenSigTableModel::E_Columns C_CamGenSigTableModel::h_ColumnToEnum(const sint32 os32_Column)
{
   C_CamGenSigTableModel::E_Columns e_Retval;
   switch (os32_Column)
   {
   case 0:
      e_Retval = eBIT_POS;
      break;
   case 1:
      e_Retval = eNAME;
      break;
   case 2:
      e_Retval = eRAW;
      break;
   case 3:
      e_Retval = ePHYSICAL;
      break;
   case 4:
      e_Retval = eUNIT;
      break;
   default:
      e_Retval = eBIT_POS;
      break;
   }
   return e_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Enum to column conversion

   \param[in]  oe_Value    Enum value

   \return
   Column
   -1 Error
*/
//----------------------------------------------------------------------------------------------------------------------
sint32 C_CamGenSigTableModel::h_EnumToColumn(const C_CamGenSigTableModel::E_Columns oe_Value)
{
   sint32 s32_Retval = -1;

   switch (oe_Value)
   {
   case eBIT_POS:
      s32_Retval = 0;
      break;
   case eNAME:
      s32_Retval = 1;
      break;
   case eRAW:
      s32_Retval = 2;
      break;
   case ePHYSICAL:
      s32_Retval = 3;
      break;
   case eUNIT:
      s32_Retval = 4;
      break;
   default:
      break;
   }
   return s32_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Check if message in interpreted display mode

   \return
   True  Interpreted display mode
   False Raw display mode
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_CamGenSigTableModel::m_CheckInterpretedMode(void) const
{
   bool q_Retval = false;
   const C_CamProMessageData * const pc_Message = C_CamProHandler::h_GetInstance()->GetMessageConst(
      this->mu32_MessageIndex);

   if (pc_Message != NULL)
   {
      if (pc_Message->c_DataBaseFilePath.IsEmpty() == true)
      {
         q_Retval = false;
      }
      else
      {
         q_Retval = true;
      }
   }

   return q_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get  interpreted OSY message

   \return
   Interpreted OSY message
*/
//----------------------------------------------------------------------------------------------------------------------
const stw_opensyde_core::C_OSCCanMessage * C_CamGenSigTableModel::m_GetMessageInterpretedOsy(void) const
{
   const stw_opensyde_core::C_OSCCanMessage * pc_Retval = NULL;
   const C_CamProMessageData * const pc_Message = C_CamProHandler::h_GetInstance()->GetMessageConst(
      this->mu32_MessageIndex);

   if (pc_Message != NULL)
   {
      pc_Retval = C_CamDbHandler::h_GetInstance()->GetOSCMessage(pc_Message->c_DataBaseFilePath.c_str(),
                                                                 pc_Message->c_Name.c_str(),
                                                                 pc_Message->q_ContainsValidHash,
                                                                 pc_Message->u32_Hash);
   }
   return pc_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get interpreted OSY signal

   \param[in]  ou32_Index  Signal index

   \return
   Interpreted OSY signal
*/
//----------------------------------------------------------------------------------------------------------------------
const stw_opensyde_core::C_OSCCanSignal * C_CamGenSigTableModel::m_GetSignalInterpretedOsy(const uint32 ou32_Index)
const
{
   const C_OSCCanSignal * pc_Retval = NULL;
   const C_OSCCanMessage * const pc_Message = m_GetMessageInterpretedOsy();

   if ((pc_Message != NULL) && (ou32_Index < pc_Message->c_Signals.size()))
   {
      pc_Retval = &pc_Message->c_Signals[ou32_Index];
   }

   return pc_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get list for interpreted OSY list

   \return
   List for interpreted OSY list
*/
//----------------------------------------------------------------------------------------------------------------------
const stw_opensyde_core::C_OSCNodeDataPoolList * C_CamGenSigTableModel::m_GetMessageListInterpreted(void) const
{
   const stw_opensyde_core::C_OSCNodeDataPoolList * pc_Retval = NULL;
   const C_CamProMessageData * const pc_Message = C_CamProHandler::h_GetInstance()->GetMessageConst(
      this->mu32_MessageIndex);

   if (pc_Message != NULL)
   {
      pc_Retval = C_CamDbHandler::h_GetInstance()->GetOSCList(pc_Message->c_DataBaseFilePath.c_str(),
                                                              pc_Message->c_Name.c_str(),
                                                              pc_Message->q_ContainsValidHash,
                                                              pc_Message->u32_Hash);
   }
   return pc_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get interpreted OSY signal common part

   \param[in]  ou32_Index  Signal index

   \return
   Interpreted OSY signal common part
*/
//----------------------------------------------------------------------------------------------------------------------
const C_OSCNodeDataPoolListElement * C_CamGenSigTableModel::m_GetSignalInterpretedOsyCommon(const uint32 ou32_Index)
const
{
   const C_OSCNodeDataPoolListElement * pc_Retval = NULL;
   const stw_opensyde_core::C_OSCCanSignal * const pc_Signal = m_GetSignalInterpretedOsy(ou32_Index);
   const stw_opensyde_core::C_OSCNodeDataPoolList * const pc_List = m_GetMessageListInterpreted();

   if (((pc_Signal != NULL) && (pc_List != NULL)) && (pc_Signal->u32_ComDataElementIndex < pc_List->c_Elements.size()))
   {
      pc_Retval = &pc_List->c_Elements[pc_Signal->u32_ComDataElementIndex];
   }

   return pc_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get interpreted DBC message

   \return
   Interpreted DBC message
*/
//----------------------------------------------------------------------------------------------------------------------
const C_CieConverter::C_CIECanMessage * C_CamGenSigTableModel::m_GetMessageInterpretedDbc(void) const
{
   const C_CieConverter::C_CIECanMessage * pc_Retval = NULL;
   const C_CamProMessageData * const pc_Message = C_CamProHandler::h_GetInstance()->GetMessageConst(
      this->mu32_MessageIndex);

   if (pc_Message != NULL)
   {
      pc_Retval = C_CamDbHandler::h_GetInstance()->GetDbcMessage(pc_Message->c_DataBaseFilePath.c_str(),
                                                                 pc_Message->c_Name.c_str(),
                                                                 pc_Message->q_ContainsValidHash,
                                                                 pc_Message->u32_Hash);
   }
   return pc_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get interpreted DBC signal

   \param[in]  ou32_Index  Signal index

   \return
   Interpreted DBC signal
*/
//----------------------------------------------------------------------------------------------------------------------
const C_CieConverter::C_CIECanSignal * C_CamGenSigTableModel::m_GetSignalInterpretedDbc(const uint32 ou32_Index) const
{
   const C_CieConverter::C_CIECanSignal * pc_Retval = NULL;
   const C_CieConverter::C_CIECanMessage * const pc_Message = m_GetMessageInterpretedDbc();

   if ((pc_Message != NULL) && (ou32_Index < pc_Message->c_Signals.size()))
   {
      pc_Retval = &pc_Message->c_Signals[ou32_Index];
   }

   return pc_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Decode raw value into C_OSCNodeDataPoolContent

   Based on: DBC

   \param[in]  orc_Raw     Raw value
   \param[in]  orc_Signal  Signal description

   \return
   Value as C_OSCNodeDataPoolContent
*/
//----------------------------------------------------------------------------------------------------------------------
C_OSCNodeDataPoolContent C_CamGenSigTableModel::mh_DecodeRawToContentDbc(const std::vector<uint8> & orc_Raw,
                                                                         const C_CieConverter::C_CIECanSignal & orc_Signal)
{
   const C_OSCCanSignal c_Signal = C_CamGenSigUtil::h_ConvertDBCToOSY(orc_Signal);
   C_OSCNodeDataPoolContent c_Retval;

   //Use openSYDE decoding
   c_Retval = C_CamGenSigUtil::h_DecodeRawToContentSignal(orc_Raw, c_Signal, orc_Signal.c_Element.c_MinValue);
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Check if a checkbox is possible for the raw value

   \param[in]  orc_Min  Minimum value
   \param[in]  orc_Max  Maximum value

   \return
   True  Use check box
   False Use other edit method
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_CamGenSigTableModel::m_CheckUseCheckBox(const C_OSCNodeDataPoolContent & orc_Min,
                                               const C_OSCNodeDataPoolContent & orc_Max) const
{
   bool q_Retval = false;

   //Only for non float types
   if (((orc_Min.GetType() != C_OSCNodeDataPoolContent::eFLOAT32) &&
        (orc_Min.GetType() != C_OSCNodeDataPoolContent::eFLOAT64)) &&
       ((orc_Max.GetType() != C_OSCNodeDataPoolContent::eFLOAT32) &&
        (orc_Max.GetType() != C_OSCNodeDataPoolContent::eFLOAT64)))
   {
      //Check exact min and max values
      uint64 u64_Min;
      uint64 u64_Max;
      C_SdNdeDpContentUtil::h_GetAnyValueAsUint64(orc_Min, u64_Min, 0UL);
      C_SdNdeDpContentUtil::h_GetAnyValueAsUint64(orc_Max, u64_Max, 0UL);
      if ((u64_Min == 0ULL) && (u64_Max == 1ULL))
      {
         q_Retval = true;
      }
   }
   return q_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Handle all requests for column "Raw" in interpreted mode

   \param[in]  ou32_Index  Row index
   \param[in]  osn_Role    Requested role type

   \return
   Returned value as specified by role
*/
//----------------------------------------------------------------------------------------------------------------------
QVariant C_CamGenSigTableModel::m_HandleColRawInterpreted(const uint32 ou32_Index, const sintn osn_Role) const
{
   QVariant c_Retval;

   if ((osn_Role == static_cast<sintn>(Qt::DisplayRole)) || (osn_Role == static_cast<sintn>(Qt::EditRole)))
   {
      const C_OSCNodeDataPoolListElement * const pc_OsySignalCommon = m_GetSignalInterpretedOsyCommon(ou32_Index);
      const C_OSCCanSignal * const pc_OsySignal = m_GetSignalInterpretedOsy(ou32_Index);
      if ((pc_OsySignal != NULL) && (pc_OsySignalCommon != NULL))
      {
         const C_OSCNodeDataPoolContent c_Min = C_CamGenSigTableModel::mh_GetBorderValue(pc_OsySignalCommon->c_MinValue,
                                                                                         pc_OsySignal->u16_ComBitLength,
                                                                                         true);
         const C_OSCNodeDataPoolContent c_Max = C_CamGenSigTableModel::mh_GetBorderValue(pc_OsySignalCommon->c_MinValue,
                                                                                         pc_OsySignal->u16_ComBitLength,
                                                                                         false);
         if (m_CheckUseCheckBox(c_Min, c_Max))
         {
            //Don't handle checkbox
         }
         else
         {
            //Generic
            const C_CamProMessageData * const pc_Message =
               C_CamProHandler::h_GetInstance()->GetMessageConst(this->mu32_MessageIndex);
            if (pc_Message != NULL)
            {
               const std::vector<uint8> c_RawData = C_CamGenSigUtil::h_ConvertRawDataFormat(*pc_Message);
               const C_OSCNodeDataPoolContent c_ConvertedContent =
                  C_CamGenSigUtil::h_DecodeRawToContentSignal(c_RawData, *pc_OsySignal, pc_OsySignalCommon->c_MinValue);
               if (osn_Role == static_cast<sintn>(Qt::DisplayRole))
               {
                  QString c_DisplayValue;
                  C_SdNdeDpContentUtil::h_GetValueAsScaledString(c_ConvertedContent, 1.0, 0.0, c_DisplayValue, 0UL);
                  c_Retval = c_DisplayValue;
               }
               else
               {
                  c_Retval = C_SdNdeDpContentUtil::h_ConvertContentToGeneric(c_ConvertedContent, 0UL);
               }
            }
         }
      }
      else
      {
         const C_CieConverter::C_CIECanSignal * const pc_DbcSignal = m_GetSignalInterpretedDbc(ou32_Index);
         if (pc_DbcSignal != NULL)
         {
            const C_OSCNodeDataPoolContent c_Min = C_CamGenSigTableModel::mh_GetBorderValue(
               pc_DbcSignal->c_Element.c_MinValue, pc_DbcSignal->u16_ComBitLength, true);
            const C_OSCNodeDataPoolContent c_Max = C_CamGenSigTableModel::mh_GetBorderValue(
               pc_DbcSignal->c_Element.c_MinValue, pc_DbcSignal->u16_ComBitLength, false);
            if (m_CheckUseCheckBox(c_Min, c_Max))
            {
               //Don't handle checkbox
            }
            else
            {
               //Generic
               const C_CamProMessageData * const pc_Message =
                  C_CamProHandler::h_GetInstance()->GetMessageConst(this->mu32_MessageIndex);
               if (pc_Message != NULL)
               {
                  const std::vector<uint8> c_RawData = C_CamGenSigUtil::h_ConvertRawDataFormat(*pc_Message);
                  const C_OSCNodeDataPoolContent c_ConvertedContent =
                     C_CamGenSigTableModel::mh_DecodeRawToContentDbc(c_RawData, *pc_DbcSignal);
                  if (osn_Role == static_cast<sintn>(Qt::DisplayRole))
                  {
                     QString c_DisplayValue;
                     C_SdNdeDpContentUtil::h_GetValueAsScaledString(c_ConvertedContent, 1.0, 0.0, c_DisplayValue,
                                                                    0UL);
                     c_Retval = c_DisplayValue;
                  }
                  else
                  {
                     c_Retval = C_SdNdeDpContentUtil::h_ConvertScaledContentToGeneric(c_ConvertedContent, 1.0,
                                                                                      0.0, 0UL);
                  }
               }
            }
         }
      }
   }
   else if (osn_Role == static_cast<sintn>(Qt::CheckStateRole))
   {
      //Handle checkable flag
      const C_OSCNodeDataPoolListElement * const pc_OsySignalCommon = m_GetSignalInterpretedOsyCommon(ou32_Index);
      const C_OSCCanSignal * const pc_OsySignal = m_GetSignalInterpretedOsy(ou32_Index);
      if ((pc_OsySignal != NULL) && (pc_OsySignalCommon != NULL))
      {
         const C_OSCNodeDataPoolContent c_Min = C_CamGenSigTableModel::mh_GetBorderValue(pc_OsySignalCommon->c_MinValue,
                                                                                         pc_OsySignal->u16_ComBitLength,
                                                                                         true);
         const C_OSCNodeDataPoolContent c_Max = C_CamGenSigTableModel::mh_GetBorderValue(pc_OsySignalCommon->c_MinValue,
                                                                                         pc_OsySignal->u16_ComBitLength,
                                                                                         false);
         //Handle checkbox
         if (m_CheckUseCheckBox(c_Min, c_Max))
         {
            //Extract the bit value from message
            const C_CamProMessageData * const pc_Message =
               C_CamProHandler::h_GetInstance()->GetMessageConst(
                  this->mu32_MessageIndex);
            if (pc_Message != NULL)
            {
               const std::vector<uint8> c_Data = C_CamGenSigUtil::h_ConvertRawDataFormat(*pc_Message);
               //Get current state
               if (C_CamGenSigUtil::h_GetBit(pc_OsySignal->u16_ComBitStart, c_Data))
               {
                  c_Retval = static_cast<sintn>(Qt::Checked);
               }
               else
               {
                  c_Retval = static_cast<sintn>(Qt::Unchecked);
               }
            }
         }
      }
      else
      {
         const C_CieConverter::C_CIECanSignal * const pc_DbcSignal = m_GetSignalInterpretedDbc(ou32_Index);
         if (pc_DbcSignal != NULL)
         {
            const C_OSCNodeDataPoolContent c_Min = C_CamGenSigTableModel::mh_GetBorderValue(
               pc_DbcSignal->c_Element.c_MinValue, pc_DbcSignal->u16_ComBitLength, true);
            const C_OSCNodeDataPoolContent c_Max = C_CamGenSigTableModel::mh_GetBorderValue(
               pc_DbcSignal->c_Element.c_MinValue, pc_DbcSignal->u16_ComBitLength, false);
            //Handle checkbox
            if (m_CheckUseCheckBox(c_Min, c_Max))
            {
               //Extract the bit value from message
               const C_CamProMessageData * const pc_Message =
                  C_CamProHandler::h_GetInstance()->GetMessageConst(
                     this->mu32_MessageIndex);
               if (pc_Message != NULL)
               {
                  const std::vector<uint8> c_Data = C_CamGenSigUtil::h_ConvertRawDataFormat(*pc_Message);
                  //Get current state
                  if (C_CamGenSigUtil::h_GetBit(pc_DbcSignal->u16_ComBitStart, c_Data))
                  {
                     c_Retval = static_cast<sintn>(Qt::Checked);
                  }
                  else
                  {
                     c_Retval = static_cast<sintn>(Qt::Unchecked);
                  }
               }
            }
         }
      }
   }
   else if (osn_Role == msn_USER_ROLE_INTERACTION_ELEMENT_TYPE)
   {
      c_Retval = static_cast<sintn>(eURIEL_GENERIC_SPIN_BOX);
   }
   else if (osn_Role == msn_USER_ROLE_INTERACTION_GENERIC_SPIN_BOX_PARAMETERS_LIST)
   {
      const C_OSCNodeDataPoolListElement * const pc_OsySignalCommon = m_GetSignalInterpretedOsyCommon(ou32_Index);
      const C_OSCCanSignal * const pc_OsySignal = m_GetSignalInterpretedOsy(ou32_Index);
      if ((pc_OsySignal != NULL) && (pc_OsySignalCommon != NULL))
      {
         const C_OSCNodeDataPoolContent c_Min = C_CamGenSigTableModel::mh_GetBorderValue(pc_OsySignalCommon->c_MinValue,
                                                                                         pc_OsySignal->u16_ComBitLength,
                                                                                         true);
         const C_OSCNodeDataPoolContent c_Max = C_CamGenSigTableModel::mh_GetBorderValue(pc_OsySignalCommon->c_MinValue,
                                                                                         pc_OsySignal->u16_ComBitLength,
                                                                                         false);
         if (m_CheckUseCheckBox(c_Min, c_Max))
         {
            //Don't handle checkbox
         }
         else
         {
            //Generic spin box parameters
            QStringList c_List;
            //Min
            QString c_Tmp = C_SdNdeDpContentUtil::h_GetAllContentAsString(c_Min);
            c_List.push_back(c_Tmp);
            //Max
            c_Tmp = C_SdNdeDpContentUtil::h_GetAllContentAsString(c_Max);
            c_List.push_back(c_Tmp);
            //Factor
            c_Tmp = QString::number(1.0);
            c_List.push_back(c_Tmp);
            //Offset
            c_Tmp = QString::number(0.0);
            c_List.push_back(c_Tmp);

            c_Retval = c_List;
         }
      }
      else
      {
         const C_CieConverter::C_CIECanSignal * const pc_DbcSignal = m_GetSignalInterpretedDbc(ou32_Index);
         if (pc_DbcSignal != NULL)
         {
            const C_OSCNodeDataPoolContent c_Min = C_CamGenSigTableModel::mh_GetBorderValue(
               pc_DbcSignal->c_Element.c_MinValue, pc_DbcSignal->u16_ComBitLength, true);
            const C_OSCNodeDataPoolContent c_Max = C_CamGenSigTableModel::mh_GetBorderValue(
               pc_DbcSignal->c_Element.c_MinValue, pc_DbcSignal->u16_ComBitLength, false);
            if (m_CheckUseCheckBox(c_Min, c_Max))
            {
               //Don't handle checkbox
            }
            else
            {
               //Generic spin box parameters
               QStringList c_List;
               //Min
               QString c_Tmp = C_SdNdeDpContentUtil::h_GetAllContentAsString(c_Min);
               c_List.push_back(c_Tmp);
               //Max
               c_Tmp = C_SdNdeDpContentUtil::h_GetAllContentAsString(c_Max);
               c_List.push_back(c_Tmp);
               //Factor
               c_Tmp = QString::number(1.0);
               c_List.push_back(c_Tmp);
               //Offset
               c_Tmp = QString::number(0.0);
               c_List.push_back(c_Tmp);

               c_Retval = c_List;
            }
         }
      }
   }
   else
   {
      // return QVariant default
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Handle all requests for column "Physical" in interpreted mode

   \param[in]  ou32_Index  Row index
   \param[in]  osn_Role    Requested role type

   \return
   Returned value as specified by role
*/
//----------------------------------------------------------------------------------------------------------------------
QVariant C_CamGenSigTableModel::m_HandleColPhysicalInterpreted(const uint32 ou32_Index, const sintn osn_Role) const
{
   QVariant c_Retval;

   if ((osn_Role == static_cast<sintn>(Qt::DisplayRole)) || (osn_Role == static_cast<sintn>(Qt::EditRole)))
   {
      const C_OSCCanSignal * const pc_OsySignal = m_GetSignalInterpretedOsy(ou32_Index);
      if (pc_OsySignal != NULL)
      {
         //Generic
         const C_OSCNodeDataPoolListElement * const pc_OsySignalCommon = m_GetSignalInterpretedOsyCommon(ou32_Index);
         if (pc_OsySignalCommon != NULL)
         {
            const C_CamProMessageData * const pc_Message =
               C_CamProHandler::h_GetInstance()->GetMessageConst(this->mu32_MessageIndex);
            if (pc_Message != NULL)
            {
               const std::vector<uint8> c_RawData =
                  C_CamGenSigUtil::h_ConvertRawDataFormat(*pc_Message);
               const C_OSCNodeDataPoolContent c_ConvertedContent =
                  C_CamGenSigUtil::h_DecodeRawToContentSignal(c_RawData, *pc_OsySignal, pc_OsySignalCommon->c_MinValue);
               if (osn_Role == static_cast<sintn>(Qt::DisplayRole))
               {
                  QString c_DisplayValue;
                  C_SdNdeDpContentUtil::h_GetValueAsScaledString(c_ConvertedContent,
                                                                 pc_OsySignalCommon->f64_Factor,
                                                                 pc_OsySignalCommon->f64_Offset,
                                                                 c_DisplayValue, 0UL);
                  c_Retval = c_DisplayValue;
               }
               else
               {
                  c_Retval = C_SdNdeDpContentUtil::h_ConvertScaledContentToGeneric(c_ConvertedContent,
                                                                                   pc_OsySignalCommon->f64_Factor,
                                                                                   pc_OsySignalCommon->f64_Offset,
                                                                                   0UL);
               }
            }
         }
      }
      else
      {
         //Generic
         const C_CieConverter::C_CIECanSignal * const pc_DbcSignal = m_GetSignalInterpretedDbc(ou32_Index);
         if (pc_DbcSignal != NULL)
         {
            const C_CamProMessageData * const pc_Message =
               C_CamProHandler::h_GetInstance()->GetMessageConst(this->mu32_MessageIndex);
            if (pc_Message != NULL)
            {
               const std::vector<uint8> c_RawData = C_CamGenSigUtil::h_ConvertRawDataFormat(*pc_Message);
               const C_OSCNodeDataPoolContent c_ConvertedContent =
                  C_CamGenSigTableModel::mh_DecodeRawToContentDbc(c_RawData, *pc_DbcSignal);
               if (osn_Role == static_cast<sintn>(Qt::DisplayRole))
               {
                  QString c_DisplayValue;
                  C_SdNdeDpContentUtil::h_GetValueAsScaledString(c_ConvertedContent,
                                                                 pc_DbcSignal->c_Element.f64_Factor,
                                                                 pc_DbcSignal->c_Element.f64_Offset,
                                                                 c_DisplayValue, 0UL);
                  c_Retval = c_DisplayValue;
               }
               else
               {
                  //Value table needs raw value
                  if (pc_DbcSignal->c_ValueDescription.size() > 0UL)
                  {
                     c_Retval = C_SdNdeDpContentUtil::h_ConvertScaledContentToGeneric(c_ConvertedContent, 1.0,
                                                                                      0.0, 0UL);
                  }
                  else
                  {
                     c_Retval = C_SdNdeDpContentUtil::h_ConvertScaledContentToGeneric(c_ConvertedContent,
                                                                                      pc_DbcSignal->c_Element.f64_Factor,
                                                                                      pc_DbcSignal->c_Element.f64_Offset,
                                                                                      0UL);
                  }
               }
            }
         }
      }
   }
   else if ((osn_Role == msn_USER_ROLE_INTERACTION_COMBO_BOX_VALUES_LIST) ||
            (osn_Role == msn_USER_ROLE_INTERACTION_COMBO_BOX_STRINGS_LIST))
   {
      //Combo box values (DBC only)
      const C_CieConverter::C_CIECanSignal * const pc_DbcSignal = m_GetSignalInterpretedDbc(ou32_Index);
      if (pc_DbcSignal != NULL)
      {
         uint64 u64_Counter = 0ULL;
         //Use one bit less for signed
         const uint64 u64_AllowedNumValuesSigned =
            C_CamGenSigTableModel::mh_GetMax(pc_DbcSignal->u16_ComBitLength - 1UL);
         const uint64 u64_AllowedNumValues = C_CamGenSigTableModel::mh_GetMax(pc_DbcSignal->u16_ComBitLength);
         bool q_IsSigned;
         QStringList c_Strings;
         //Check for length and number of bits
         switch (pc_DbcSignal->c_Element.c_MinValue.GetType()) //lint !e788  //not all types handled explicitly
         {
         case C_OSCNodeDataPoolContent::eSINT8:
         case C_OSCNodeDataPoolContent::eSINT16:
         case C_OSCNodeDataPoolContent::eSINT32:
         case C_OSCNodeDataPoolContent::eSINT64:
            q_IsSigned = true;
            break;
         default:
            q_IsSigned = false;
            break;
         }
         //Handle allowed combo box values
         c_Strings.reserve(pc_DbcSignal->c_ValueDescription.size());
         for (std::map<stw_types::uint32, stw_scl::C_SCLString>::const_iterator c_It =
                 pc_DbcSignal->c_ValueDescription.begin();
              c_It != pc_DbcSignal->c_ValueDescription.end(); ++c_It)
         {
            //Smaller or equal as zero is also a valid index (+1 might have unintended effects with u64 max)
            if (u64_Counter <= u64_AllowedNumValues)
            {
               if (osn_Role == msn_USER_ROLE_INTERACTION_COMBO_BOX_STRINGS_LIST)
               {
                  c_Strings.push_back(c_It->second.c_str());
               }
               else
               {
                  //Temporary fix for signed values
                  if ((q_IsSigned) && (static_cast<uint64>(c_It->first) > u64_AllowedNumValuesSigned))
                  {
                     QString c_Negative = "-";
                     c_Negative += QString::number(u64_AllowedNumValues - static_cast<uint64>(c_It->first));
                     c_Strings.push_back(c_Negative);
                  }
                  else
                  {
                     c_Strings.push_back(QString::number(c_It->first));
                  }
               }
               //Keep track of number of added values (don't allow index out of range)
               ++u64_Counter;
            }
         }
         c_Retval = c_Strings;
      }
   }
   else if (osn_Role == msn_USER_ROLE_INTERACTION_ELEMENT_TYPE)
   {
      //Interaction type -> combo box or spin box (no check box for physical value)
      bool q_IsComboBox = false;
      const C_CieConverter::C_CIECanSignal * const pc_DbcSignal = m_GetSignalInterpretedDbc(ou32_Index);
      if (pc_DbcSignal != NULL)
      {
         if (pc_DbcSignal->c_ValueDescription.size() > 0UL)
         {
            q_IsComboBox = true;
         }
      }
      if (q_IsComboBox)
      {
         c_Retval = static_cast<sintn>(eURIEL_COMBO_BOX);
      }
      else
      {
         c_Retval = static_cast<sintn>(eURIEL_GENERIC_SPIN_BOX);
      }
   }
   else if (osn_Role == msn_USER_ROLE_INTERACTION_GENERIC_SPIN_BOX_PARAMETERS_LIST)
   {
      //Handle generic spin box parameters
      const C_OSCNodeDataPoolListElement * const pc_OsySignalCommon = m_GetSignalInterpretedOsyCommon(ou32_Index);
      const C_OSCCanSignal * const pc_OsySignal = m_GetSignalInterpretedOsy(ou32_Index);
      if ((pc_OsySignal != NULL) && (pc_OsySignalCommon != NULL))
      {
         const C_OSCNodeDataPoolContent c_Min = C_CamGenSigTableModel::mh_GetBorderValue(pc_OsySignalCommon->c_MinValue,
                                                                                         pc_OsySignal->u16_ComBitLength,
                                                                                         true);
         const C_OSCNodeDataPoolContent c_Max = C_CamGenSigTableModel::mh_GetBorderValue(pc_OsySignalCommon->c_MinValue,
                                                                                         pc_OsySignal->u16_ComBitLength,
                                                                                         false);
         //Generic spin box parameters
         QStringList c_List;
         //Min
         QString c_Tmp = C_SdNdeDpContentUtil::h_GetAllContentAsString(c_Min);
         c_List.push_back(c_Tmp);
         //Max
         c_Tmp = C_SdNdeDpContentUtil::h_GetAllContentAsString(c_Max);
         c_List.push_back(c_Tmp);
         //Factor
         c_Tmp = QString::number(pc_OsySignalCommon->f64_Factor);
         c_List.push_back(c_Tmp);
         //Offset
         c_Tmp = QString::number(pc_OsySignalCommon->f64_Offset);
         c_List.push_back(c_Tmp);

         c_Retval = c_List;
      }
      else
      {
         const C_CieConverter::C_CIECanSignal * const pc_DbcSignal = m_GetSignalInterpretedDbc(ou32_Index);
         if (pc_DbcSignal != NULL)
         {
            const C_OSCNodeDataPoolContent c_Min = C_CamGenSigTableModel::mh_GetBorderValue(
               pc_DbcSignal->c_Element.c_MinValue, pc_DbcSignal->u16_ComBitLength, true);
            const C_OSCNodeDataPoolContent c_Max = C_CamGenSigTableModel::mh_GetBorderValue(
               pc_DbcSignal->c_Element.c_MinValue, pc_DbcSignal->u16_ComBitLength, false);
            //Generic spin box parameters
            QStringList c_List;
            //Min
            QString c_Tmp = C_SdNdeDpContentUtil::h_GetAllContentAsString(c_Min);
            c_List.push_back(c_Tmp);
            //Max
            c_Tmp = C_SdNdeDpContentUtil::h_GetAllContentAsString(c_Max);
            c_List.push_back(c_Tmp);
            //Factor
            c_Tmp = QString::number(pc_DbcSignal->c_Element.f64_Factor);
            c_List.push_back(c_Tmp);
            //Offset
            c_Tmp = QString::number(pc_DbcSignal->c_Element.f64_Offset);
            c_List.push_back(c_Tmp);

            c_Retval = c_List;
         }
      }
   }
   else if (osn_Role == msn_USER_ROLE_INTERACTION_COMBO_BOX_EDITABLE)
   {
      c_Retval = true;
   }
   else
   {
      // return QVariant default
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get initial value (i.e. minimum value)

   \param[in]  opc_OsySignalCommon  OSY signal common (datapool list element)
   \param[in]  opc_DbcSignal        DBC signal

   \return
   Initial value for signal.
*/
//----------------------------------------------------------------------------------------------------------------------
C_OSCNodeDataPoolContent C_CamGenSigTableModel::mh_GetInitialValue(
   const C_OSCNodeDataPoolListElement * const opc_OsySignalCommon,
   const C_CieConverter::C_CIECanSignal * const opc_DbcSignal)
{
   C_OSCNodeDataPoolContent c_Return;

   if (opc_OsySignalCommon != NULL)
   {
      if (opc_OsySignalCommon->c_DataSetValues.size() > 0UL)
      {
         c_Return = opc_OsySignalCommon->c_DataSetValues[0UL]; // init value
      }
   }
   else if (opc_DbcSignal != NULL)
   {
      if (opc_DbcSignal->c_Element.c_DataSetValues.size() > 0UL)
      {
         c_Return = opc_DbcSignal->c_Element.c_DataSetValues[0UL];
      }
   }
   else
   {
      //Should not happen
   }

   return c_Return;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Utility so set a boolean in a content structure

   true -> 1
   false -> 0

   \param[in,out]  orc_Value  Value to change
   \param[in]      oq_Value   New value to set
*/
//----------------------------------------------------------------------------------------------------------------------
void C_CamGenSigTableModel::mh_SetBoolInContent(C_OSCNodeDataPoolContent & orc_Value, const bool oq_Value)
{
   switch (orc_Value.GetType())
   {
   case C_OSCNodeDataPoolContent::eUINT8:
      if (oq_Value)
      {
         orc_Value.SetValueU8(1U);
      }
      else
      {
         orc_Value.SetValueU8(0U);
      }
      break;
   case C_OSCNodeDataPoolContent::eUINT16:
      if (oq_Value)
      {
         orc_Value.SetValueU16(1U);
      }
      else
      {
         orc_Value.SetValueU16(0U);
      }
      break;
   case C_OSCNodeDataPoolContent::eUINT32:
      if (oq_Value)
      {
         orc_Value.SetValueU32(1UL);
      }
      else
      {
         orc_Value.SetValueU32(0UL);
      }
      break;
   case C_OSCNodeDataPoolContent::eUINT64:
      if (oq_Value)
      {
         orc_Value.SetValueU64(1ULL);
      }
      else
      {
         orc_Value.SetValueU64(0ULL);
      }
      break;
   case C_OSCNodeDataPoolContent::eSINT8:
      if (oq_Value)
      {
         orc_Value.SetValueS8(1);
      }
      else
      {
         orc_Value.SetValueS8(0);
      }
      break;
   case C_OSCNodeDataPoolContent::eSINT16:
      if (oq_Value)
      {
         orc_Value.SetValueS16(1);
      }
      else
      {
         orc_Value.SetValueS16(0);
      }
      break;
   case C_OSCNodeDataPoolContent::eSINT32:
      if (oq_Value)
      {
         orc_Value.SetValueS32(1L);
      }
      else
      {
         orc_Value.SetValueS32(0L);
      }
      break;
   case C_OSCNodeDataPoolContent::eSINT64:
      if (oq_Value)
      {
         orc_Value.SetValueS64(1LL);
      }
      else
      {
         orc_Value.SetValueS64(0LL);
      }
      break;
   case C_OSCNodeDataPoolContent::eFLOAT32:
      if (oq_Value)
      {
         orc_Value.SetValueF32(1.0F);
      }
      else
      {
         orc_Value.SetValueF32(0.0F);
      }
      break;
   case C_OSCNodeDataPoolContent::eFLOAT64:
      if (oq_Value)
      {
         orc_Value.SetValueF64(1.0);
      }
      else
      {
         orc_Value.SetValueF64(0.0);
      }
      break;
   default:
      tgl_assert(false);
      break;
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Set signal from Datapool content value

   \param[in]  orc_OsySignal  openSYDE signal
   \param[in]  orc_Value      Value

   \return
   C_NO_ERR Operation success
   C_RANGE  Operation failure: parameter invalid
*/
//----------------------------------------------------------------------------------------------------------------------
sint32 C_CamGenSigTableModel::m_SetSignalFromOsyValue(const C_OSCCanSignal & orc_OsySignal,
                                                      const C_OSCNodeDataPoolContent & orc_Value)
{
   sint32 s32_Retval = C_NO_ERR;
   const C_CamProMessageData * const pc_Message =
      C_CamProHandler::h_GetInstance()->GetMessageConst(this->mu32_MessageIndex);

   if (pc_Message != NULL)
   {
      //Get raw as vector
      std::vector<uint8> c_RawData = C_CamGenSigUtil::h_ConvertRawDataFormat(*pc_Message);
      //Encode new data
      C_CamGenSigUtil::h_DecodeSignalValueToRaw(c_RawData, orc_OsySignal, orc_Value);
      //Write new values
      s32_Retval = C_CamProHandler::h_GetInstance()->SetMessageDataBytes(this->mu32_MessageIndex, c_RawData);
   }
   else
   {
      s32_Retval = C_RANGE;
   }

   return s32_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Function to get border (min/max) values depending on current bit length

   \param[in]  orc_InitValue     Init value to use for output content type
   \param[in]  ou16_BitLength    Signal bit length (only supported up to 64 bit)
   \param[in]  oq_IsMin          Flag to switch for min or max value

   \return
   Border value with the init value type and the value set as specified by the bit length and the border flag
*/
//----------------------------------------------------------------------------------------------------------------------
C_OSCNodeDataPoolContent C_CamGenSigTableModel::mh_GetBorderValue(const C_OSCNodeDataPoolContent & orc_InitValue,
                                                                  const uint16 ou16_BitLength, const bool oq_IsMin)
{
   C_OSCNodeDataPoolContent c_Retval = orc_InitValue;

   //Handle out of range (no error reporting)
   if (ou16_BitLength > 64U)
   {
      if (oq_IsMin)
      {
         C_SdNdeDpContentUtil::h_InitMin(c_Retval);
      }
      else
      {
         C_SdNdeDpContentUtil::h_InitMax(c_Retval);
      }
      //Error handling
      osc_write_log_warning("Message signal edit", "Signals over 64 bit are not supported");
   }
   else
   {
      float64 f64_Base;
      uint64 u64_Base;
      sint64 s64_Base;

      switch (orc_InitValue.GetType())
      {
      case C_OSCNodeDataPoolContent::eSINT8:
      case C_OSCNodeDataPoolContent::eSINT16:
      case C_OSCNodeDataPoolContent::eSINT32:
      case C_OSCNodeDataPoolContent::eSINT64:
         //Get min/max value in appropriate format
         if (oq_IsMin)
         {
            //Special handling for 64 bit
            if (ou16_BitLength >= 64)
            {
               s64_Base = std::numeric_limits<sint64>::lowest();
            }
            else
            {
               //Use one bit less for signed
               const uint64 u64_Max = C_CamGenSigTableModel::mh_GetMax(ou16_BitLength - 1UL);
               s64_Base = static_cast<sint64>(u64_Max);
               //Invert to negative
               s64_Base *= -1LL;
               //Subtract one more as negative range is one bigger than positive range
               s64_Base -= 1LL;
            }
         }
         else
         {
            //Special handling for 64 bit
            if (ou16_BitLength >= 64)
            {
               s64_Base = std::numeric_limits<sint64>::max();
            }
            else
            {
               //Use one bit less for signed
               const uint64 u64_Max = C_CamGenSigTableModel::mh_GetMax(ou16_BitLength - 1UL);
               s64_Base = static_cast<sint64>(u64_Max);
            }
         }
         //Apply value
         if (orc_InitValue.GetType() == C_OSCNodeDataPoolContent::eSINT8)
         {
            if (orc_InitValue.GetArray())
            {
               c_Retval.SetValueAS8Element(static_cast<sint8>(s64_Base), 0UL);
            }
            else
            {
               c_Retval.SetValueS8(static_cast<sint8>(s64_Base));
            }
         }
         else if (orc_InitValue.GetType() == C_OSCNodeDataPoolContent::eSINT16)
         {
            if (orc_InitValue.GetArray())
            {
               c_Retval.SetValueAS16Element(static_cast<sint16>(s64_Base), 0UL);
            }
            else
            {
               c_Retval.SetValueS16(static_cast<sint16>(s64_Base));
            }
         }
         else if (orc_InitValue.GetType() == C_OSCNodeDataPoolContent::eSINT32)
         {
            if (orc_InitValue.GetArray())
            {
               c_Retval.SetValueAS32Element(static_cast<sint32>(s64_Base), 0UL);
            }
            else
            {
               c_Retval.SetValueS32(static_cast<sint32>(s64_Base));
            }
         }
         else
         {
            if (orc_InitValue.GetArray())
            {
               c_Retval.SetValueAS64Element(s64_Base, 0UL);
            }
            else
            {
               c_Retval.SetValueS64(s64_Base);
            }
         }
         break;
      case C_OSCNodeDataPoolContent::eUINT8:
      case C_OSCNodeDataPoolContent::eUINT16:
      case C_OSCNodeDataPoolContent::eUINT32:
      case C_OSCNodeDataPoolContent::eUINT64:
         //Get min/max value in appropriate format
         if (oq_IsMin)
         {
            u64_Base = 0ULL;
         }
         else
         {
            //Special handling for 64 bit
            if (ou16_BitLength >= 64)
            {
               u64_Base = std::numeric_limits<uint64>::max();
            }
            else
            {
               //Use one bit less for signed
               u64_Base = C_CamGenSigTableModel::mh_GetMax(ou16_BitLength);
            }
         }
         //Apply value
         if (orc_InitValue.GetType() == C_OSCNodeDataPoolContent::eUINT8)
         {
            if (orc_InitValue.GetArray())
            {
               c_Retval.SetValueAU8Element(static_cast<uint8>(u64_Base), 0UL);
            }
            else
            {
               c_Retval.SetValueU8(static_cast<uint8>(u64_Base));
            }
         }
         else if (orc_InitValue.GetType() == C_OSCNodeDataPoolContent::eUINT16)
         {
            if (orc_InitValue.GetArray())
            {
               c_Retval.SetValueAU16Element(static_cast<uint16>(u64_Base), 0UL);
            }
            else
            {
               c_Retval.SetValueU16(static_cast<uint16>(u64_Base));
            }
         }
         else if (orc_InitValue.GetType() == C_OSCNodeDataPoolContent::eUINT32)
         {
            if (orc_InitValue.GetArray())
            {
               c_Retval.SetValueAU32Element(static_cast<sint32>(u64_Base), 0UL);
            }
            else
            {
               c_Retval.SetValueU32(static_cast<uint32>(u64_Base));
            }
         }
         else
         {
            if (orc_InitValue.GetArray())
            {
               c_Retval.SetValueAU64Element(u64_Base, 0UL);
            }
            else
            {
               c_Retval.SetValueU64(u64_Base);
            }
         }
         break;
      case C_OSCNodeDataPoolContent::eFLOAT32:
      case C_OSCNodeDataPoolContent::eFLOAT64:
         //Get min/max value in appropriate format
         if (oq_IsMin)
         {
            //Special handling for 64 bit
            if (ou16_BitLength >= 64)
            {
               f64_Base = std::numeric_limits<float64>::lowest();
            }
            else
            {
               f64_Base = static_cast<float64>(std::numeric_limits<float32>::lowest());
            }
         }
         else
         {
            //Special handling for 64 bit
            if (ou16_BitLength >= 64)
            {
               f64_Base = std::numeric_limits<float64>::max();
            }
            else
            {
               f64_Base = static_cast<float64>(std::numeric_limits<float32>::max());
            }
         }
         //Apply value
         C_OSCNodeDataPoolContentUtil::h_SetValueInContent(f64_Base, c_Retval, 0UL);
         break;
      default:
         tgl_assert(false);
         break;
      }
   }

   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get maximum value for an unsigned variable with the specified number of bits

   \param[in]  ou16_Bit    Number of bits for variable (only supported up to 64 bit)

   \return
   Maximum value for an unsigned variable with the specified number of bits
*/
//----------------------------------------------------------------------------------------------------------------------
uint64 C_CamGenSigTableModel::mh_GetMax(const uint16 ou16_Bit)
{
   uint64 u64_Max = 0ULL;

   if (ou16_Bit <= 64)
   {
      for (uint16 u16_ItBit = 0UL; u16_ItBit < ou16_Bit; ++u16_ItBit)
      {
         const uint64 u64_Val = static_cast<uint64>(1ULL) << u16_ItBit;
         u64_Max += u64_Val;
      }
   }
   else
   {
      //Error handling
      osc_write_log_warning("Message signal edit", "Signals over 64 bit are not supported");
   }
   return u64_Max;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Check if message is multiplexed

   \param[in]  opc_OsyMessage    OSY message
   \param[in]  opc_DbcMessage    DBC message

   \retval true  Is multiplexed
   \retval false Not multiplexed
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_CamGenSigTableModel::mh_IsMultiplexed(const C_OSCCanMessage * const opc_OsyMessage,
                                             const C_CieConverter::C_CIECanMessage * const opc_DbcMessage)
{
   bool q_IsMultiplexed;

   if (opc_OsyMessage != NULL)
   {
      q_IsMultiplexed = opc_OsyMessage->IsMultiplexed();
   }
   else if (opc_DbcMessage != NULL)
   {
      q_IsMultiplexed = false;
      for (uint32 u32_ItSig = 0UL; u32_ItSig < opc_DbcMessage->c_Signals.size(); ++u32_ItSig)
      {
         const C_CieConverter::C_CIECanSignal & rc_Sig = opc_DbcMessage->c_Signals[u32_ItSig];
         if (rc_Sig.e_MultiplexerType == C_OSCCanSignal::eMUX_MULTIPLEXER_SIGNAL)
         {
            q_IsMultiplexed = true;
            break;
         }
      }
   }
   else
   {
      q_IsMultiplexed = false;
   }
   return q_IsMultiplexed;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get multiplexer value if any

   \param[in]  opc_OsyMessage    OSY message
   \param[in]  opc_DbcMessage    DBC message

   \return
   Multiplexer value if any
*/
//----------------------------------------------------------------------------------------------------------------------
uint16 C_CamGenSigTableModel::m_GetMultiplexerValue(const C_OSCCanMessage * const opc_OsyMessage,
                                                    const C_CieConverter::C_CIECanMessage * const opc_DbcMessage) const
{
   uint16 u16_Retval = 0;
   const C_CamProMessageData * const pc_Message =
      C_CamProHandler::h_GetInstance()->GetMessageConst(
         this->mu32_MessageIndex);

   if (pc_Message != NULL)
   {
      const std::vector<uint8> c_RawData =
         C_CamGenSigUtil::h_ConvertRawDataFormat(*pc_Message);

      if (opc_OsyMessage != NULL)
      {
         for (uint32 u32_ItSig = 0UL; u32_ItSig < opc_OsyMessage->c_Signals.size(); ++u32_ItSig)
         {
            const C_OSCCanSignal & rc_Sig = opc_OsyMessage->c_Signals[u32_ItSig];
            if (rc_Sig.e_MultiplexerType == C_OSCCanSignal::eMUX_MULTIPLEXER_SIGNAL)
            {
               const C_OSCNodeDataPoolListElement * const pc_OsySignalCommon =
                  m_GetSignalInterpretedOsyCommon(u32_ItSig);
               if (pc_OsySignalCommon != NULL)
               {
                  sint64 s64_Tmp;
                  const C_OSCNodeDataPoolContent c_ConvertedContent =
                     C_CamGenSigUtil::h_DecodeRawToContentSignal(
                        c_RawData, rc_Sig, pc_OsySignalCommon->c_MinValue);
                  C_SdNdeDpContentUtil::h_GetAnyValueAsSint64(c_ConvertedContent, s64_Tmp, 0UL);
                  u16_Retval = static_cast<uint16>(s64_Tmp);
               }
               break;
            }
         }
      }
      else if (opc_DbcMessage != NULL)
      {
         for (uint32 u32_ItSig = 0UL; u32_ItSig < opc_DbcMessage->c_Signals.size(); ++u32_ItSig)
         {
            const C_CieConverter::C_CIECanSignal & rc_Sig = opc_DbcMessage->c_Signals[u32_ItSig];
            if (rc_Sig.e_MultiplexerType == C_OSCCanSignal::eMUX_MULTIPLEXER_SIGNAL)
            {
               sint64 s64_Tmp;
               const C_OSCNodeDataPoolContent c_ConvertedContent = C_CamGenSigTableModel::mh_DecodeRawToContentDbc(
                  c_RawData, rc_Sig);
               C_SdNdeDpContentUtil::h_GetAnyValueAsSint64(c_ConvertedContent, s64_Tmp, 0UL);
               u16_Retval = static_cast<uint16>(s64_Tmp);
               break;
            }
         }
      }
      else
      {
         //Nothing to do
      }
   }
   return u16_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get number of signals for the current mux value

   \param[in]  opc_OsyMessage    OSY message
   \param[in]  opc_DbcMessage    DBC message
   \param[in]  ou16_MuxValue     Current mux value

   \return
   Number of signals for the current mux value
*/
//----------------------------------------------------------------------------------------------------------------------
sint32 C_CamGenSigTableModel::mh_GetNumRowsForMuxValue(const C_OSCCanMessage * const opc_OsyMessage,
                                                       const C_CieConverter::C_CIECanMessage * const opc_DbcMessage,
                                                       const uint16 ou16_MuxValue)
{
   sint32 s32_Retval = 0;

   if (opc_OsyMessage != NULL)
   {
      for (uint32 u32_ItSig = 0UL; u32_ItSig < opc_OsyMessage->c_Signals.size(); ++u32_ItSig)
      {
         const C_OSCCanSignal & rc_Sig = opc_OsyMessage->c_Signals[u32_ItSig];
         if (rc_Sig.e_MultiplexerType == C_OSCCanSignal::eMUX_MULTIPLEXED_SIGNAL)
         {
            if (rc_Sig.u16_MultiplexValue == ou16_MuxValue)
            {
               //Count
               ++s32_Retval;
            }
         }
         else
         {
            //Count
            ++s32_Retval;
         }
      }
   }
   else if (opc_DbcMessage != NULL)
   {
      for (uint32 u32_ItSig = 0UL; u32_ItSig < opc_DbcMessage->c_Signals.size(); ++u32_ItSig)
      {
         const C_CieConverter::C_CIECanSignal & rc_Sig = opc_DbcMessage->c_Signals[u32_ItSig];
         if (rc_Sig.e_MultiplexerType == C_OSCCanSignal::eMUX_MULTIPLEXED_SIGNAL)
         {
            if (rc_Sig.u16_MultiplexValue == ou16_MuxValue)
            {
               //Count
               ++s32_Retval;
            }
         }
         else
         {
            //Count
            ++s32_Retval;
         }
      }
   }
   else
   {
      //Nothing to do
   }
   return s32_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Translate row index in message index

   \param[in]  os32_Row    Row

   \return
   Index in message
*/
//----------------------------------------------------------------------------------------------------------------------
uint32 C_CamGenSigTableModel::m_TranslateRowToIndex(const sint32 os32_Row) const
{
   //Default
   uint32 u32_Retval = static_cast<uint32>(os32_Row);
   const C_OSCCanMessage * const pc_OsyMessage = m_GetMessageInterpretedOsy();
   const C_CieConverter::C_CIECanMessage * const pc_DbcMessage = m_GetMessageInterpretedDbc();

   if ((pc_OsyMessage != NULL) || (pc_DbcMessage != NULL))
   {
      const bool q_IsMultiplexed = C_CamGenSigTableModel::mh_IsMultiplexed(pc_OsyMessage, pc_DbcMessage);
      if (q_IsMultiplexed)
      {
         const uint16 u16_MuxValue = C_CamGenSigTableModel::m_GetMultiplexerValue(pc_OsyMessage, pc_DbcMessage);
         std::vector<uint16> c_StartBits = C_CamGenSigTableModel::mh_GetStartBits(pc_OsyMessage, pc_DbcMessage,
                                                                                  u16_MuxValue);
         //Sort
         //lint -e{864} Call as expected by interface
         std::sort(c_StartBits.begin(), c_StartBits.end());
         //Get index for start bit
         if (pc_OsyMessage != NULL)
         {
            const uint16 u16_SearchedStartBit = c_StartBits[static_cast<uint32>(os32_Row)];
            for (uint32 u32_ItSig = 0UL; u32_ItSig < pc_OsyMessage->c_Signals.size(); ++u32_ItSig)
            {
               const C_OSCCanSignal & rc_Sig = pc_OsyMessage->c_Signals[u32_ItSig];
               if (rc_Sig.u16_ComBitStart == u16_SearchedStartBit)
               {
                  //Also check if it is actually part of the current message
                  if ((rc_Sig.e_MultiplexerType != C_OSCCanSignal::eMUX_MULTIPLEXED_SIGNAL) ||
                      (rc_Sig.u16_MultiplexValue == u16_MuxValue))
                  {
                     u32_Retval = u32_ItSig;
                     break;
                  }
               }
            }
         }
         else if (pc_DbcMessage != NULL)
         {
            const uint16 u16_SearchedStartBit = c_StartBits[static_cast<uint32>(os32_Row)];
            for (uint32 u32_ItSig = 0UL; u32_ItSig < pc_DbcMessage->c_Signals.size(); ++u32_ItSig)
            {
               const C_CieConverter::C_CIECanSignal & rc_Sig = pc_DbcMessage->c_Signals[u32_ItSig];
               if (rc_Sig.u16_ComBitStart == u16_SearchedStartBit)
               {
                  //Also check if it is actually part of the current message
                  if ((rc_Sig.e_MultiplexerType != C_OSCCanSignal::eMUX_MULTIPLEXED_SIGNAL) ||
                      (rc_Sig.u16_MultiplexValue == u16_MuxValue))
                  {
                     u32_Retval = u32_ItSig;
                     break;
                  }
               }
            }
         }
         else
         {
         }
      }
   }

   return u32_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get start bit locations for the current mux value

   \param[in]  opc_OsyMessage    OSY message
   \param[in]  opc_DbcMessage    DBC message
   \param[in]  ou16_MuxValue     Mux value

   \return
   Start bit locations for the current mux value
*/
//----------------------------------------------------------------------------------------------------------------------
std::vector<uint16> C_CamGenSigTableModel::mh_GetStartBits(const C_OSCCanMessage * const opc_OsyMessage,
                                                           const C_CieConverter::C_CIECanMessage * const opc_DbcMessage,
                                                           const uint16 ou16_MuxValue)
{
   std::vector<uint16> c_Retval;
   if (opc_OsyMessage != NULL)
   {
      for (uint32 u32_ItSig = 0UL; u32_ItSig < opc_OsyMessage->c_Signals.size(); ++u32_ItSig)
      {
         const C_OSCCanSignal & rc_Sig = opc_OsyMessage->c_Signals[u32_ItSig];
         if (rc_Sig.e_MultiplexerType == C_OSCCanSignal::eMUX_MULTIPLEXED_SIGNAL)
         {
            if (rc_Sig.u16_MultiplexValue == ou16_MuxValue)
            {
               c_Retval.push_back(rc_Sig.u16_ComBitStart);
            }
         }
         else
         {
            c_Retval.push_back(rc_Sig.u16_ComBitStart);
         }
      }
   }
   else if (opc_DbcMessage != NULL)
   {
      for (uint32 u32_ItSig = 0UL; u32_ItSig < opc_DbcMessage->c_Signals.size(); ++u32_ItSig)
      {
         const C_CieConverter::C_CIECanSignal & rc_Sig = opc_DbcMessage->c_Signals[u32_ItSig];
         if (rc_Sig.e_MultiplexerType == C_OSCCanSignal::eMUX_MULTIPLEXED_SIGNAL)
         {
            if (rc_Sig.u16_MultiplexValue == ou16_MuxValue)
            {
               c_Retval.push_back(rc_Sig.u16_ComBitStart);
            }
         }
         else
         {
            c_Retval.push_back(rc_Sig.u16_ComBitStart);
         }
      }
   }
   else
   {
   }
   return c_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Reset message unused multiplexed signals
*/
//----------------------------------------------------------------------------------------------------------------------
void C_CamGenSigTableModel::m_ResetUnusedMultiplexedSignals(void)
{
   const C_OSCCanMessage * const pc_OsyMessage = m_GetMessageInterpretedOsy();
   const C_CieConverter::C_CIECanMessage * const pc_DbcMessage = m_GetMessageInterpretedDbc();

   const uint16 u16_MultiplexValue = this->m_GetMultiplexerValue(pc_OsyMessage, pc_DbcMessage);

   if (pc_OsyMessage != NULL)
   {
      // first reset all unused signals to zero
      for (uint32 u32_ItSig = 0UL; u32_ItSig < pc_OsyMessage->c_Signals.size(); ++u32_ItSig)
      {
         this->m_UpdateMultiplexedSignal(pc_OsyMessage, NULL, u16_MultiplexValue, u32_ItSig, true);
      }
      // then set all used to initial value (might overwrite signal that was recently set to zero)
      for (uint32 u32_ItSig = 0UL; u32_ItSig < pc_OsyMessage->c_Signals.size(); ++u32_ItSig)
      {
         this->m_UpdateMultiplexedSignal(pc_OsyMessage, NULL, u16_MultiplexValue, u32_ItSig, false);
      }
   }
   else if (pc_DbcMessage != NULL)
   {
      // first reset all unused signals to zero
      for (uint32 u32_ItSig = 0UL; u32_ItSig < pc_DbcMessage->c_Signals.size(); ++u32_ItSig)
      {
         this->m_UpdateMultiplexedSignal(NULL, pc_DbcMessage, u16_MultiplexValue, u32_ItSig, true);
      }
      // then set all used to initial value (might overwrite signal that was recently set to zero)
      for (uint32 u32_ItSig = 0UL; u32_ItSig < pc_DbcMessage->c_Signals.size(); ++u32_ItSig)
      {
         this->m_UpdateMultiplexedSignal(NULL, pc_DbcMessage, u16_MultiplexValue, u32_ItSig, false);
      }
   }
   else
   {
      //Nothing to do
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Update multiplexed signal

   Either reset to zero or reset to initial value, depending on flag oq_SetToZero.

   \param[in]  opc_OsyMessage    OSY message
   \param[in]  opc_DbcMessage    DBC message
   \param[in]  ou16_MuxValue     Multiplex value
   \param[in]  ou32_SignalIndex  Signal index
   \param[in]  oq_SetToZero      true: set to zero; false: reset to initial value
*/
//----------------------------------------------------------------------------------------------------------------------
void C_CamGenSigTableModel::m_UpdateMultiplexedSignal(const C_OSCCanMessage * const opc_OsyMessage,
                                                      const C_CieConverter::C_CIECanMessage * const opc_DbcMessage,
                                                      const uint16 ou16_MuxValue, const uint32 ou32_SignalIndex,
                                                      const bool oq_SetToZero)
{
   if (opc_OsyMessage != NULL)
   {
      const C_OSCCanSignal & rc_Sig = opc_OsyMessage->c_Signals[ou32_SignalIndex];

      if (rc_Sig.e_MultiplexerType == C_OSCCanSignal::eMUX_MULTIPLEXED_SIGNAL)
      {
         const C_OSCNodeDataPoolListElement * const pc_OsySignalCommon =
            m_GetSignalInterpretedOsyCommon(ou32_SignalIndex);
         if (pc_OsySignalCommon != NULL)
         {
            if (oq_SetToZero == true)
            {
               if (rc_Sig.u16_MultiplexValue != ou16_MuxValue)
               {
                  // set to zero
                  C_OSCNodeDataPoolContent c_InitialValue = mh_GetInitialValue(pc_OsySignalCommon, NULL);
                  C_OSCNodeDataPoolContentUtil::h_ZeroContent(c_InitialValue);
                  tgl_assert(this->m_SetSignalFromOsyValue(rc_Sig, c_InitialValue) == C_NO_ERR);
               }
            }
            else
            {
               if (rc_Sig.u16_MultiplexValue == ou16_MuxValue)
               {
                  // reset to initial value
                  const C_OSCNodeDataPoolContent & rc_InitialValue = mh_GetInitialValue(pc_OsySignalCommon, NULL);
                  tgl_assert(this->m_SetSignalFromOsyValue(rc_Sig, rc_InitialValue) == C_NO_ERR);
               }
            }
         }
      }
   }
   else if (opc_DbcMessage != NULL)
   {
      const C_CieConverter::C_CIECanSignal & rc_Sig = opc_DbcMessage->c_Signals[ou32_SignalIndex];
      if (rc_Sig.e_MultiplexerType == C_OSCCanSignal::eMUX_MULTIPLEXED_SIGNAL)
      {
         const C_CieConverter::C_CIECanSignal * const pc_DbcSignal = m_GetSignalInterpretedDbc(ou32_SignalIndex);
         if (pc_DbcSignal != NULL)
         {
            if (oq_SetToZero == true)
            {
               if (rc_Sig.u16_MultiplexValue != ou16_MuxValue)
               {
                  // set to zero
                  C_OSCNodeDataPoolContent c_InitialValue = mh_GetInitialValue(NULL, pc_DbcSignal);
                  const C_OSCCanSignal & rc_OsySig = C_CamGenSigUtil::h_ConvertDBCToOSY(*pc_DbcSignal);
                  C_OSCNodeDataPoolContentUtil::h_ZeroContent(c_InitialValue);
                  tgl_assert(this->m_SetSignalFromOsyValue(rc_OsySig, c_InitialValue) == C_NO_ERR);
               }
            }
            else
            {
               if (rc_Sig.u16_MultiplexValue == ou16_MuxValue)
               {
                  // reset to initial value
                  const C_OSCNodeDataPoolContent & rc_InitialValue = mh_GetInitialValue(NULL, pc_DbcSignal);
                  const C_OSCCanSignal & rc_OsySig = C_CamGenSigUtil::h_ConvertDBCToOSY(*pc_DbcSignal);
                  tgl_assert(this->m_SetSignalFromOsyValue(rc_OsySig, rc_InitialValue) == C_NO_ERR);
               }
            }
         }
      }
   }
   else
   {
      //Nothing to do
   }
}
