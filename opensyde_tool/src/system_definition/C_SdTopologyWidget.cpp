//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for system definition topology

   \copyright   Copyright 2016 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include <QScreen>
#include <QMimeData>
#include <QGraphicsItem>
#include <QDesktopWidget>
#include <QGuiApplication>

#include "stwtypes.h"

#include "C_SdTopologyWidget.h"
#include "ui_C_SdTopologyWidget.h"

#include "C_SebUtil.h"
#include "C_SebGraphicsView.h"
#include "C_HeHandler.h"
#include "C_SdTopologyToolbox.h"
#include "C_GtGetText.h"
#include "C_UsHandler.h"
#include "C_OgeWiUtil.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */

using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_gui_elements;
using namespace stw_types;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */
const sintn C_SdTopologyWidget::mhsn_WidgetBorder = 12;
const sintn C_SdTopologyWidget::mhsn_ToolboxInitPosY = 150;

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
C_SdTopologyWidget::C_SdTopologyWidget(QWidget * const opc_Parent) :
   QWidget(opc_Parent),
   mpc_Ui(new Ui::C_SdTopologyWidget),
   mpc_Toolbox(NULL),
   mpc_FixMinimizedToolbox(NULL)
{
   sintn sn_ViewZoomValue;

   mpc_Ui->setupUi(this);

   // create scene for graphics view
   this->mpc_Scene = new C_SdTopologyScene(true);

   // configure scene
   this->mpc_Scene->setSceneRect(0.0, 0.0,
                                 static_cast<float64>(this->mpc_Ui->pc_GraphicsView->width()),
                                 static_cast<float64>(this->mpc_Ui->pc_GraphicsView->height()));
   this->mpc_Ui->pc_GraphicsView->SetSceneAndConnect(this->mpc_Scene);

   C_OgeWiUtil::h_ApplyStylesheetProperty(this->mpc_Ui->pc_DrawFrame, "OpenRight", true);

   // make all generic connects
   //Connect for tool tip
   connect(this->mpc_Scene, &C_SebScene::SigShowToolTip, this->mpc_Ui->pc_GraphicsView,
           &C_SebGraphicsView::ShowToolTip);
   connect(this->mpc_Scene, &C_SebScene::SigHideToolTip, this->mpc_Ui->pc_GraphicsView,
           &C_SebGraphicsView::HideToolTip);
   connect(this->mpc_Ui->pc_GraphicsView, &C_SebGraphicsView::SigShowToolTip, this->mpc_Scene,
           &C_SebScene::DisplayToolTip);
   //Interaction point scaling
   connect(this->mpc_Scene, &C_SebScene::SigTriggerUpdateTransform, this->mpc_Ui->pc_GraphicsView,
           &C_SebGraphicsView::UpdateTransform);
   // forwarding of this signal
   connect(this->mpc_Scene, &C_SdTopologyScene::SigChangeMode, this, &C_SdTopologyWidget::SigChangeMode);
   connect(this->mpc_Scene, &C_SdTopologyScene::SigChanged, this, &C_SdTopologyWidget::SigChanged);
   connect(this->mpc_Scene, &C_SdTopologyScene::SigNodeDeleted, this, &C_SdTopologyWidget::SigNodeDeleted);
   connect(this->mpc_Scene, &C_SdTopologyScene::SigBusDeleted, this, &C_SdTopologyWidget::SigBusDeleted);
   connect(this->mpc_Scene, &C_SdTopologyScene::SigNodeChanged, this, &C_SdTopologyWidget::SigNodeChanged);
   connect(this->mpc_Scene, &C_SdTopologyScene::SigBusChanged, this, &C_SdTopologyWidget::SigBusChanged);
   connect(this->mpc_Scene, &C_SdTopologyScene::SigErrorChange, this,
           &C_SdTopologyWidget::SigErrorChange);

   // restore configuration of the view
   sn_ViewZoomValue = C_UsHandler::h_GetInstance()->GetProjSdTopologyViewZoom();
   // only change the zoom if the value is not default
   this->mpc_Ui->pc_GraphicsView->SetZoomValue(sn_ViewZoomValue, true);
   this->mpc_Ui->pc_GraphicsView->SetViewPos(C_UsHandler::h_GetInstance()->GetProjSdTopologyViewPos());
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   default destructor

   Clean up.
*/
//----------------------------------------------------------------------------------------------------------------------
//lint -e{1540} Toolbox cleaned up via Qt parent mechanism
C_SdTopologyWidget::~C_SdTopologyWidget()
{
   // store configuration of the view
   C_UsHandler::h_GetInstance()->SetProjSdTopologyViewPos(this->mpc_Ui->pc_GraphicsView->GetViewPos());
   C_UsHandler::h_GetInstance()->SetProjSdTopologyViewZoom(this->mpc_Ui->pc_GraphicsView->GetZoomValue());

   delete mpc_Ui;
   delete mpc_Scene;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Function to set the parent of the widget

   The toolbox will be placed on the parent widget.

   \param[in,out]  opc_Parent    Pointer to toolbox
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdTopologyWidget::SetParentHook(QWidget * const opc_Parent)
{
   if (opc_Parent != NULL)
   {
      // create toolbox
      C_SdTopologyToolbox * pc_TopologyToolboxWidget;

      pc_TopologyToolboxWidget = new C_SdTopologyToolbox();
      this->mpc_Toolbox =
         new C_OgeWiHover(*pc_TopologyToolboxWidget, C_GtGetText::h_GetText("TOOLBOX"), ":images/IconToolbox.svg",
                          true, opc_Parent, opc_Parent);

      // create fix minimized toolbox
      this->mpc_FixMinimizedToolbox = new C_OgeWiFixPosition(C_GtGetText::h_GetText("TOOLBOX"),
                                                             ":images/IconToolbox.svg",
                                                             QRect(1449, 14, 190, 36), opc_Parent);
      // check for saved default values for toolbox
      if (C_UsHandler::h_GetInstance()->GetSdTopologyToolboxPos().x() < 0)
      {
         // default value
         const sintn sn_ToolboxWidth = 355;
         const sintn sn_ToolboxHeight = 328;
         // needed to make the toolbox stay inside graphics view (maybe depends on size of navibar)
         const sintn sn_ToolboxOffset = 308;
         // use available desktop space to have real information about screen (widget sizes are not reliable here)
         {
            sintn sn_MaxWidth = 0;
            const QList<QScreen *> c_Desktop = QGuiApplication::screens();
            for (const QScreen * const pc_It : c_Desktop)
            {
               sn_MaxWidth = std::max(sn_MaxWidth, pc_It->availableGeometry().width());
            }
            this->mpc_Toolbox->setGeometry(((sn_MaxWidth - sn_ToolboxWidth) -
                                            mhsn_WidgetBorder) - sn_ToolboxOffset,
                                           mhsn_ToolboxInitPosY, sn_ToolboxWidth, sn_ToolboxHeight);
         }
         this->mpc_Toolbox->SetMaximizedHeight(sn_ToolboxHeight);
      }
      else
      {
         // restore last position and size
         this->mpc_Toolbox->setGeometry(QRect(C_UsHandler::h_GetInstance()->GetSdTopologyToolboxPos(),
                                              C_UsHandler::h_GetInstance()->GetSdTopologyToolboxSize()));
         this->mpc_Toolbox->SetMaximizedHeight(C_UsHandler::h_GetInstance()->GetSdTopologyToolboxSize().height());
      }

      if (C_UsHandler::h_GetInstance()->GetSdTopologyToolboxMaximized() == false)
      {
         this->m_WiHoverMinBtnClicked();
      }
      else
      {
         this->mpc_Toolbox->show();
      }

      connect(this->mpc_Toolbox, &C_OgeWiHover::SigSearchChanged, pc_TopologyToolboxWidget,
              &C_SdTopologyToolbox::SearchChanged);
      connect(this->mpc_Toolbox, &C_OgeWiHover::SigWiHoverMinBtnClicked,
              this, &C_SdTopologyWidget::m_WiHoverMinBtnClicked);
      connect(this->mpc_FixMinimizedToolbox, &C_OgeWiFixPosition::SigWiFixPosMaxBtnClicked,
              this, &C_SdTopologyWidget::m_WiFixPosMaxBtnClicked);
   } //lint !e429  //no memory leak because of the parent of pc_Drag and the Qt memory management
   else
   {
      QSize c_Size;
      // save position, size and state of toolbox
      C_UsHandler::h_GetInstance()->SetSdTopologyToolboxPos(this->mpc_Toolbox->pos());
      c_Size.setWidth(this->mpc_Toolbox->width());
      c_Size.setHeight(this->mpc_Toolbox->GetMaximizedHeight());
      C_UsHandler::h_GetInstance()->SetSdTopologyToolboxSize(c_Size);
      C_UsHandler::h_GetInstance()->SetSdTopologyToolboxMaximized(this->mpc_Toolbox->GetMaximized());

      // reset parent for memory management
      this->mpc_Toolbox->setParent(this);
      this->mpc_FixMinimizedToolbox->setParent(this);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Function for toolbar

   Saves the project.
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdTopologyWidget::SaveToData(void) const
{
   this->mpc_Scene->Save();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Hide or show the entire widget with the toolbox

   \param[in]  oq_Hide  Flag for hiding or showing
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdTopologyWidget::HideAll(const bool oq_Hide)
{
   if (oq_Hide == true)
   {
      this->hide();
      this->mpc_Toolbox->hide();
      this->mpc_FixMinimizedToolbox->hide();
   }
   else
   {
      this->show();
      if (this->mpc_Toolbox->GetMaximized() != false)
      {
         this->mpc_Toolbox->show();
      }
      else
      {
         this->mpc_FixMinimizedToolbox->show();
      }
      this->mpc_Scene->CheckAllItemsForChanges();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Function to prepare closing the widget

   Updating of the system definition without saving it into the files
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdTopologyWidget::PrepareToClose(void) const
{
   this->mpc_Scene->UpdateSystemDefinition();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten resize event

   Move the toolbox.

   \param[in,out]  opc_Event  Event identification and information
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdTopologyWidget::resizeEvent(QResizeEvent * const opc_Event)
{
   Q_UNUSED(opc_Event)

   // only resize if scene is active to avoid bad toolbox geometry in case of "open tool with *.syde double click"
   //   if (this->mpc_Scene->isActive() == true)
   //   {
   QPoint c_Point = this->mpc_Toolbox->pos();
   QSize c_Size = this->mpc_Toolbox->size();

   QPoint c_PointFixMiniToolbox = this->mpc_FixMinimizedToolbox->pos();

   QWidget * pc_Widget = this->parentWidget();

   if (pc_Widget == NULL)
   {
      // if no parent exist use this widget
      pc_Widget = this;
   }

   // would the toolbox be outside of the widget in x direction
   if ((this->mpc_Toolbox->x() + this->mpc_Toolbox->width() + mhsn_WidgetBorder) > pc_Widget->width())
   {
      // is the toolbox to big?
      if ((this->mpc_Toolbox->width() + (2 * mhsn_WidgetBorder)) > pc_Widget->width())
      {
         c_Size.setWidth(pc_Widget->width() - (2 * mhsn_WidgetBorder));
      }
      else
      {
         // adapt position of toolbox
         c_Point.setX((pc_Widget->width() - this->mpc_Toolbox->width()) - mhsn_WidgetBorder);
      }
   }

   // would the toolbox be outside of the widget in y direction
   if ((this->mpc_Toolbox->y() + this->mpc_Toolbox->height() + mhsn_WidgetBorder) > pc_Widget->height())
   {
      // is the toolbox to big?
      if ((this->mpc_Toolbox->height() + (2 * mhsn_WidgetBorder)) > pc_Widget->height())
      {
         c_Size.setHeight(pc_Widget->height() - (2 * mhsn_WidgetBorder));
      }
      else
      {
         // adapt position of toolbox
         c_Point.setY((pc_Widget->height() - this->mpc_Toolbox->height()) - mhsn_WidgetBorder);
      }
   }

   // adapt position of fix minimized toolbox
   c_PointFixMiniToolbox.setX((pc_Widget->width() - this->mpc_FixMinimizedToolbox->width()) -
                              mhsn_WidgetBorder);

   this->mpc_Toolbox->setGeometry(QRect(c_Point, c_Size));
   this->mpc_FixMinimizedToolbox->setGeometry(QRect(c_PointFixMiniToolbox, this->mpc_FixMinimizedToolbox->size()));
   //   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Getter of graphicsscene of topology widget.

   Example use case is to print 'Network Topology'.

   Hint: Getter can't be const because of call to render function for QPainter.

   \return
   graphicsscene of topology widget
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdTopologyScene * C_SdTopologyWidget::GetScene(void)
{
   return this->mpc_Scene;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Slot function of fix minimized toolbox widget for button maximizing click
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdTopologyWidget::m_WiFixPosMaxBtnClicked(void)
{
   this->mpc_Toolbox->setVisible(true);
   this->mpc_Toolbox->SetMaximized(true);
   this->mpc_FixMinimizedToolbox->setVisible(false);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Slot function of hover widget for button minimizing click
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdTopologyWidget::m_WiHoverMinBtnClicked(void)
{
   this->mpc_Toolbox->setVisible(false);
   this->mpc_Toolbox->SetMaximized(false);
   this->mpc_FixMinimizedToolbox->setVisible(true);
}
