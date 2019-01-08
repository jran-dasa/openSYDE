//-----------------------------------------------------------------------------
/*!
   \internal
   \file
   \brief       Bus protocol for specific node base class for connect and disconnect commands (implementation)

   detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     03.04.2017  STW/B.Bayer
   \endimplementation
*/
//-----------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------- */
#include "precomp_headers.h"

#include "stwerrors.h"

#include "C_SdBueUnoBusProtNodeConnectDisconnectBaseCommand.h"

#include "C_PuiSdHandler.h"
#include "C_OSCNode.h"
#include "C_OSCNodeDataPool.h"
#include "C_SdBueComIfDescriptionWidget.h"
#include "C_GtGetText.h"
#include "C_PuiSdUtil.h"

/* -- Used Namespaces ------------------------------------------------------ */
using namespace stw_types;
using namespace stw_errors;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_gui;
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

   \param[in]     ou32_NodeIndex      Node index
   \param[in]     ou32_InterfaceIndex Interface index
   \param[in]     oe_Protocol         Com protocol
   \param[in,out] opc_Widget          Widget to notify for changes
   \param[in]     orc_Text            Optional command text for informational display
   \param[in,out] opc_Parent          Optional pointer to parent

   \created     03.04.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
C_SdBueUnoBusProtNodeConnectDisconnectBaseCommand::C_SdBueUnoBusProtNodeConnectDisconnectBaseCommand(
   const uint32 ou32_NodeIndex, const uint32 ou32_InterfaceIndex, const C_OSCCanProtocol::E_Type oe_Protocol,
   QWidget * const opc_Widget, const QString & orc_Text, QUndoCommand * const opc_Parent) :
   QUndoCommand(orc_Text, opc_Parent),
   mu32_NodeIndex(ou32_NodeIndex),
   mu32_InterfaceIndex(ou32_InterfaceIndex),
   me_Protocol(oe_Protocol),
   mpc_Widget(opc_Widget)
{
}

//-----------------------------------------------------------------------------
/*!
   \brief   Connect node & interface to protocol

   \created     03.04.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_SdBueUnoBusProtNodeConnectDisconnectBaseCommand::m_DoConnectNodeToProt(void)
{
   this->m_SetComProtocolUsedByInterfaceFlag(true);
   m_UpdateUi();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Connect node & interface to protocol and create data pool

   \created     03.04.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_SdBueUnoBusProtNodeConnectDisconnectBaseCommand::m_DoConnectNodeToProtAndAddDataPool(void)
{
   if (this->m_AddDataPool() == C_NO_ERR)
   {
      this->m_SetComProtocolUsedByInterfaceFlag(true);
   }
   m_UpdateUi();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Disconnect node & interface from protocol

   \created     03.04.2017  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_SdBueUnoBusProtNodeConnectDisconnectBaseCommand::m_DoDisconnectNodeFromProt(void)
{
   this->m_SetComProtocolUsedByInterfaceFlag(false);
   m_UpdateUi();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Disconnect node & interface from protocol and delete data pool

   \created     21.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueUnoBusProtNodeConnectDisconnectBaseCommand::m_DoDisconnectNodeToProtAndDeleteDataPool(void)
{
   this->m_SetComProtocolUsedByInterfaceFlag(false);

   //Delete data pool
   this->m_DeleteDataPool();

   m_UpdateUi();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Connect node & interface to protocol and create data pool

   \created     14.05.2018  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_SdBueUnoBusProtNodeConnectDisconnectBaseCommand::m_DoAddDataPool(void)
{
   this->m_AddDataPool();

   m_UpdateUi();
}

//-----------------------------------------------------------------------------
/*!
   \brief   Disconnect node & interface from protocol and delete data pool

   \created     14.05.2018  STW/B.Bayer
*/
//-----------------------------------------------------------------------------
void C_SdBueUnoBusProtNodeConnectDisconnectBaseCommand::m_DoDeleteDataPool(void)
{
   this->m_DeleteDataPool();

   m_UpdateUi();
}

//-----------------------------------------------------------------------------
void C_SdBueUnoBusProtNodeConnectDisconnectBaseCommand::m_SetComProtocolUsedByInterfaceFlag(const bool oq_Flag) const
{
   C_OSCNode c_AdaptedNode = *C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);

   // get necessary datapool
   C_OSCCanProtocol * const pc_Protocol = c_AdaptedNode.GetCANProtocol(this->me_Protocol);

   if (pc_Protocol != NULL)
   {
      if (this->mu32_InterfaceIndex < pc_Protocol->c_ComMessages.size())
      {
         // set the flag
         pc_Protocol->c_ComMessages[this->mu32_InterfaceIndex].q_IsComProtocolUsedByInterface = oq_Flag;
         C_PuiSdHandler::h_GetInstance()->SetOSCNode(this->mu32_NodeIndex, c_AdaptedNode);
      }
   }
}

//-----------------------------------------------------------------------------
sint32 C_SdBueUnoBusProtNodeConnectDisconnectBaseCommand::m_AddDataPool(void) const
{
   C_OSCNodeDataPool c_NewDatapool;
   C_PuiSdNodeDataPool c_UIDataPool;
   sint32 s32_Return;
   QString c_Comment;
   QString c_ProtocolName = C_PuiSdUtil::h_ConvertProtocolTypeToString(this->me_Protocol);

   // add the new datapool
   c_NewDatapool.e_Type = C_OSCNodeDataPool::eCOM;

   c_Comment = C_GtGetText::h_GetText("Automatically generated Datapool for ");
   c_Comment += c_ProtocolName;
   c_Comment += C_GtGetText::h_GetText(" CAN communication");

   // special case layer 2 -> no spaces and no underscore number at the end
   c_ProtocolName = C_PuiSdUtil::h_ConvertProtocolTypeToDatapoolNameString(this->me_Protocol);

   c_NewDatapool.c_Name = C_PuiSdHandler::h_GetInstance()->GetUniqueDataPoolName(this->mu32_NodeIndex,
                                                                                 c_ProtocolName.toStdString().c_str());

   c_NewDatapool.c_Comment = c_Comment.toStdString().c_str();

   s32_Return = C_PuiSdHandler::h_GetInstance()->AddDataPool(this->mu32_NodeIndex, c_NewDatapool, c_UIDataPool,
                                                             this->me_Protocol);

   return s32_Return;
}

//-----------------------------------------------------------------------------
void C_SdBueUnoBusProtNodeConnectDisconnectBaseCommand::m_DeleteDataPool(void) const
{
   const C_OSCNode * pc_Node;

   //Delete data pool
   pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);
   if (pc_Node != NULL)
   {
      uint32 u32_ItComDataPool = 0;
      bool q_Found = false;
      //Match com data pool to com protocol type
      for (uint32 u32_ItDataPool = 0; (u32_ItDataPool < pc_Node->c_DataPools.size()) && (q_Found == false);
           ++u32_ItDataPool)
      {
         const C_OSCNodeDataPool & orc_DataPool = pc_Node->c_DataPools[u32_ItDataPool];
         if (orc_DataPool.e_Type == C_OSCNodeDataPool::eCOM)
         {
            if (u32_ItComDataPool < pc_Node->c_ComProtocols.size())
            {
               const C_OSCCanProtocol & orc_Protocol = pc_Node->c_ComProtocols[u32_ItComDataPool];
               if (orc_Protocol.e_Type == this->me_Protocol)
               {
                  //Everything matches
                  C_PuiSdHandler::h_GetInstance()->RemoveDataPool(this->mu32_NodeIndex, u32_ItDataPool);
                  q_Found = true;
               }
            }
            ++u32_ItComDataPool;
         }
      }
   }
}

//-----------------------------------------------------------------------------
/*!
   \brief   Notify ui

   \created     21.04.2017  STW/M.Echtler
*/
//-----------------------------------------------------------------------------
void C_SdBueUnoBusProtNodeConnectDisconnectBaseCommand::m_UpdateUi(void)
{
   //lint -e{929}  false positive in PC-Lint: allowed by MISRA 5-2-2
   C_SdBueComIfDescriptionWidget * const pc_Widget =
      dynamic_cast<C_SdBueComIfDescriptionWidget * const>(this->mpc_Widget);

   if (pc_Widget != NULL)
   {
      pc_Widget->PartialReload();
   }
}