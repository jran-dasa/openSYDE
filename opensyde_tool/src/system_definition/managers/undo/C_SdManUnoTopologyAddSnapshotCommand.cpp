//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Add new items based on snapshot information (implementation)

   Add new items based on snapshot information

   \copyright   Copyright 2016 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include "stwtypes.h"
#include "C_PuiSdUtil.h"
#include "C_SdTopologyScene.h"
#include "C_SdManUnoTopologyAddSnapshotCommand.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_core;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   \param[in,out]  opc_Scene                 Pointer to currently active scene
   \param[in]      orc_InitialSnapshotData   Initial snapshot data
   \param[in]      orc_IDs                   Affected unique IDs
   \param[in]      orc_NewPos                New position
   \param[in]      of64_HighestUsedZValue    Highest used Z value
   \param[in,out]  opc_Parent                Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdManUnoTopologyAddSnapshotCommand::C_SdManUnoTopologyAddSnapshotCommand(QGraphicsScene * const opc_Scene,
                                                                           const C_SdTopologyDataSnapshot & orc_InitialSnapshotData,
                                                                           const std::vector<uint64> & orc_IDs, const QPointF & orc_NewPos, const float64 of64_HighestUsedZValue,
                                                                           QUndoCommand * const opc_Parent) :
   C_SdManUnoTopologyAddBaseCommand(opc_Scene, orc_IDs, "Paste drawing element(s)",
                                    opc_Parent, orc_InitialSnapshotData)
{
   //Handle pos
   this->m_SetDataPositionOffset(orc_NewPos);
   //Handle Z
   this->m_SetDataZOffset(of64_HighestUsedZValue);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default destructor
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdManUnoTopologyAddSnapshotCommand::~C_SdManUnoTopologyAddSnapshotCommand(void)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Add elements without previous knowledge
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdManUnoTopologyAddSnapshotCommand::m_AddNew(void)
{
   C_SdTopologyScene * const pc_Scene = dynamic_cast<C_SdTopologyScene * const>(mpc_Scene);

   if (pc_Scene != NULL)
   {
      QMap<C_PuiBsTemporaryDataID, uint64> c_IDMap;
      uint32 u32_ItID = 0;
      const C_SdTopologyDataSnapshot c_InitialData = this->GetDataBackup();
      const std::vector<stw_types::uint64> c_AllIDs = this->m_GetIDs();
      const uint32 u32_ItemCount = c_InitialData.Count();
      if (u32_ItemCount <= c_AllIDs.size())
      {
         //Map IDs
         uint32 u32_Counter = 0UL;

         //Nodes
         for (uint32 u32_ItItem = 0; u32_ItItem < c_InitialData.c_UINodes.size(); ++u32_ItItem)
         {
            const bool q_IsFirst = C_PuiSdUtil::h_CheckIsFirstInAnyGroupOrNotInAny(u32_ItItem,
                                                                                   c_InitialData.c_OSCNodeGroups);
            if (q_IsFirst)
            {
               c_IDMap.insert(C_PuiBsTemporaryDataID(static_cast<sint32>(C_PuiSdDataElement::eNODE),
                                                     u32_Counter), c_AllIDs[u32_ItID]);
               ++u32_ItID;
               ++u32_Counter;
            }
         }
         //Buses
         for (uint32 u32_ItItem = 0; u32_ItItem < c_InitialData.c_OSCBuses.size(); ++u32_ItItem)
         {
            c_IDMap.insert(C_PuiBsTemporaryDataID(static_cast<sint32>(C_PuiSdDataElement::eBUS),
                                                  u32_ItItem), c_AllIDs[u32_ItID]);
            ++u32_ItID;
         }
         //Boundaries
         for (uint32 u32_ItItem = 0; u32_ItItem < c_InitialData.c_Boundaries.size(); ++u32_ItItem)
         {
            c_IDMap.insert(C_PuiBsTemporaryDataID(static_cast<sint32>(C_PuiSdDataElement::eBOUNDARY),
                                                  u32_ItItem),
                           c_AllIDs[u32_ItID]);
            ++u32_ItID;
         }
         //Images
         for (uint32 u32_ItItem = 0; u32_ItItem < c_InitialData.c_Images.size(); ++u32_ItItem)
         {
            c_IDMap.insert(C_PuiBsTemporaryDataID(static_cast<sint32>(C_PuiSdDataElement::eIMAGE),
                                                  u32_ItItem), c_AllIDs[u32_ItID]);
            ++u32_ItID;
         }
         //Line arrows
         for (uint32 u32_ItItem = 0; u32_ItItem < c_InitialData.c_LineArrows.size(); ++u32_ItItem)
         {
            c_IDMap.insert(C_PuiBsTemporaryDataID(static_cast<sint32>(C_PuiSdDataElement::eLINE_ARROW),
                                                  u32_ItItem),
                           c_AllIDs[u32_ItID]);
            ++u32_ItID;
         }
         //Text elements
         for (uint32 u32_ItItem = 0; u32_ItItem < c_InitialData.c_TextElements.size(); ++u32_ItItem)
         {
            c_IDMap.insert(C_PuiBsTemporaryDataID(static_cast<sint32>(C_PuiSdDataElement::eTEXT_ELEMENT),
                                                  u32_ItItem),
                           c_AllIDs[u32_ItID]);
            ++u32_ItID;
         }
         //Bus connector
         for (uint32 u32_ItItem = 0; u32_ItItem < c_InitialData.c_BusConnections.size(); ++u32_ItItem)
         {
            c_IDMap.insert(C_PuiBsTemporaryDataID(static_cast<sint32>(C_PuiSdDataElement::eBUS_CONNECTOR),
                                                  u32_ItItem), c_AllIDs[u32_ItID]);
            ++u32_ItID;
         }
         //Bus text elements
         for (uint32 u32_ItItem = 0; u32_ItItem < c_InitialData.c_BusTextElements.size(); ++u32_ItItem)
         {
            c_IDMap.insert(C_PuiBsTemporaryDataID(static_cast<sint32>(C_PuiSdDataElement::eTEXT_ELEMENT_BUS),
                                                  u32_ItItem), c_AllIDs[u32_ItID]);
            ++u32_ItID;
         }
         pc_Scene->CopyFromSnapshotToScene(c_InitialData, &c_IDMap);
      }
   }
}
