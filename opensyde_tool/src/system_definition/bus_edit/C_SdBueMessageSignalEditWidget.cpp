//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for editing messages and / or signals

   Widget for editing messages and / or signals

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include "stwerrors.h"
#include "C_UsHandler.h"
#include "C_PuiSdHandler.h"
#include "C_SdBueMessageSignalEditWidget.h"
#include "ui_C_SdBueMessageSignalEditWidget.h"
#include "C_GtGetText.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_errors;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_core;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in,out]  opc_Parent    Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdBueMessageSignalEditWidget::C_SdBueMessageSignalEditWidget(QWidget * const opc_Parent) :
   QWidget(opc_Parent),
   mpc_Ui(new Ui::C_SdBueMessageSignalEditWidget)
{
   mpc_Ui->setupUi(this);
   this->mpc_Ui->pc_MsgPropertiesWidget->setVisible(false);
   this->mpc_Ui->pc_SignalLabel->setVisible(false);

   this->InitStaticNames();

   //Init splitter
   this->mpc_Ui->pc_Splitter->SetMargins(9, 0);
   this->mpc_Ui->pc_Splitter->setStretchFactor(0, 1);
   this->mpc_Ui->pc_Splitter->setStretchFactor(1, 1000);

   connect(this->mpc_Ui->pc_MsgPropertiesWidget, &C_SdBueMessagePropertiesWidget::SigMessageIdChanged, this,
           &C_SdBueMessageSignalEditWidget::m_OnMessageIdChange);
   connect(this->mpc_Ui->pc_MsgPropertiesWidget, &C_SdBueMessagePropertiesWidget::SigMessageNameChanged, this,
           &C_SdBueMessageSignalEditWidget::m_OnMessageNameChange);
   connect(this->mpc_Ui->pc_MsgPropertiesWidget, &C_SdBueMessagePropertiesWidget::SigMessageDlcChanged, this,
           &C_SdBueMessageSignalEditWidget::m_OnMessageDlcChange);
   connect(this->mpc_Ui->pc_MsgLayoutViewerWidget, &C_SdBueMlvWidget::SigMessageUpdated, this,
           &C_SdBueMessageSignalEditWidget::m_OnSignalUpdatedViaSelector);
   connect(this->mpc_Ui->pc_SigPropertiesWidget, &C_SdBueSignalPropertiesWidget::SigUpdateMlv, this,
           &C_SdBueMessageSignalEditWidget::m_OnSignalUpdatedViaProperties);
   connect(this->mpc_Ui->pc_SigPropertiesWidget, &C_SdBueSignalPropertiesWidget::SigNameChanged, this,
           &C_SdBueMessageSignalEditWidget::m_OnSignalNameChanged);
   connect(this->mpc_Ui->pc_SigPropertiesWidget, &C_SdBueSignalPropertiesWidget::SigStartBitChanged, this,
           &C_SdBueMessageSignalEditWidget::m_OnSignalStartBitChanged);
   connect(this->mpc_Ui->pc_MsgLayoutViewerWidget, &C_SdBueMlvWidget::SigSignalActivated, this,
           &C_SdBueMessageSignalEditWidget::m_OnSignalActivated);
   //Error
   connect(this->mpc_Ui->pc_MsgPropertiesWidget, &C_SdBueMessagePropertiesWidget::SigRecheckError, this,
           &C_SdBueMessageSignalEditWidget::m_RecheckErrorGlobal);
   connect(this->mpc_Ui->pc_SigPropertiesWidget, &C_SdBueSignalPropertiesWidget::SigRecheckError, this,
           &C_SdBueMessageSignalEditWidget::m_RecheckErrorGlobal);
   connect(this->mpc_Ui->pc_SigPropertiesWidget, &C_SdBueSignalPropertiesWidget::SigRecheckError, this,
           &C_SdBueMessageSignalEditWidget::m_RecheckError);
   //Change
   connect(this->mpc_Ui->pc_MsgPropertiesWidget, &C_SdBueMessagePropertiesWidget::SigChanged, this,
           &C_SdBueMessageSignalEditWidget::m_OnChange);
   connect(this->mpc_Ui->pc_SigPropertiesWidget, &C_SdBueSignalPropertiesWidget::SigChanged, this,
           &C_SdBueMessageSignalEditWidget::m_OnChange);
   connect(this->mpc_Ui->pc_MsgPropertiesWidget, &C_SdBueMessagePropertiesWidget::SigMessageRxChanged, this,
           &C_SdBueMessageSignalEditWidget::SigMessageRxChanged);

   // MLV actions
   connect(this->mpc_Ui->pc_MsgLayoutViewerWidget, &C_SdBueMlvWidget::SigAddSignal, this,
           &C_SdBueMessageSignalEditWidget::SigAddSignal);
   connect(this->mpc_Ui->pc_MsgLayoutViewerWidget, &C_SdBueMlvWidget::SigAddSignalMultiplexed, this,
           &C_SdBueMessageSignalEditWidget::SigAddSignalMultiplexed);
   connect(this->mpc_Ui->pc_MsgLayoutViewerWidget, &C_SdBueMlvWidget::SigCopySignal, this,
           &C_SdBueMessageSignalEditWidget::SigCopySignal);
   connect(this->mpc_Ui->pc_MsgLayoutViewerWidget, &C_SdBueMlvWidget::SigCutSignal, this,
           &C_SdBueMessageSignalEditWidget::SigCutSignal);
   connect(this->mpc_Ui->pc_MsgLayoutViewerWidget, &C_SdBueMlvWidget::SigPasteSignal, this,
           &C_SdBueMessageSignalEditWidget::SigPasteSignal);
   connect(this->mpc_Ui->pc_MsgLayoutViewerWidget, &C_SdBueMlvWidget::SigDeleteSignal, this,
           &C_SdBueMessageSignalEditWidget::SigDeleteSignal);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   default destructor

   Clean up.
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdBueMessageSignalEditWidget::~C_SdBueMessageSignalEditWidget(void)
{
   //Store splitter position
   const QList<sintn> c_Sizes = this->mpc_Ui->pc_Splitter->sizes();

   if (c_Sizes.size() > 0)
   {
      const sintn sn_Size = c_Sizes.at(0);
      //Avoid saving invalid values
      if (sn_Size > 0)
      {
         C_UsHandler::h_GetInstance()->SetSdBusEditLayoutSplitterX(sn_Size);
      }
   }
   delete mpc_Ui;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set message sync manager

   \param[in,out]  opc_Value  Message sync manager
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::SetMessageSyncManager(
   stw_opensyde_gui_logic::C_PuiSdNodeCanMessageSyncManager * const opc_Value) const
{
   this->mpc_Ui->pc_MsgPropertiesWidget->SetMessageSyncManager(opc_Value);
   this->mpc_Ui->pc_SigPropertiesWidget->SetMessageSyncManager(opc_Value);
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SetMessageSyncManager(opc_Value);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set new com protocol

   \param[in]  ore_Value   New value
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::SetComProtocol(const stw_opensyde_core::C_OSCCanProtocol::E_Type & ore_Value) const
{
   this->mpc_Ui->pc_MsgPropertiesWidget->SetComProtocol(ore_Value);
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SetComProtocol(ore_Value);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Select message

   \param[in]  orc_MessageId  Message identification indices
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::SelectMessage(const C_OSCCanMessageIdentificationIndices & orc_MessageId) const
{
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SelectMessage(orc_MessageId);
   this->mpc_Ui->pc_MsgPropertiesWidget->setVisible(true);
   this->mpc_Ui->pc_SigPropertiesWidget->setVisible(false);
   this->mpc_Ui->pc_MessageLabel->setVisible(true);
   this->mpc_Ui->pc_SignalLabel->setVisible(false);
   this->mpc_Ui->pc_MsgPropertiesWidget->SetMessageId(true, orc_MessageId);
   //also initialize signal properties to have valid indices for everything besides signal index itself
   this->mpc_Ui->pc_SigPropertiesWidget->SetSignalId(orc_MessageId, 0U);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Select signal

   \param[in]  orc_MessageId        Message identification indices
   \param[in]  oru32_SignalIndex    Signal index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::SelectSignal(const C_OSCCanMessageIdentificationIndices & orc_MessageId,
                                                  const uint32 & oru32_SignalIndex) const
{
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SelectSignal(orc_MessageId, oru32_SignalIndex);
   this->mpc_Ui->pc_MsgPropertiesWidget->setVisible(false);
   this->mpc_Ui->pc_MsgPropertiesWidget->SetMessageId(false, C_OSCCanMessageIdentificationIndices());
   this->mpc_Ui->pc_SigPropertiesWidget->setVisible(true);
   this->mpc_Ui->pc_MessageLabel->setVisible(false);
   this->mpc_Ui->pc_SignalLabel->setVisible(true);
   this->mpc_Ui->pc_SigPropertiesWidget->SetSignalId(orc_MessageId, oru32_SignalIndex);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Hide
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::Hide(void)
{
   this->setVisible(false);
   this->mpc_Ui->pc_MsgPropertiesWidget->SetMessageId(false, C_OSCCanMessageIdentificationIndices());
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Sets the node 'mode' of the widget with all necessary indexes

   \param[in]  ou32_NodeIndex       Node index
   \param[in]  ou32_InterfaceIndex  Interface index
   \param[in]  orc_DatapoolIndexes  All Datapool indexes associated to the same protocol
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::SetNodeId(const stw_types::uint32 ou32_NodeIndex,
                                               const stw_types::uint32 ou32_InterfaceIndex,
                                               const std::vector<stw_types::uint32> & orc_DatapoolIndexes) const
{
   this->mpc_Ui->pc_MsgPropertiesWidget->SetNodeId(ou32_NodeIndex, ou32_InterfaceIndex, orc_DatapoolIndexes);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Sets the bus 'mode' of the widget

   \param[in]  ou32_BusIndex  Bus index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::SetBusId(const uint32 ou32_BusIndex) const
{
   this->mpc_Ui->pc_MsgPropertiesWidget->SetBusId(ou32_BusIndex);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Handle connection change
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::OnConnectionChange(void) const
{
   //Only relevant if visible
   if (this->mpc_Ui->pc_MsgPropertiesWidget->isVisible() == true)
   {
      this->mpc_Ui->pc_MsgPropertiesWidget->OnConnectionChange();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   In case of a disconnected node update of the unique message ids

   \param[in]  ou32_NodeIndex       Node index
   \param[in]  ou32_InterfaceIndex  Interface index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::OnNodeDisconnected(const uint32 ou32_NodeIndex, const uint32 ou32_InterfaceIndex)
{
   //Only relevant if visible
   if (this->mpc_Ui->pc_MsgPropertiesWidget->isVisible() == true)
   {
      this->mpc_Ui->pc_MsgPropertiesWidget->OnNodeDisconnected(ou32_NodeIndex, ou32_InterfaceIndex);
   }
   else if (this->mpc_Ui->pc_SigPropertiesWidget->isVisible() == true)
   {
      this->mpc_Ui->pc_SigPropertiesWidget->OnNodeDisconnected(ou32_NodeIndex, ou32_InterfaceIndex);
   }
   else
   {
      // Nothing to do
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   On change of signal count in message

   \param[in]  orc_MessageId  Message identification indices
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::OnSignalCountOfMessageChanged(
   const C_OSCCanMessageIdentificationIndices & orc_MessageId) const
{
   //Check if relevant
   bool q_Found = false;
   const std::vector<stw_opensyde_core::C_OSCCanMessageIdentificationIndices> c_MatchingMessageIds =
      this->mpc_Ui->pc_MsgPropertiesWidget->GetMatchingMessageIds();

   for (uint32 u32_ItMessage = 0; u32_ItMessage < c_MatchingMessageIds.size(); ++u32_ItMessage)
   {
      const stw_opensyde_core::C_OSCCanMessageIdentificationIndices & rc_MessageId =
         c_MatchingMessageIds[u32_ItMessage];
      if (rc_MessageId == orc_MessageId)
      {
         q_Found = true;
      }
   }
   if (q_Found == true)
   {
      this->mpc_Ui->pc_MsgLayoutViewerWidget->SelectMessage(orc_MessageId);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize all displayed static names
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::InitStaticNames(void) const
{
   this->mpc_Ui->pc_SignalLabel->setText(C_GtGetText::h_GetText("Signal Properties"));
   this->mpc_Ui->pc_MessageLabel->setText(C_GtGetText::h_GetText("Message Properties"));
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Selecting the name of the actual element
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::SelectName(void) const
{
   if (this->isVisible() == true)
   {
      if (this->mpc_Ui->pc_MsgPropertiesWidget->isVisible() == true)
      {
         this->mpc_Ui->pc_MsgPropertiesWidget->SelectName();
      }
      else if (this->mpc_Ui->pc_SigPropertiesWidget->isVisible() == true)
      {
         this->mpc_Ui->pc_SigPropertiesWidget->SelectName();
      }
      else
      {
         // nothing to do
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get last selection info

   \param[out]  orq_MessageSelected    Set flag if there is a selected message
   \param[out]  orc_MessageName        Selected message name if any
   \param[out]  orq_SignalSelected     Flag if signal selected
   \param[out]  orc_SignalName         Selected signal name if any
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::GetLastSelection(bool & orq_MessageSelected, QString & orc_MessageName,
                                                      bool & orq_SignalSelected, QString & orc_SignalName) const
{
   // do not use visibility of signal/message properties widget here, because they might be already invisible
   orq_MessageSelected = false;
   orq_SignalSelected = false;

   // check if message is selected (matching IDs vector is empty if signal is selected)
   const std::vector<C_OSCCanMessageIdentificationIndices> c_MatchingIds =
      this->mpc_Ui->pc_MsgPropertiesWidget->GetMatchingMessageIds();
   if (c_MatchingIds.size() > 0UL)
   {
      const C_OSCCanMessage * const pc_Message = C_PuiSdHandler::h_GetInstance()->GetCanMessage(c_MatchingIds[0]);
      if (pc_Message != NULL)
      {
         orc_MessageName = pc_Message->c_Name.c_str();
         orq_MessageSelected = true;
      }
   }

   // if no message is selected, check for signal selection
   if (orq_MessageSelected == false)
   {
      const C_OSCCanMessageIdentificationIndices c_Id = this->mpc_Ui->pc_SigPropertiesWidget->GetMessageId();
      const uint32 u32_SignalIndex = this->mpc_Ui->pc_SigPropertiesWidget->GetSignalIndex();
      const C_OSCCanMessage * const pc_Message = C_PuiSdHandler::h_GetInstance()->GetCanMessage(c_Id);
      const C_OSCNodeDataPoolListElement * const pc_Signal =
         C_PuiSdHandler::h_GetInstance()->GetOSCCanDataPoolListElement(c_Id, u32_SignalIndex);
      if ((pc_Message != NULL) && (pc_Signal != NULL))
      {
         orc_MessageName = pc_Message->c_Name.c_str();
         orc_SignalName = pc_Signal->c_Name.c_str();
         orq_MessageSelected = false;
         orq_SignalSelected = true;
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Reload colors
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::RefreshColors(void) const
{
   this->mpc_Ui->pc_MsgLayoutViewerWidget->RefreshColors();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Reconnect update signals for fields which signal changes
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::ConnectAllChanges(void) const
{
   this->mpc_Ui->pc_MsgPropertiesWidget->ConnectAllChanges();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Disconnect update signals for fields which signal changes
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::DisconnectAllChanges(void) const
{
   this->mpc_Ui->pc_MsgPropertiesWidget->DisconnectAllChanges();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten show event slot

   Here: Load splitter position

   \param[in,out]  opc_Event  Event identification and information
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::showEvent(QShowEvent * const opc_Event)
{
   const sint32 s32_FirstSegmentWidth = C_UsHandler::h_GetInstance()->GetSdBusEditLayoutSplitterX();

   this->mpc_Ui->pc_Splitter->SetFirstSegment(s32_FirstSegmentWidth);
   QWidget::showEvent(opc_Event);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Any message id is invalid

   \param[in]  orc_MessageId  Message id
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnMessageIdChange(const C_OSCCanMessageIdentificationIndices & orc_MessageId)
{
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SelectMessage(orc_MessageId);
   Q_EMIT (this->SigMessageIdChanged());
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   The specified message data was changed
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnMessageNameChange(void)
{
   Q_EMIT (this->SigMessageNameChanged());
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   The specified message data was changed

   \param[in]  orc_MessageId  Message id
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnMessageDlcChange(const C_OSCCanMessageIdentificationIndices & orc_MessageId)
const
{
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SelectMessage(orc_MessageId);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   On change of signal position in message

   \param[in]  orc_MessageId     Message identification indices
   \param[in]  ou32_SignalIndex  Index of signal of message
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnSignalUpdatedViaProperties(
   const C_OSCCanMessageIdentificationIndices & orc_MessageId, const uint32 ou32_SignalIndex) const
{
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SelectSignal(orc_MessageId, ou32_SignalIndex);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   On change of signal position in message
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnSignalUpdatedViaSelector(void)
{
   C_OSCCanMessageIdentificationIndices c_MessageId;

   //If current selected
   this->mpc_Ui->pc_SigPropertiesWidget->ReloadPosition();
   //If not current selected
   if (this->m_GetMessageId(c_MessageId) == C_NO_ERR)
   {
      Q_EMIT this->SigRecheckError(c_MessageId);
   }
   Q_EMIT this->SigChanged();
   Q_EMIT (this->SigSignalStartBitChanged(c_MessageId));
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   On change of active signal via selector widget

   \param[in]  ou32_SignalIndex  Active signal index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnSignalActivated(const uint32 ou32_SignalIndex)
{
   C_OSCCanMessageIdentificationIndices c_MessageId;

   if (m_GetMessageId(c_MessageId) == C_NO_ERR)
   {
      Q_EMIT this->SigSignalSelected(c_MessageId, ou32_SignalIndex);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   On change of signal name

   \param[in]  orc_MessageId  Message identification indices
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnSignalNameChanged(const C_OSCCanMessageIdentificationIndices & orc_MessageId)
{
   Q_EMIT this->SigSignalNameChanged(orc_MessageId);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   On change of signal start bit

   \param[in]  orc_MessageId  Message identification indices
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnSignalStartBitChanged(
   const C_OSCCanMessageIdentificationIndices & orc_MessageId)
{
   Q_EMIT (this->SigSignalStartBitChanged(orc_MessageId));
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Trigger global error check (icons only)
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_RecheckErrorGlobal(void)
{
   Q_EMIT this->SigRecheckErrorGlobal();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Trigger recheck of error values for tree

   \param[in]  orc_MessageId  Message identification indices
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_RecheckError(const C_OSCCanMessageIdentificationIndices & orc_MessageId)
{
   Q_EMIT this->SigRecheckError(orc_MessageId);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get current active message id

   \param[out]  orc_MessageId    Message identification indices

   \retval   C_NO_ERR   Message ID found
   \retval   C_RANGE    Message ID not found
*/
//----------------------------------------------------------------------------------------------------------------------
sint32 C_SdBueMessageSignalEditWidget::m_GetMessageId(C_OSCCanMessageIdentificationIndices & orc_MessageId) const
{
   sint32 s32_Retval = C_NO_ERR;

   if (this->mpc_Ui->pc_MsgPropertiesWidget->isVisible())
   {
      const std::vector<C_OSCCanMessageIdentificationIndices> c_MatchingMessageIds =
         this->mpc_Ui->pc_MsgPropertiesWidget->GetMatchingMessageIds();
      if (c_MatchingMessageIds.size() > 0)
      {
         orc_MessageId = c_MatchingMessageIds[0];
      }
      else
      {
         s32_Retval = C_RANGE;
      }
   }
   else
   {
      orc_MessageId = this->mpc_Ui->pc_SigPropertiesWidget->GetMessageId();
   }
   return s32_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Register any change
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnChange(void)
{
   Q_EMIT this->SigChanged();
}
