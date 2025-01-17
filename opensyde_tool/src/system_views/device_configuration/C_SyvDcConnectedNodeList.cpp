//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       List for all connected node entries (implementation)

   List for all connected node entries

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include <QDrag>
#include <QMimeData>
#include <QScrollBar>
#include "constants.h"
#include "TGLUtils.h"
#include "C_OgeWiUtil.h"
#include "C_PuiSdHandler.h"
#include "C_PuiSvHandler.h"
#include "C_SyvDcConnectedNodeList.h"
#include "C_SyvDcConnectedNodeWidget.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_core;
using namespace stw_opensyde_gui_logic;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */
const QString C_SyvDcConnectedNodeList::mhc_MimeData = "stw_opensyde_connected_node";
const QString C_SyvDcConnectedNodeList::mhc_MimeDataExtFormat = "stw_opensyde_connected_node_ext_format";
const QString C_SyvDcConnectedNodeList::mhc_MimeDataManufacturerFormat = "stw_opensyde_connected_node_manu_format";
const QString C_SyvDcConnectedNodeList::mhc_MimeDataDevice = "stw_opensyde_connected_node_device";
const QString C_SyvDcConnectedNodeList::mhc_MimeDataDeviceValid = "stw_opensyde_connected_node_device_valid";
const QString C_SyvDcConnectedNodeList::mhc_MimeDataSubNodeIdsToOldNodeIds =
   "stw_opensyde_connected_node_subnodeids_to_nodeids";

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in,out] opc_Parent Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvDcConnectedNodeList::C_SyvDcConnectedNodeList(QWidget * const opc_Parent) :
   QListWidget(opc_Parent),
   mq_GridSizeSet(false)
{
   //UI Settings
   this->setEditTriggers(QAbstractItemView::NoEditTriggers);
   this->setDragEnabled(true);
   this->setDragDropMode(QAbstractItemView::DragOnly);
   this->setDefaultDropAction(Qt::TargetMoveAction);
   this->setAlternatingRowColors(true);
   this->setSelectionMode(QAbstractItemView::SingleSelection);
   this->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
   this->setAcceptDrops(false);
   this->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

   // configure the scrollbar to stop resizing the widget when showing or hiding the scrollbar
   this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   this->verticalScrollBar()->hide();

   // Deactivate custom context menu of scroll bar
   this->verticalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);
   this->horizontalScrollBar()->setContextMenuPolicy(Qt::NoContextMenu);

   //Connects
   connect(this->verticalScrollBar(), &QScrollBar::rangeChanged, this,
           &C_SyvDcConnectedNodeList::m_ScrollBarRangeChangedVer);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set data

   \param[in] orc_Infos Data
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvDcConnectedNodeList::SetData(const std::vector<C_SyvDcDeviceInformation> & orc_Infos)
{
   this->mc_Data = orc_Infos;
   m_Init();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Enable serial number

   \param[in] orc_SerialNumber       Serial number
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvDcConnectedNodeList::EnableSerialNumber(const C_OSCProtocolSerialNumber & orc_SerialNumber) const
{
   for (sintn sn_It = 0; sn_It < this->count(); ++sn_It)
   {
      C_SyvDcConnectedNodeWidget * const pc_Widget =
         dynamic_cast<C_SyvDcConnectedNodeWidget * const>(this->itemWidget(this->item(sn_It)));
      if ((pc_Widget != NULL) &&
          (pc_Widget->CompareSerialNumber(orc_SerialNumber) == true))
      {
         pc_Widget->setEnabled(true);
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Disable serial number

   \param[in] orc_SerialNumber       Serial number
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvDcConnectedNodeList::DisableSerialNumber(const stw_opensyde_core::C_OSCProtocolSerialNumber & orc_SerialNumber)
const
{
   for (sintn sn_It = 0; sn_It < this->count(); ++sn_It)
   {
      C_SyvDcConnectedNodeWidget * const pc_Widget =
         dynamic_cast<C_SyvDcConnectedNodeWidget * const>(this->itemWidget(this->item(sn_It)));
      if ((pc_Widget != NULL) &&
          (pc_Widget->CompareSerialNumber(orc_SerialNumber) == true))
      {
         pc_Widget->setEnabled(false);
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten start drag event slot

   Here: Start manual drag

   \param[in,out]  oc_Actions  Drop actions type
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvDcConnectedNodeList::startDrag(const Qt::DropActions oc_Actions)
{
   QListWidgetItem * const pc_Item = this->currentItem();

   C_SyvDcConnectedNodeWidget * const pc_Widget =
      dynamic_cast<C_SyvDcConnectedNodeWidget * const>(this->itemWidget(pc_Item));

   if ((pc_Widget != NULL) && (pc_Widget->isEnabled() == true))
   {
      const QRect c_Rect = pc_Widget->rect();
      QList<QListWidgetItem *> c_Items;
      QPixmap c_Screenshot(c_Rect.size());
      //Manual drag
      QDrag * const pc_Drag = new QDrag(this);
      QRegion c_Region;

      Q_EMIT (this->SigStartDrag(pc_Widget->GetDeviceName(), pc_Widget->GetDeviceNameValid()));

      // create the screenshot
      c_Region.setRects(&c_Rect, 1);
      pc_Widget->render(&c_Screenshot, QPoint(), c_Region);

      pc_Drag->setPixmap(c_Screenshot);

      c_Items.push_back(pc_Item);
      pc_Drag->setMimeData(this->mimeData(c_Items));
      pc_Drag->exec(oc_Actions, this->defaultDropAction());

      // pc_Drag->exec will start its own event loop. It will return when the mouse button is released.
      // No Qt event or signal will detect this on drop exit on one common position
      // That is the only solution to handle a drag exit always.
      Q_EMIT (this->SigStopDrag());
   } //lint !e429  //no memory leak because of the parent of pc_Drag and the Qt memory management
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten mime data request event slot

   Here: Add additional information

   \param[in] oc_Items Affected items

   \return
   Mime data for drag event
*/
//----------------------------------------------------------------------------------------------------------------------
QMimeData * C_SyvDcConnectedNodeList::mimeData(const QList<QListWidgetItem *> oc_Items) const
{
   QMimeData * const pc_Retval = QListWidget::mimeData(oc_Items);

   if (pc_Retval != NULL)
   {
      if (oc_Items.size() > 0)
      {
         C_SyvDcConnectedNodeWidget * const pc_Widget =
            dynamic_cast<C_SyvDcConnectedNodeWidget * const>(this->itemWidget(oc_Items[0]));
         if (pc_Widget != NULL)
         {
            QString c_StringSubNodeIdsToOldNodeIds;
            const std::map<uint8, C_SyvDcDeviceOldComConfig> c_SubNodeIdsToOldNodeIds =
               pc_Widget->GetSubNodeIdsToOldNodeIds();
            std::map<stw_types::uint8, C_SyvDcDeviceOldComConfig>::const_iterator c_ItIds;

            pc_Retval->setData(C_SyvDcConnectedNodeList::mhc_MimeData,
                               pc_Widget->GetPlainSerialNumberString().toStdString().c_str());
            pc_Retval->setData(C_SyvDcConnectedNodeList::mhc_MimeDataExtFormat,
                               QString::number(static_cast<sintn>(pc_Widget->GetExtFormat())).
                               toStdString().c_str());
            pc_Retval->setData(C_SyvDcConnectedNodeList::mhc_MimeDataManufacturerFormat,
                               QString::number(static_cast<sintn>(pc_Widget->GetManufacturerFormat())).
                               toStdString().c_str());
            pc_Retval->setData(C_SyvDcConnectedNodeList::mhc_MimeDataDevice,
                               pc_Widget->GetDeviceName().toStdString().c_str());
            pc_Retval->setData(C_SyvDcConnectedNodeList::mhc_MimeDataDeviceValid,
                               QString::number(static_cast<sintn>(pc_Widget->GetDeviceNameValid())).
                               toStdString().c_str());

            // Build a string to set the sub node id to node id mapping
            for (c_ItIds = c_SubNodeIdsToOldNodeIds.begin(); c_ItIds != c_SubNodeIdsToOldNodeIds.end(); ++c_ItIds)
            {
               // Format: sub node id,node id, ip valid flag(true), ip0:ip1:ip2:ip3;
               // sub node id, node id, ip valid flag(false);
               c_StringSubNodeIdsToOldNodeIds +=
                  QString::number(c_ItIds->first) + "," +
                  QString::number(c_ItIds->second.u8_OldNodeId) + "," +
                  QString::number(static_cast<uint8>(c_ItIds->second.q_OldIpAddressValid));

               if (c_ItIds->second.q_OldIpAddressValid == true)
               {
                  c_StringSubNodeIdsToOldNodeIds += ",";
                  c_StringSubNodeIdsToOldNodeIds += QString::number(c_ItIds->second.au8_OldIpAddress[0]) + ":" +
                                                    QString::number(c_ItIds->second.au8_OldIpAddress[1]) + ":" +
                                                    QString::number(c_ItIds->second.au8_OldIpAddress[2]) + ":" +
                                                    QString::number(c_ItIds->second.au8_OldIpAddress[3]);
               }
               c_StringSubNodeIdsToOldNodeIds += ";";
            }
            pc_Retval->setData(C_SyvDcConnectedNodeList::mhc_MimeDataSubNodeIdsToOldNodeIds,
                               c_StringSubNodeIdsToOldNodeIds.toStdString().c_str());
         }
      }
   }
   return pc_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Init step
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvDcConnectedNodeList::m_Init(void)
{
   uint32 u32_ItData;

   std::vector<C_SyvDcDeviceInformation> c_DataUnique;
   std::vector<std::map<uint8, C_SyvDcDeviceOldComConfig> > c_DataUniqueSubNodeIdsToOldNodeIds;

   //Init/Reinit UI
   this->clear();

   c_DataUnique.reserve(this->mc_Data.size());
   c_DataUniqueSubNodeIdsToOldNodeIds.reserve(this->mc_Data.size());

   // Check for Multi-CPU nodes
   for (u32_ItData = 0U; u32_ItData < this->mc_Data.size(); ++u32_ItData)
   {
      const C_SyvDcDeviceInformation & rc_Data = this->mc_Data[u32_ItData];
      uint32 u32_ItDataUnique;
      bool q_MatchingSubNodeFound = false;

      // Check if already existing
      for (u32_ItDataUnique = 0U; u32_ItDataUnique < c_DataUnique.size(); ++u32_ItDataUnique)
      {
         const C_SyvDcDeviceInformation & rc_DataUnique = c_DataUnique[u32_ItDataUnique];

         if (rc_DataUnique.IsSerialNumberIdentical(rc_Data) == true)
         {
            // Serial number is identical, add the sub node id
            C_SyvDcDeviceOldComConfig c_OldComConfig;
            c_OldComConfig.SetContent(rc_Data.u8_NodeId, rc_Data.q_IpAddressValid, &rc_Data.au8_IpAddress[0]);

            c_DataUniqueSubNodeIdsToOldNodeIds[u32_ItDataUnique][rc_Data.u8_SubNodeId] = c_OldComConfig;
            q_MatchingSubNodeFound = true;
            break;
         }
      }

      if (q_MatchingSubNodeFound == false)
      {
         std::map<uint8, C_SyvDcDeviceOldComConfig> c_SubNodeIdToNodeId;
         C_SyvDcDeviceOldComConfig c_OldComConfig;
         c_OldComConfig.SetContent(rc_Data.u8_NodeId, rc_Data.q_IpAddressValid, &rc_Data.au8_IpAddress[0]);

         c_SubNodeIdToNodeId[rc_Data.u8_SubNodeId] = c_OldComConfig;

         // New serial number
         c_DataUnique.push_back(rc_Data);
         c_DataUniqueSubNodeIdsToOldNodeIds.push_back(c_SubNodeIdToNodeId);
      }
   }

   tgl_assert(c_DataUnique.size() == c_DataUniqueSubNodeIdsToOldNodeIds.size());

   //No point if pc not connected
   for (u32_ItData = 0U; u32_ItData < c_DataUnique.size(); ++u32_ItData)
   {
      m_AppendNode(c_DataUnique[u32_ItData], c_DataUniqueSubNodeIdsToOldNodeIds[u32_ItData]);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Append node to list

   \param[in] orc_Info                    Read device information
   \param[in] orc_SubNodeIdsToOldNodeIds  Detected sub node ids and the associated used node ids with same serial number
                                          - In case of a normal node, exact one sub node id which should be 0
                                          - In case of a multiple CPU, at least two sub node ids
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvDcConnectedNodeList::m_AppendNode(const C_SyvDcDeviceInformation & orc_Info, const std::map<uint8,
                                                                                                      C_SyvDcDeviceOldComConfig> & orc_SubNodeIdsToOldNodeIds)
{
   C_SyvDcConnectedNodeWidget * pc_Widget;

   this->addItem("");

   pc_Widget = new C_SyvDcConnectedNodeWidget(this->item(this->count() - 1), orc_Info,
                                              orc_SubNodeIdsToOldNodeIds, this);

   this->setItemWidget(this->item(this->count() - 1), pc_Widget);

   //Style first
   C_OgeWiUtil::h_ApplyStylesheetProperty(pc_Widget, "First", this->count() == 1);

   if (this->mq_GridSizeSet == false)
   {
      // Set the grid size for one time. The size hint seems not to be enough since Qt 5.15.2
      this->mq_GridSizeSet = true;
      this->setGridSize(QSize(pc_Widget->width(), pc_Widget->height()));
   }
}

//----------------------------------------------------------------------------------------------------------------------
void C_SyvDcConnectedNodeList::m_ScrollBarRangeChangedVer(const sintn osn_Min, const sintn osn_Max) const
{
   // manual showing and hiding of the scrollbar to stop resizing the parent widget when showing or hiding the scrollbar
   if ((osn_Min == 0) && (osn_Max == 0))
   {
      this->verticalScrollBar()->hide();
   }
   else
   {
      this->verticalScrollBar()->show();
   }
}
