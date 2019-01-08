//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for editing messages and / or signals

   Widget for editing messages and / or signals

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     24.03.2017  STW/B.Bayer
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include "stwerrors.h"
#include "C_UsHandler.h"
#include "C_PuiSdHandler.h"
#include "C_SdBueMessageSignalEditWidget.h"
#include "ui_C_SdBueMessageSignalEditWidget.h"
#include "C_GtGetText.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_types;
using namespace stw_errors;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;
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

   Set up GUI with all elements.

   \param[in,out] opc_Parent Optional pointer to parent

   \created     24.03.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
C_SdBueMessageSignalEditWidget::C_SdBueMessageSignalEditWidget(QWidget * const opc_Parent) :
   QWidget(opc_Parent),
   mpc_Ui(new Ui::C_SdBueMessageSignalEditWidget)
{
   mpc_Ui->setupUi(this);
   this->mpc_Ui->pc_MsgPropertiesWidget->setVisible(false);
   this->mpc_Ui->pc_SignalLabel->setVisible(false);

   this->InitStaticNames();

   //Init splitter
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
   connect(this->mpc_Ui->pc_SigPropertiesWidget, &C_SdBueSignalPropertiesWidget::SigPositionChanged, this,
           &C_SdBueMessageSignalEditWidget::m_OnSignalUpdatedViaProperties);
   connect(this->mpc_Ui->pc_SigPropertiesWidget, &C_SdBueSignalPropertiesWidget::SigNameChanged, this,
           &C_SdBueMessageSignalEditWidget::m_OnSignalNameChanged);
   connect(this->mpc_Ui->pc_SigPropertiesWidget, &C_SdBueSignalPropertiesWidget::SigTypeChanged, this,
           &C_SdBueMessageSignalEditWidget::m_OnSignalTypeChanged);
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
   connect(this->mpc_Ui->pc_MsgLayoutViewerWidget, &C_SdBueMlvWidget::SigCopySignal, this,
           &C_SdBueMessageSignalEditWidget::SigCopySignal);
   connect(this->mpc_Ui->pc_MsgLayoutViewerWidget, &C_SdBueMlvWidget::SigCutSignal, this,
           &C_SdBueMessageSignalEditWidget::SigCutSignal);
   connect(this->mpc_Ui->pc_MsgLayoutViewerWidget, &C_SdBueMlvWidget::SigPasteSignal, this,
           &C_SdBueMessageSignalEditWidget::SigPasteSignal);
   connect(this->mpc_Ui->pc_MsgLayoutViewerWidget, &C_SdBueMlvWidget::SigDeleteSignal, this,
           &C_SdBueMessageSignalEditWidget::SigDeleteSignal);
}

//-----------------------------------------------------------------------------
/*!
   \brief   default destructor

   Clean up.

   \created     24.03.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
/*!
   \brief   Set message sync manager

   \param[in,out] opc_Value Message sync manager

   \created     25.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::SetMessageSyncManager(
   stw_opensyde_gui_logic::C_PuiSdNodeCanMessageSyncManager * const opc_Value) const
{
   this->mpc_Ui->pc_MsgPropertiesWidget->SetMessageSyncManager(opc_Value);
   this->mpc_Ui->pc_SigPropertiesWidget->SetMessageSyncManager(opc_Value);
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SetMessageSyncManager(opc_Value);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Set new com protocol

   \param[in] ore_Value New value

   \created     26.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::SetComProtocol(const stw_opensyde_core::C_OSCCanProtocol::E_Type & ore_Value) const
{
   this->mpc_Ui->pc_MsgPropertiesWidget->SetComProtocol(ore_Value);
   this->mpc_Ui->pc_SigPropertiesWidget->SetProtocol(ore_Value);
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SetComProtocol(ore_Value);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Select message

   \param[in] orc_MessageId Message identification indices

   \created     21.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::SelectMessage(const C_OSCCanMessageIdentificationIndices & orc_MessageId) const
{
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SelectMessage(orc_MessageId);
   this->mpc_Ui->pc_MsgPropertiesWidget->setVisible(true);
   this->mpc_Ui->pc_SigPropertiesWidget->setVisible(false);
   this->mpc_Ui->pc_MessageLabel->setVisible(true);
   this->mpc_Ui->pc_SignalLabel->setVisible(false);
   this->mpc_Ui->pc_MsgPropertiesWidget->SetMessageId(orc_MessageId);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Select signal

   \param[in] orc_MessageId     Message identification indices
   \param[in] oru32_SignalIndex Signal index

   \created     21.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::SelectSignal(const C_OSCCanMessageIdentificationIndices & orc_MessageId,
                                                  const uint32 & oru32_SignalIndex) const
{
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SelectSignal(orc_MessageId, oru32_SignalIndex);
   this->mpc_Ui->pc_MsgPropertiesWidget->setVisible(false);
   this->mpc_Ui->pc_SigPropertiesWidget->setVisible(true);
   this->mpc_Ui->pc_MessageLabel->setVisible(false);
   this->mpc_Ui->pc_SignalLabel->setVisible(true);
   this->mpc_Ui->pc_SigPropertiesWidget->SetSignalId(orc_MessageId, oru32_SignalIndex);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Sets the node 'mode' of the widget with all necessary indexes

   \param[in] ou32_NodeIndex      Node index
   \param[in] ou32_InterfaceIndex Interface index

   \created     25.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::SetNodeDataPool(const stw_types::uint32 ou32_NodeIndex,
                                                     const stw_types::uint32 ou32_InterfaceIndex) const
{
   this->mpc_Ui->pc_MsgPropertiesWidget->SetNodeDataPool(ou32_NodeIndex, ou32_InterfaceIndex);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Sets the bus 'mode' of the widget

   \param[in] ou32_BusIndex Bus index

   \created     25.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::SetBusId(const uint32 ou32_BusIndex) const
{
   this->mpc_Ui->pc_MsgPropertiesWidget->SetBusId(ou32_BusIndex);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Handle connection change

   \created     26.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::OnConnectionChange(void) const
{
   //Only relevant if visible
   if (this->mpc_Ui->pc_MsgPropertiesWidget->isVisible() == true)
   {
      this->mpc_Ui->pc_MsgPropertiesWidget->OnConnectionChange();
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   On change of signal count in message

   \param[in] orc_MessageId Message identification indices

   \created     26.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
/*!
   \brief   Initialize all displayed static names

   \created     04.05.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::InitStaticNames(void) const
{
   this->mpc_Ui->pc_SignalLabel->setText(C_GtGetText::h_GetText("Signal Properties"));
   this->mpc_Ui->pc_MessageLabel->setText(C_GtGetText::h_GetText("Message Properties"));
}

//-----------------------------------------------------------------------------
/*!
   \brief   Selecting the name of the actual element

   \created     08.06.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
/*!
   \brief   Get last selection info

   \param[out] orq_MessageSelected Set flag if there is a selected message
   \param[out] orc_MessageName     Selected message name if any
   \param[out] orq_SignalSelected  Flag if signal selected
   \param[out] orc_SignalName      Selected signal name if any

   \created     27.04.2018  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::GetLastSelection(bool & orq_MessageSelected, QString & orc_MessageName,
                                                      bool & orq_SignalSelected, QString & orc_SignalName) const
{
   orq_MessageSelected = this->mpc_Ui->pc_MsgPropertiesWidget->isVisible();
   orq_SignalSelected = this->mpc_Ui->pc_SigPropertiesWidget->isVisible();
   if (orq_MessageSelected == true)
   {
      const std::vector<C_OSCCanMessageIdentificationIndices> c_MatchingIds =
         this->mpc_Ui->pc_MsgPropertiesWidget->GetMatchingMessageIds();
      if (c_MatchingIds.size() > 0UL)
      {
         const C_OSCCanMessage * const pc_Message = C_PuiSdHandler::h_GetInstance()->GetCanMessage(c_MatchingIds[0]);
         if (pc_Message != NULL)
         {
            orc_MessageName = pc_Message->c_Name.c_str();
         }
      }
   }
   if (orq_SignalSelected == true)
   {
      const C_OSCCanMessageIdentificationIndices c_Id = this->mpc_Ui->pc_SigPropertiesWidget->GetMessageId();
      const uint32 u32_SignalIndex = this->mpc_Ui->pc_SigPropertiesWidget->GetSignalIndex();
      const C_OSCCanMessage * const pc_Message = C_PuiSdHandler::h_GetInstance()->GetCanMessage(c_Id);
      const C_OSCNodeDataPoolListElement * const pc_Signal =
         C_PuiSdHandler::h_GetInstance()->GetOSCCanDataPoolListElement(c_Id, u32_SignalIndex);
      if (pc_Message != NULL)
      {
         orc_MessageName = pc_Message->c_Name.c_str();
      }
      if (pc_Signal != NULL)
      {
         orc_SignalName = pc_Signal->c_Name.c_str();
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Overwritten show event slot

   Here: Load splitter position

   \param[in,out] opc_Event Event identification and information

   \created     22.09.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::showEvent(QShowEvent * const opc_Event)
{
   const sint32 s32_FirstSegmentWidth = C_UsHandler::h_GetInstance()->GetSdBusEditLayoutSplitterX();

   this->mpc_Ui->pc_Splitter->SetFirstSegment(s32_FirstSegmentWidth);
   QWidget::showEvent(opc_Event);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Any message id is invalid

   \created     25.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnMessageIdChange(const C_OSCCanMessageIdentificationIndices & orc_MessageId)
{
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SelectMessage(orc_MessageId);
   Q_EMIT this->SigMessageIdChanged();
}

//-----------------------------------------------------------------------------
/*!
   \brief   The specified message data was changed

   \created     25.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnMessageNameChange(void)
{
   Q_EMIT this->SigMessageNameChanged();
}

//-----------------------------------------------------------------------------
/*!
   \brief   The specified message data was changed

   \created     26.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnMessageDlcChange(const C_OSCCanMessageIdentificationIndices & orc_MessageId)
const
{
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SelectMessage(orc_MessageId);
}

//-----------------------------------------------------------------------------
/*!
   \brief   On change of signal position in message

   \param[in] orc_MessageId Message identification indices
   \param[in] ou32_SignalIndex   Index of signal of message

   \created     27.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnSignalUpdatedViaProperties(
   const C_OSCCanMessageIdentificationIndices & orc_MessageId, const uint32 ou32_SignalIndex) const
{
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SelectSignal(orc_MessageId, ou32_SignalIndex);
}

//-----------------------------------------------------------------------------
/*!
   \brief   On change of signal position in message

   \created     27.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
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
}

//-----------------------------------------------------------------------------
/*!
   \brief   On change of active signal via selector widget

   \param[in] ou32_SignalIndex Active signal index

   \created     27.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnSignalActivated(const uint32 ou32_SignalIndex)
{
   C_OSCCanMessageIdentificationIndices c_MessageId;

   if (m_GetMessageId(c_MessageId) == C_NO_ERR)
   {
      Q_EMIT this->SigSignalSelected(c_MessageId, ou32_SignalIndex);
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   On change of signal name

   \param[in] orc_MessageId      Message identification indices
   \param[in] ou32_SignalIndex   Index of signal of message

   \created     27.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnSignalNameChanged(const C_OSCCanMessageIdentificationIndices & orc_MessageId,
                                                           const uint32 ou32_SignalIndex)
{
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SelectSignal(orc_MessageId, ou32_SignalIndex);
   Q_EMIT this->SigSignalNameChanged(orc_MessageId);
}

//-----------------------------------------------------------------------------
/*!
   \brief   On change of signal name

   \param[in] orc_MessageId Message identification indices
   \param[in] ou32_SignalIndex   Index of signal of message

   \created     08.06.2018  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnSignalTypeChanged(const C_OSCCanMessageIdentificationIndices & orc_MessageId,
                                                           const uint32 ou32_SignalIndex)
const
{
   // Inform message layout viewer because of different behavior with float or normal types
   this->mpc_Ui->pc_MsgLayoutViewerWidget->SelectSignal(orc_MessageId, ou32_SignalIndex);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Trigger global error check (icons only)

   \created     28.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_RecheckErrorGlobal(void)
{
   Q_EMIT this->SigRecheckErrorGlobal();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Trigger recheck of error values for tree

   \param[in] orc_MessageId Message identification indices

   \created     28.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_RecheckError(const C_OSCCanMessageIdentificationIndices & orc_MessageId)
{
   Q_EMIT this->SigRecheckError(orc_MessageId);
}

//-----------------------------------------------------------------------------
/*!
   \brief   Get current active message id

   \param[out] orc_MessageId Message identification indices

   \created     02.05.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------
/*!
   \brief   Register any change

   \created     09.05.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueMessageSignalEditWidget::m_OnChange(void)
{
   Q_EMIT this->SigChanged();
}