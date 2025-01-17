//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for one node in the update package

   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include <QScrollBar>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QFileInfo>

#include "C_SyvUpUpdatePackageNodeWidget.h"
#include "ui_C_SyvUpUpdatePackageNodeWidget.h"

#include "stwerrors.h"

#include "constants.h"
#include "TGLUtils.h"
#include "C_GtGetText.h"
#include "C_OgeWiUtil.h"
#include "C_PuiSdHandler.h"
#include "C_PuiSvHandler.h"
#include "C_PuiProject.h"
#include "C_PuiUtil.h"
#include "C_ImpUtil.h"
#include "C_OgeWiCustomMessage.h"
#include "C_UsHandler.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_errors;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_gui_elements;
using namespace stw_opensyde_core;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */
const stw_types::sint32 C_SyvUpUpdatePackageNodeWidget::mhs32_LAYOUT_THRESHOLD = 100; // Keep synced with update
// summary widget

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in]      ou32_ViewIndex         View index
   \param[in]      ou32_PositionNumber    Position number of node for showing
   \param[in]      ou32_NodeIndex         Node index
   \param[in]      orc_NodeName           Name of the node
   \param[in,out]  opc_Parent             Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpUpdatePackageNodeWidget::C_SyvUpUpdatePackageNodeWidget(const uint32 ou32_ViewIndex,
                                                               const uint32 ou32_PositionNumber,
                                                               const uint32 ou32_NodeIndex,
                                                               const QString & orc_NodeName,
                                                               QWidget * const opc_Parent) :
   QWidget(opc_Parent),
   mpc_Ui(new Ui::C_SyvUpUpdatePackageNodeWidget),
   mu32_ViewIndex(ou32_ViewIndex),
   mu32_NodeIndex(ou32_NodeIndex),
   mc_NodeName(orc_NodeName),
   mq_FileBased(false),
   mq_NvmHalcBased(false),
   mq_StwFlashloader(false),
   mq_Connected(false),
   mq_EmptyOptionalSectionsVisible(true),
   mu32_PositionNumber(ou32_PositionNumber),
   mu32_FilesUpdated(0U),
   mpc_FilesWidget(NULL),
   mpc_FilesWidgetSeparator(NULL)
{
   this->mpc_Ui->setupUi(this);

   stw_opensyde_gui_logic::C_OgeWiUtil::h_ApplyStylesheetProperty(this->mpc_Ui->pc_FrameSepTop,
                                                                  "HasColor9Background", true);
   this->mpc_Ui->pc_ScrollAreaWidget->SetBackgroundColor(0);

   this->m_UpdateTitle();

   this->m_Init();

   //lint -e{1938}  static const is guaranteed preinitialized before main
   this->mpc_Ui->pc_WidgetTitle->SetColorReserved(mc_STYLE_GUIDE_COLOR_10);
   //lint -e{1938}  static const is guaranteed preinitialized before main
   this->mpc_Ui->pc_WidgetTitle->SetColorTooMuch(mc_STYLE_GUIDE_COLOR_10);
   //lint -e{1938}  static const is guaranteed preinitialized before main
   this->mpc_Ui->pc_WidgetTitle->SetColorFree(mc_STYLE_GUIDE_COLOR_0);

   this->mpc_Ui->pc_LabIconWarning->SetSvg("://images/system_views/IconWarning.svg");
   this->mpc_Ui->pc_LabIconWarning->SetToolTipInformation(
      C_GtGetText::h_GetText("Missing Files"),
      C_GtGetText::h_GetText("The Update Package for this node contains files that were not found."));
   this->mpc_Ui->pc_LabIconWarning->setVisible(false);

   this->mpc_Ui->pc_LabIconLock->SetSvg("://images/system_views/IconLockedGrey.svg");
   this->mpc_Ui->pc_LabIconLock->SetToolTipInformation(
      C_GtGetText::h_GetText("Update Package Locked"),
      C_GtGetText::h_GetText("Update Package cannot be configured while update mode is active."));
   this->mpc_Ui->pc_LabIconLock->setVisible(false);

   //Drag & drop of files
   this->setAcceptDrops(true);

   // restore sections expand/collapse from user settings
   const C_PuiSvData * const pc_View = C_PuiSvHandler::h_GetInstance()->GetView(this->mu32_ViewIndex);
   if (pc_View != NULL)
   {
      sintn sn_ExpectedSections;
      QVector<bool> c_Flags = C_UsHandler::h_GetInstance()->GetProjSvSetupView(pc_View->GetName()).
                              GetSvNode(orc_NodeName).GetSectionsExpanded();

      sn_ExpectedSections = this->mc_DatablockWidgets.size();
      if (this->mpc_FilesWidget != NULL)
      {
         ++sn_ExpectedSections;
      }

      if (c_Flags.size() == sn_ExpectedSections)
      {
         // The user settings still match, use it
         sintn sn_DatablockCounter;

         for (sn_DatablockCounter = 0; sn_DatablockCounter < this->mc_DatablockWidgets.size(); ++sn_DatablockCounter)
         {
            this->mc_DatablockWidgets[sn_DatablockCounter]->Expand(c_Flags[sn_DatablockCounter]);
         }

         if (this->mpc_FilesWidget != NULL)
         {
            this->mpc_FilesWidget->Expand(c_Flags[c_Flags.size() - 1]);
         }
      }
   }

   // Start defaults to make delegate work properly ("drag-mode")
   this->mpc_Ui->pc_ScrollArea->setVisible(false);
   this->mpc_Ui->pc_WidgetTitle->setMinimumHeight(25);
   this->mpc_Ui->pc_FrameSepTop->setVisible(false);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default destructor
*/
//----------------------------------------------------------------------------------------------------------------------
//lint -e{1540}  no memory leak because of the parent of mpc_FilesWidget & mpc_FilesWidgetSeparator and the Qt memory
// management
C_SyvUpUpdatePackageNodeWidget::~C_SyvUpUpdatePackageNodeWidget()
{
   // save user settings
   const C_PuiSvData * const pc_View = C_PuiSvHandler::h_GetInstance()->GetView(this->mu32_ViewIndex);

   if (pc_View != NULL)
   {
      QVector<bool> c_Flags;
      sintn sn_DatablockCounter;

      c_Flags.resize(this->mc_DatablockWidgets.size());

      for (sn_DatablockCounter = 0; sn_DatablockCounter < this->mc_DatablockWidgets.size(); ++sn_DatablockCounter)
      {
         c_Flags[sn_DatablockCounter] = this->mc_DatablockWidgets[sn_DatablockCounter]->IsExpanded();
      }

      if (this->mpc_FilesWidget != NULL)
      {
         c_Flags.push_back(this->mpc_FilesWidget->IsExpanded());
      }

      C_UsHandler::h_GetInstance()->SetProjSvUpdateSectionsExpandedFlags(pc_View->GetName(), this->mc_NodeName,
                                                                         c_Flags);
   }

   delete this->mpc_Ui;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Adapts the widget for the connected state
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::SetConnected(void)
{
   sint32 s32_DatablockCounter;

   this->mq_Connected = true;

   this->mpc_Ui->pc_LabIconLock->setVisible(true);

   // Reset progress bar
   this->mpc_Ui->pc_WidgetTitle->SetProgress(0U);

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      this->mc_DatablockWidgets[s32_DatablockCounter]->SetConnected();
   }

   if (this->mpc_FilesWidget != NULL)
   {
      this->mpc_FilesWidget->SetConnected();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Adapts the widget when the updated was started
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::SetUpdateStarted(void) const
{
   sint32 s32_DatablockCounter;
   bool q_ApplicationUpdateNecessary = false;

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      //lint -e{514}  Using operator with a bool value was intended and is no accident
      q_ApplicationUpdateNecessary |= this->mc_DatablockWidgets[s32_DatablockCounter]->IsUpdateNecessary();
   }

   if (this->mpc_FilesWidget != NULL)
   {
      //lint -e{514}  Using operator with a bool value was intended and is no accident
      q_ApplicationUpdateNecessary |= this->mpc_FilesWidget->IsUpdateNecessary();
   }

   if (q_ApplicationUpdateNecessary == true)
   {
      // Reset progress bar
      this->mpc_Ui->pc_WidgetTitle->SetProgress(0U);
   }
   else
   {
      // No update necessary.
      this->mpc_Ui->pc_WidgetTitle->SetProgress(100U);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Adapts the widget when the application update was started
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::SetUpdateApplicationStarted(void) const
{
   C_SyvUpPackageSectionNodeWidget * const pc_List = this->m_GetNextListInUpdateOrder();

   tgl_assert(pc_List != NULL);
   if (pc_List != NULL)
   {
      pc_List->SetUpdateApplicationStarted();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Adapts the widget when the updated was started
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::SetUpdateApplicationFinished(void)
{
   const uint32 u32_FileCount = this->m_GetFileCount();
   // Call before increase
   C_SyvUpPackageSectionNodeWidget * const pc_List = this->m_GetNextListInUpdateOrder();

   ++this->mu32_FilesUpdated;

   if (u32_FileCount > 0)
   {
      this->mpc_Ui->pc_WidgetTitle->SetProgress((this->mu32_FilesUpdated * 100U) / u32_FileCount);
   }
   else
   {
      this->mpc_Ui->pc_WidgetTitle->SetProgress(100U);
   }

   tgl_assert(pc_List != NULL);
   if (pc_List != NULL)
   {
      pc_List->SetUpdateApplicationFinished();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Adapts the widget when the application update hat an error
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::SetUpdateApplicationError(void) const
{
   C_SyvUpPackageSectionNodeWidget * const pc_List = this->m_GetNextListInUpdateOrder();

   tgl_assert(pc_List != NULL);
   if (pc_List != NULL)
   {
      pc_List->SetUpdateApplicationError();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Adapts the widget when the entire update was finished
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::SetUpdateFinished(void) const
{
   sint32 s32_DatablockCounter;

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      this->mc_DatablockWidgets[s32_DatablockCounter]->SetUpdateFinished();
   }

   if (this->mpc_FilesWidget != NULL)
   {
      this->mpc_FilesWidget->SetUpdateFinished();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Trigger device application information discard
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::DiscardApplicationStatus(void) const
{
   sint32 s32_DatablockCounter;

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      this->mc_DatablockWidgets[s32_DatablockCounter]->DiscardApplicationStatus();
   }

   if (this->mpc_FilesWidget != NULL)
   {
      this->mpc_FilesWidget->DiscardApplicationStatus();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Adapts the widget for the disconnected state

   Reset the application states
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::SetDisconnected(void)
{
   sint32 s32_DatablockCounter;

   this->mu32_FilesUpdated = 0U;
   this->SetProgress(0U);
   this->mq_Connected = false;

   this->mpc_Ui->pc_LabIconLock->setVisible(false);

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      this->mc_DatablockWidgets[s32_DatablockCounter]->SetDisconnected();
   }

   if (this->mpc_FilesWidget != NULL)
   {
      this->mpc_FilesWidget->SetDisconnected();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Update device information

   \param[in]  orc_DeviceInformation   Device info
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::UpdateDeviceInformation(const C_SyvUpDeviceInfo & orc_DeviceInformation) const
{
   sint32 s32_DatablockCounter;

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      this->mc_DatablockWidgets[s32_DatablockCounter]->UpdateDeviceInformation(orc_DeviceInformation);
   }

   if (this->mpc_FilesWidget != NULL)
   {
      this->mpc_FilesWidget->UpdateDeviceInformation(orc_DeviceInformation);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Sets the current progress for showing for all applications

   \param[in]  ou32_Percentage   Percentage of progress
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::SetProgress(const uint32 ou32_Percentage) const
{
   const uint32 u32_FileCount = this->m_GetFileCount();

   if (u32_FileCount > 0U)
   {
      const uint32 u32_FinishedNodesPercentage = (this->mu32_FilesUpdated * 100U) / u32_FileCount;
      this->mpc_Ui->pc_WidgetTitle->SetProgress(u32_FinishedNodesPercentage + (ou32_Percentage / u32_FileCount));
   }
   else
   {
      this->mpc_Ui->pc_WidgetTitle->SetProgress(0U);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Updates the position number

   \param[in]  ou32_PositionNumber  New position number
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::UpdatePositionNumber(const uint32 ou32_PositionNumber)
{
   sint32 s32_DatablockCounter;

   this->mu32_PositionNumber = ou32_PositionNumber;
   this->m_UpdateTitle();

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      this->mc_DatablockWidgets[s32_DatablockCounter]->UpdatePositionNumber(this->mu32_PositionNumber);
   }

   if (this->mpc_FilesWidget != NULL)
   {
      this->mpc_FilesWidget->UpdatePositionNumber(this->mu32_PositionNumber);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Adds a new file for the file based file list, a parameter set file or a PEM file

   \param[in]  orc_File       New path
   \param[in]  oq_Paramset    Flag if file for update or paramset file
   \param[in]  oq_PemFile     Flag if file is a PEM file
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::AddNewFile(const QString & orc_File, const bool oq_Paramset, const bool oq_PemFile)
{
   // compare all data block and file based files
   if ((this->mpc_FilesWidget != NULL) &&
       (this->mq_StwFlashloader == false) &&
       ((this->mq_FileBased == true) || (oq_Paramset == true) || (oq_PemFile == true)) &&
       (this->m_CheckFileAlreadyContained(orc_File) == false))
   {
      this->mpc_FilesWidget->AddFile(orc_File);
      this->mpc_FilesWidget->Expand(true);
   }

   // update visibility of optional sections
   this->UpdateSectionsVisibility();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Sets a new file for the application

   \param[in]  orc_File    New path
   \param[in]  opc_App     Application widget
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::AdaptFile(const QString & orc_File,
                                               C_SyvUpPackageListNodeItemWidget * const opc_App)
{
   C_SyvUpPackageSectionNodeWidget * const pc_Parent = m_GetAppParentList(opc_App);

   if (pc_Parent != NULL)
   {
      if (this->mq_FileBased == true)
      {
         // for file based nodes it is not allowed two add a file twice, independent of section where it gets added
         if (this->m_CheckFileAlreadyContained(orc_File) == false)
         {
            pc_Parent->AdaptFile(orc_File, opc_App);
         }
      }
      else
      {
         pc_Parent->AdaptFile(orc_File, opc_App);
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Revert the current application to the system definition default path

   Only for address based devices

   \param[in]  opc_App  Application widget
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::RevertFile(C_SyvUpPackageListNodeItemWidget * const opc_App) const
{
   C_SyvUpPackageSectionNodeWidget * const pc_Parent = m_GetAppParentList(opc_App);

   if (pc_Parent != NULL)
   {
      pc_Parent->RevertFile(opc_App);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Removes the current application

   Only for file based devices

   \param[in]  opc_App  Application widget
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::RemoveFile(C_SyvUpPackageListNodeItemWidget * const opc_App) const
{
   C_SyvUpPackageSectionNodeWidget * const pc_Parent = m_GetAppParentList(opc_App);

   if (pc_Parent != NULL)
   {
      pc_Parent->RemoveFile(opc_App);

      // update visibility of optional sections
      this->UpdateSectionsVisibility();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Removes all application paths
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::RemoveAllFiles(void) const
{
   sint32 s32_DatablockCounter;

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      this->mc_DatablockWidgets[s32_DatablockCounter]->RemoveAllFiles();
   }

   if (this->mpc_FilesWidget != NULL)
   {
      this->mpc_FilesWidget->RemoveAllFiles();
   }

   tgl_assert(C_PuiSvHandler::h_GetInstance()->ClearNodeUpdateInformationAsAppropriate(this->mu32_ViewIndex,
                                                                                       this->mu32_NodeIndex,
                                                                                       C_PuiSvNodeUpdate::
                                                                                       eFTP_DATA_BLOCK) == C_NO_ERR);

   if (this->mq_FileBased == true)
   {
      tgl_assert(C_PuiSvHandler::h_GetInstance()->ClearNodeUpdateInformationAsAppropriate(this->mu32_ViewIndex,
                                                                                          this->mu32_NodeIndex,
                                                                                          C_PuiSvNodeUpdate::
                                                                                          eFTP_FILE_BASED) == C_NO_ERR);
   }

   // update visibility of optional sections
   this->UpdateSectionsVisibility();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Prepares the update package node configuration

   \param[out]  orc_NodeConfig   Node configuration
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::PrepareExportConfig(C_SyvUpUpdatePackageConfigNode & orc_NodeConfig) const
{
   sint32 s32_DatablockCounter;

   orc_NodeConfig.c_Name = this->mc_NodeName;
   orc_NodeConfig.c_DeviceType = this->mc_DeviceType;

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      this->mc_DatablockWidgets[s32_DatablockCounter]->PrepareExportConfig(orc_NodeConfig);
   }

   if (this->mpc_FilesWidget != NULL)
   {
      this->mpc_FilesWidget->PrepareExportConfig(orc_NodeConfig);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Loads all matching configurations for this node of the import configuration

   \param[out]  orc_Config    Import configuration
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::LoadImportConfig(const C_SyvUpUpdatePackageConfig & orc_Config) const
{
   sint32 s32_DatablockCounter;

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      this->mc_DatablockWidgets[s32_DatablockCounter]->LoadImportConfig(orc_Config);
   }

   if (this->mpc_FilesWidget != NULL)
   {
      this->mpc_FilesWidget->LoadImportConfig(orc_Config);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Checks all paths for existence

   \param[out]  oru32_CountFiles             Number of files
   \param[out]  opc_FlashwareWarningsApps    App names of apps with flashware warnings
   \param[out]  opc_MissingApps              App names of apps with missing data block files
   \param[out]  opc_MissingParamFiles        App names of apps with missing parameter set image files
   \param[out]  opc_MissingFiles             App names of apps with missing files

   \return
   C_NO_ERR    All files are existent
   C_CONFIG    Minimum one file was not found
*/
//----------------------------------------------------------------------------------------------------------------------
sint32 C_SyvUpUpdatePackageNodeWidget::CheckAllFiles(stw_types::uint32 & oru32_CountFiles,
                                                     QStringList * const opc_FlashwareWarningsApps,
                                                     QStringList * const opc_MissingApps,
                                                     QStringList * const opc_MissingParamFiles,
                                                     QStringList * const opc_MissingFiles) const
{
   sint32 s32_Return = C_NO_ERR;

   sint32 s32_DatablockCounter;

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      s32_Return += this->mc_DatablockWidgets[s32_DatablockCounter]->CheckAllFiles(oru32_CountFiles, opc_MissingApps,
                                                                                   opc_MissingParamFiles,
                                                                                   opc_FlashwareWarningsApps);
   }

   if (this->mpc_FilesWidget != NULL)
   {
      s32_Return += this->mpc_FilesWidget->CheckAllFiles(oru32_CountFiles, opc_MissingFiles,
                                                         opc_MissingParamFiles,
                                                         opc_FlashwareWarningsApps);
   }

   if (s32_Return != C_NO_ERR)
   {
      s32_Return = C_CONFIG;
   }

   this->mpc_Ui->pc_LabIconWarning->setVisible(s32_Return != C_NO_ERR);

   return s32_Return;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Returns the section

   \param[in]  orc_Pos  Position of application

   \retval   NULL            No section list on the position
   \retval   Valid pointer   Pointer to section list
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpPackageSectionNodeWidget * C_SyvUpUpdatePackageNodeWidget::GetSectionList(const QPoint & orc_Pos) const
{
   C_SyvUpPackageSectionNodeWidget * pc_Return = NULL;
   const QPoint c_AdaptedPosScrollArea = this->mpc_Ui->pc_ScrollAreaWidget->mapFrom(this->parentWidget(), orc_Pos);

   sint32 s32_DatablockCounter;

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      C_SyvUpPackageSectionNodeDatablockWidget * const pc_DbWidget = this->mc_DatablockWidgets[s32_DatablockCounter];
      const QPoint c_AdaptedPosList = pc_DbWidget->mapFrom(this->mpc_Ui->pc_ScrollAreaWidget,
                                                           c_AdaptedPosScrollArea);
      if (pc_DbWidget->rect().contains(c_AdaptedPosList) == true)
      {
         pc_Return = pc_DbWidget;
         break;
      }
   }

   if ((pc_Return == NULL) &&
       (this->mpc_FilesWidget != NULL))
   {
      const QPoint c_AdaptedPosList = this->mpc_FilesWidget->mapFrom(this->mpc_Ui->pc_ScrollAreaWidget,
                                                                     c_AdaptedPosScrollArea);
      //c_AdaptedPosList = c_AdaptedPosScrollArea;
      if (this->mpc_FilesWidget->rect().contains(c_AdaptedPosList) == true)
      {
         pc_Return = this->mpc_FilesWidget;
      }
   }

   return pc_Return;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Returns and select the application on the position

   \param[in]  orc_Pos  Position of application

   \return
   Pointer to application
   NULL if no application was at the position
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpPackageListNodeItemWidget * C_SyvUpUpdatePackageNodeWidget::GetAndSelectApplication(const QPoint & orc_Pos)
const
{
   C_SyvUpPackageListNodeItemWidget * pc_App = NULL;
   const QPoint c_AdaptedPos = this->mpc_Ui->pc_ScrollAreaWidget->mapFrom(this->parentWidget(), orc_Pos);

   sint32 s32_DatablockCounter;

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      C_SyvUpPackageSectionNodeDatablockWidget * const pc_DbWidget = this->mc_DatablockWidgets[s32_DatablockCounter];
      pc_App = pc_DbWidget->GetAndSelectApplication(c_AdaptedPos);

      if (pc_App != NULL)
      {
         break;
      }
   }

   if ((pc_App == NULL) &&
       (this->mpc_FilesWidget != NULL))
   {
      pc_App = this->mpc_FilesWidget->GetAndSelectApplication(c_AdaptedPos);
   }

   return pc_App;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Returns the application on the position

   \param[in]   orc_Pos       Position of application
   \param[out]  opu32_Number  Optional parameter for found app number

   \return
   Pointer to application
   NULL if no application was at the position
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpPackageListNodeItemWidget * C_SyvUpUpdatePackageNodeWidget::GetApplication(const QPoint & orc_Pos,
                                                                                  uint32 * const opu32_Number)
const
{
   C_SyvUpPackageListNodeItemWidget * pc_App = NULL;
   const QPoint c_AdaptedPos = this->mpc_Ui->pc_ScrollAreaWidget->mapFrom(this->parentWidget(), orc_Pos);

   sint32 s32_DatablockCounter;

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      C_SyvUpPackageSectionNodeDatablockWidget * const pc_DbWidget = this->mc_DatablockWidgets[s32_DatablockCounter];
      pc_App = pc_DbWidget->GetApplication(c_AdaptedPos, opu32_Number);

      if (pc_App != NULL)
      {
         break;
      }
   }

   if (pc_App == NULL)
   {
      if (this->mpc_FilesWidget != NULL)
      {
         pc_App = this->mpc_FilesWidget->GetApplication(c_AdaptedPos, opu32_Number);
      }
   }

   return pc_App;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Returns the node index

   \return
   Current node index
*/
//----------------------------------------------------------------------------------------------------------------------
uint32 C_SyvUpUpdatePackageNodeWidget::GetNodeIndex(void) const
{
   return this->mu32_NodeIndex;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Returns view index

   \return
   View index
*/
//----------------------------------------------------------------------------------------------------------------------
uint32 C_SyvUpUpdatePackageNodeWidget::GetViewIndex(void) const
{
   return this->mu32_ViewIndex;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Returns the node name

   possible return value(s) and description

   \return
   Node name
*/
//----------------------------------------------------------------------------------------------------------------------
QString C_SyvUpUpdatePackageNodeWidget::GetNodeName(void) const
{
   return this->mc_NodeName;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Returns the current position number

   \return
   Current position number
*/
//----------------------------------------------------------------------------------------------------------------------
uint32 C_SyvUpUpdatePackageNodeWidget::GetPositionNumber(void) const
{
   return this->mu32_PositionNumber;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Returns the flag if the node is a file based device

   \return
   true     Node is file based
   false    Node is not file based
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_SyvUpUpdatePackageNodeWidget::IsFileBased(void) const
{
   return this->mq_FileBased;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Returns the flag if the node has a STW flashloader

   \return
   true     Node has STW flashloader
   false    Node has not STW flashloader
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_SyvUpUpdatePackageNodeWidget::IsStwFlashloader(void) const
{
   return this->mq_StwFlashloader;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Creates and returns the update package with all information for the system update for the concrete node

   \param[out]  orc_ApplicationsToWrite   Vector with node update configuration
   \param[out]  opc_AllApplications       Optional vector with all node applications

   \return
   C_NO_ERR    Update package with all information created
   C_RD_WR     At least one file does not exist
   C_NOACT     No files for applications to write added
*/
//----------------------------------------------------------------------------------------------------------------------
sint32 C_SyvUpUpdatePackageNodeWidget::GetUpdatePackage(C_OSCSuSequences::C_DoFlash & orc_ApplicationsToWrite,
                                                        C_OSCSuSequences::C_DoFlash * const opc_AllApplications)
{
   sint32 s32_Return = C_NOACT;
   bool q_NoErr = false;
   sint32 s32_DatablockCounter;

   this->mu32_FilesUpdated = 0U;

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      C_SyvUpPackageSectionNodeDatablockWidget * const pc_DbWidget = this->mc_DatablockWidgets[s32_DatablockCounter];
      s32_Return = pc_DbWidget->GetUpdatePackage(orc_ApplicationsToWrite, opc_AllApplications,
                                                 this->mu32_FilesUpdated);
      if (s32_Return == C_NO_ERR)
      {
         q_NoErr = true;
      }
      else if (s32_Return != C_NOACT)
      {
         // C_NOACT is in this case no error yet
         break;
      }
      else
      {
         // Nothing to do
      }
   }

   if (((s32_Return == C_NO_ERR) ||
        (s32_Return == C_NOACT)) &&
       (this->mpc_FilesWidget != NULL))
   {
      s32_Return = this->mpc_FilesWidget->GetUpdatePackage(orc_ApplicationsToWrite, opc_AllApplications,
                                                           this->mu32_FilesUpdated);
   }

   if ((q_NoErr == true) &&
       (s32_Return == C_NOACT))
   {
      // C_NOACT is no error if at least one GetUpdatePackage call returned with C_NO_ERR
      s32_Return = C_NO_ERR;
   }

   return s32_Return;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Collapsing all sections

   View for delegate
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::CollapseAll(void) const
{
   sint32 s32_DatablockCounter;

   this->mpc_Ui->pc_ScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      C_SyvUpPackageSectionNodeDatablockWidget * const pc_DbWidget = this->mc_DatablockWidgets[s32_DatablockCounter];
      pc_DbWidget->Expand(false);
   }

   if (this->mpc_FilesWidget != NULL)
   {
      this->mpc_FilesWidget->Expand(false);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief Set empty optional sections visible flag and update visibility.

   \param[in]  oq_Visible  true: show sections   false: hide sections
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::SetEmptyOptionalSectionsVisible(const bool oq_Visible)
{
   // remember
   this->mq_EmptyOptionalSectionsVisible = oq_Visible;

   // update visibility
   this->UpdateSectionsVisibility();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief Update visibility of sections

   Show or hide sections "Parameter Set Images" and "Files" if there are no files in this sections.
   Do not touch sections that contain files.
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::UpdateSectionsVisibility(void) const
{
   // files
   if (this->mpc_FilesWidget != NULL) // never visible for non-file-based node
   {
      if (this->mpc_FilesWidget->GetFileCount() == 0)
      {
         this->mpc_FilesWidget->setVisible(this->mq_EmptyOptionalSectionsVisible);
         if (this->mpc_FilesWidgetSeparator != NULL)
         {
            // Hide the separator. Could be NULL when mpc_FilesWidget is not NULL if no the node has no datablocks
            this->mpc_FilesWidgetSeparator->setVisible(this->mq_EmptyOptionalSectionsVisible);
         }
      }
      else
      {
         this->mpc_FilesWidget->setVisible(true);
         if (this->mpc_FilesWidgetSeparator != NULL)
         {
            this->mpc_FilesWidgetSeparator->setVisible(true);
         }
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten show event slot

   Here: Adapt the icon position and layout state

   \param[in,out]  opc_Event  Event identification and information
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::resizeEvent(QResizeEvent * const opc_Event)
{
   QWidget::resizeEvent(opc_Event);

   if (this->height() < mhs32_LAYOUT_THRESHOLD)
   {
      this->mpc_Ui->pc_ScrollArea->setVisible(false);
      this->mpc_Ui->pc_FrameSepTop->setVisible(false);
      this->mpc_Ui->pc_WidgetTitle->setMinimumHeight(25);
   }
   else
   {
      this->mpc_Ui->pc_ScrollArea->setVisible(true);
      this->mpc_Ui->pc_FrameSepTop->setVisible(true);
      this->mpc_Ui->pc_WidgetTitle->setMinimumHeight(45);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten drag enter event slot

   Here: Accept external files

   \param[in,out]  opc_Event  Event identification and information
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::dragEnterEvent(QDragEnterEvent * const opc_Event)
{
   if (this->mq_Connected == false)
   {
      const QMimeData * const pc_MimeData = opc_Event->mimeData();

      if (pc_MimeData->hasUrls() == true)
      {
         opc_Event->accept();
      }
   }
   else
   {
      const QSize c_Size(36, 36);

      // Accept always to get the dragLeaveEvent to reset the size of the lock icon
      opc_Event->accept();

      this->mpc_Ui->pc_LabIconLock->setMinimumSize(c_Size);
      this->mpc_Ui->pc_LabIconLock->setMaximumSize(c_Size);
   }

   QWidget::dragEnterEvent(opc_Event);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten drag move event slot

   Here: Accept external files

   \param[in,out]  opc_Event  Event identification and information
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::dragMoveEvent(QDragMoveEvent * const opc_Event)
{
   if (this->mq_Connected == false)
   {
      const QMimeData * const pc_MimeData = opc_Event->mimeData();

      if (pc_MimeData->hasUrls() == true)
      {
         const QPoint c_AdaptedPos = this->mpc_Ui->pc_ScrollAreaWidget->mapFrom(this, opc_Event->pos());

         // Get Application expects coordinate from the parent of this widget
         if (this->m_CheckMime(pc_MimeData, c_AdaptedPos) == true)
         {
            opc_Event->accept();
         }
         else
         {
            opc_Event->ignore();
         }
      }
   }
   else
   {
      opc_Event->ignore();
   }

   QWidget::dragMoveEvent(opc_Event);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten drag leave event slot

   Here: Deactivate selection of apps

   \param[in,out]  opc_Event  Event identification and information
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::dragLeaveEvent(QDragLeaveEvent * const opc_Event)
{
   const QSize c_Size(24, 24);
   sint32 s32_DatablockCounter;

   this->mpc_Ui->pc_LabIconLock->setMinimumSize(c_Size);
   this->mpc_Ui->pc_LabIconLock->setMaximumSize(c_Size);

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      C_SyvUpPackageSectionNodeDatablockWidget * const pc_DbWidget = this->mc_DatablockWidgets[s32_DatablockCounter];
      this->m_SetApplicationsUnselected(pc_DbWidget);
   }

   if (this->mpc_FilesWidget != NULL)
   {
      this->m_SetApplicationsUnselected(this->mpc_FilesWidget);
   }

   QWidget::dragLeaveEvent(opc_Event);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten drop event slot

   Here: Handle dropped files

   \param[in,out]  opc_Event  Event identification and information
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::dropEvent(QDropEvent * const opc_Event)
{
   if (this->mq_Connected == false)
   {
      const QMimeData * const pc_MimeData = opc_Event->mimeData();
      QStringList c_FilePathsDatablocks;
      QStringList c_FilePathsParamsetFiles;
      QStringList c_FilePathsFileBased;
      QStringList c_FilePathsPemFiles;
      C_SyvUpPackageListNodeItemWidget * pc_App = NULL;
      const QPoint c_AdaptedPos = this->mpc_Ui->pc_ScrollAreaWidget->mapFrom(this, opc_Event->pos());

      if (this->m_CheckMime(pc_MimeData, c_AdaptedPos, &c_FilePathsDatablocks,
                            &c_FilePathsParamsetFiles,
                            &c_FilePathsFileBased,
                            &c_FilePathsPemFiles,
                            &pc_App) == true)
      {
         const QString c_Folder = C_PuiProject::h_GetInstance()->GetFolderPath();

         if ((pc_App != NULL) &&
             (c_FilePathsDatablocks.size() == 1))
         {
            // Datablock only

            // Check if relative paths are possible and appreciated
            c_FilePathsDatablocks[0] =
               C_ImpUtil::h_AskUserToSaveRelativePath(this, c_FilePathsDatablocks[0], c_Folder);

            if (c_FilePathsDatablocks[0] != "")
            {
               this->AdaptFile(c_FilePathsDatablocks[0], pc_App);
            }
         }
         else
         {
            // Param sets and file based entries

            // Check if relative paths are possible and appreciated (for all files ask once)
            QStringList c_PathsParamAndPemAndFile;
            // Concatenate paths
            c_PathsParamAndPemAndFile.append(c_FilePathsParamsetFiles);
            c_PathsParamAndPemAndFile.append(c_FilePathsFileBased);
            c_PathsParamAndPemAndFile.append(c_FilePathsPemFiles);

            // Ask
            c_PathsParamAndPemAndFile =
               C_ImpUtil::h_AskUserToSaveRelativePath(this, c_PathsParamAndPemAndFile, c_Folder);

            if (c_PathsParamAndPemAndFile.size() > 0)
            {
               // Re-split paths
               tgl_assert(c_PathsParamAndPemAndFile.size() ==
                          (c_FilePathsParamsetFiles.size() + c_FilePathsFileBased.size() + c_FilePathsPemFiles.size()));

               c_FilePathsParamsetFiles = c_PathsParamAndPemAndFile.mid(0, c_FilePathsParamsetFiles.size());

               c_FilePathsFileBased = c_PathsParamAndPemAndFile.mid(
                  c_FilePathsParamsetFiles.size(), c_FilePathsFileBased.size());

               c_FilePathsPemFiles = c_PathsParamAndPemAndFile.mid(
                  c_FilePathsParamsetFiles.size() + c_FilePathsFileBased.size(),
                  -1);

               if ((pc_App != NULL) &&
                   (c_FilePathsFileBased.size() == 1) &&
                   (c_FilePathsParamsetFiles.size() == 0) &&
                   (c_FilePathsPemFiles.size() == 0))
               {
                  // Replace one file based entry
                  this->AdaptFile(c_FilePathsFileBased[0], pc_App);
               }
               else if ((pc_App != NULL) &&
                        (c_FilePathsFileBased.size() == 0) &&
                        (c_FilePathsParamsetFiles.size() == 1) &&
                        (c_FilePathsPemFiles.size() == 0))
               {
                  // Replace one parameter set image entry
                  this->AdaptFile(c_FilePathsParamsetFiles[0], pc_App);
               }
               else if ((pc_App != NULL) &&
                        (c_FilePathsFileBased.size() == 0) &&
                        (c_FilePathsParamsetFiles.size() == 0) &&
                        (c_FilePathsPemFiles.size() == 1))
               {
                  // Replace one PEM file entry
                  this->AdaptFile(c_FilePathsPemFiles[0], pc_App);
               }
               else
               {
                  sintn sn_FileCounter;
                  // Add all files as new
                  for (sn_FileCounter = 0; sn_FileCounter < c_FilePathsFileBased.size(); ++sn_FileCounter)
                  {
                     // Add new file. If list does not support adding new files, nothing will happen
                     this->AddNewFile(c_FilePathsFileBased[sn_FileCounter], false, false);
                  }
                  for (sn_FileCounter = 0; sn_FileCounter < c_FilePathsParamsetFiles.size(); ++sn_FileCounter)
                  {
                     // Add new file. If list does not support adding new files, nothing will happen
                     this->AddNewFile(c_FilePathsParamsetFiles[sn_FileCounter], true, false);
                  }
                  for (sn_FileCounter = 0; sn_FileCounter < c_FilePathsPemFiles.size(); ++sn_FileCounter)
                  {
                     // Add new file. If list does not support adding new files, nothing will happen
                     this->AddNewFile(c_FilePathsPemFiles[sn_FileCounter], false, true);
                  }
               }
            }
         }

         if (pc_App != NULL)
         {
            pc_App->SetSelected(false);
         }
      }
   }

   QWidget::dropEvent(opc_Event);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Initialization
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::m_Init(void)
{
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);
   QSpacerItem * const pc_Spacer = new QSpacerItem(0, 3, QSizePolicy::Minimum, QSizePolicy::Expanding);
   uint32 u32_DatablockParamSetFiles = 0U;

   tgl_assert(pc_Node != NULL);
   if (pc_Node != NULL)
   {
      uint32 u32_DatablockCounter;
      uint32 u32_ViewDataBlockPathNumber = 0;

      tgl_assert(pc_Node->pc_DeviceDefinition != NULL);
      tgl_assert(pc_Node->u32_SubDeviceIndex < pc_Node->pc_DeviceDefinition->c_SubDevices.size());
      this->mq_FileBased =
         pc_Node->pc_DeviceDefinition->c_SubDevices[pc_Node->u32_SubDeviceIndex].q_FlashloaderOpenSydeIsFileBased;
      this->mq_StwFlashloader = (pc_Node->c_Properties.e_FlashLoader == C_OSCNodeProperties::eFL_STW);
      this->mc_DeviceType = pc_Node->c_DeviceType.c_str();

      // Only relevant if the node has datablocks
      for (u32_DatablockCounter = 0U; u32_DatablockCounter < pc_Node->c_Applications.size(); ++u32_DatablockCounter)
      {
         C_SyvUpPackageSectionNodeDatablockWidget * const pc_Datablock = new C_SyvUpPackageSectionNodeDatablockWidget(
            this);
         this->mpc_Ui->pc_ScrollAreaLayout->addWidget(pc_Datablock);
         this->mc_DatablockWidgets.push_back(pc_Datablock);

         pc_Datablock->InitWidget(this->mu32_ViewIndex, this->mu32_PositionNumber,
                                  this->mu32_NodeIndex, this->mc_NodeName, u32_DatablockCounter,
                                  u32_ViewDataBlockPathNumber);

         // Special case, parameter set image files in a datablock. Necessary to know the reserved saved
         // paths of the parameter set image file for the files widget
         u32_DatablockParamSetFiles += pc_Datablock->GetParamSetFileCount();

         if (pc_Node->c_Applications[u32_DatablockCounter].e_Type ==
             stw_opensyde_core::C_OSCNodeApplication::ePARAMETER_SET_HALC)
         {
            // Special case: A HALC NVM node
            this->mq_NvmHalcBased = true;
            tgl_assert(u32_DatablockParamSetFiles > 0);
         }
         else
         {
            ++u32_ViewDataBlockPathNumber;
         }

         // Add separator as long it is not the last widget
         if ((static_cast<uintn>(u32_DatablockCounter) < (pc_Node->c_Applications.size() - 1U)) ||
             ((this->mq_StwFlashloader == false) && (this->mq_NvmHalcBased == false)))
         {
            this->m_AddSeparatorToScrollArea(static_cast<uintn>(u32_DatablockCounter) ==
                                             (pc_Node->c_Applications.size() - 1U));
         }
      } //lint !e429  //no memory leak because of the parent of pc_Datablock and the Qt memory management

      // Check if an "Other files" section is necessary
      // It is only necessary for file based devices and can get psi files without restrictions
      if (this->mq_StwFlashloader == false)
      {
         tgl_assert(this->mpc_FilesWidget == NULL);
         this->mpc_FilesWidget = new C_SyvUpPackageSectionNodeFilesWidget(this);
         this->mpc_Ui->pc_ScrollAreaLayout->addWidget(this->mpc_FilesWidget);

         this->mpc_FilesWidget->SetCountSkippedParamSetFiles(u32_DatablockParamSetFiles);
         // last parameter is "0", since this case has no Data Block output files
         this->mpc_FilesWidget->InitWidget(this->mu32_ViewIndex, this->mu32_PositionNumber,
                                           this->mu32_NodeIndex, this->mc_NodeName, pc_Node->c_Applications.size(), 0);
      }
   }

   this->mpc_Ui->pc_ScrollAreaLayout->addSpacerItem(pc_Spacer);
   this->mpc_Ui->pc_ScrollAreaLayout->setStretch(this->mpc_Ui->pc_ScrollAreaLayout->count() - 1, 1);
} //lint !e429  //no memory leak because of the parent of pc_Spacer and the Qt memory management

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Adds a separator to the scroll area

   Used from generated code

   \param[in]  oq_FilesWidgetSeparator    Files widget separator
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::m_AddSeparatorToScrollArea(const bool oq_FilesWidgetSeparator)
{
   stw_opensyde_gui_elements::C_OgeFraSeparator * const pc_Separator =
      new stw_opensyde_gui_elements::C_OgeFraSeparator(this);
   QSizePolicy c_SizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
   c_SizePolicy.setHorizontalStretch(0);
   c_SizePolicy.setVerticalStretch(0);
   c_SizePolicy.setHeightForWidth(pc_Separator->sizePolicy().hasHeightForWidth());

   stw_opensyde_gui_logic::C_OgeWiUtil::h_ApplyStylesheetProperty(pc_Separator, "HasColor9Background", true);
   pc_Separator->setSizePolicy(c_SizePolicy);
   pc_Separator->setMinimumSize(QSize(0, 1));
   pc_Separator->setMaximumSize(QSize(16777215, 1));
   pc_Separator->setFrameShape(QFrame::StyledPanel);
   pc_Separator->setFrameShadow(QFrame::Raised);

   this->mpc_Ui->pc_ScrollAreaWidget->layout()->addWidget(pc_Separator);

   if (oq_FilesWidgetSeparator == true)
   {
      // Save the files widget separator for hiding it
      this->mpc_FilesWidgetSeparator = pc_Separator;
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Update of the title label
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::m_UpdateTitle(void) const
{
   this->mpc_Ui->pc_Title->setText(static_cast<QString>("#%1 - %2").arg(QString::number(this->mu32_PositionNumber + 1U),
                                                                        this->mc_NodeName));
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Returns the number of all files in all sections

   \return
   Number of files
*/
//----------------------------------------------------------------------------------------------------------------------
uint32 C_SyvUpUpdatePackageNodeWidget::m_GetFileCount(void) const
{
   uint32 u32_Count = 0U;

   sint32 s32_DatablockCounter;

   for (s32_DatablockCounter = 0U; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      C_SyvUpPackageSectionNodeDatablockWidget * const pc_DbWidget = this->mc_DatablockWidgets[s32_DatablockCounter];
      u32_Count += pc_DbWidget->GetFileCount();
   }

   if (this->mpc_FilesWidget != NULL)
   {
      u32_Count += this->mpc_FilesWidget->GetFileCount();
   }

   return u32_Count;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Returns the current relevant list for the current updating/transferring file/application

  This function use the value mu32_FilesUpdated as reference. The current file shall not be calculated into
  mu32_FilesUpdated before the call of this function.

   \retval   Valid pointer    Pointer to the list, which has the current updated file/application
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpPackageSectionNodeWidget * C_SyvUpUpdatePackageNodeWidget::m_GetNextListInUpdateOrder(void) const
{
   C_SyvUpPackageSectionNodeWidget * pc_List = NULL;
   bool q_ListFound = false;

   sint32 s32_DatablockCounter;

   for (s32_DatablockCounter = 0; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
   {
      pc_List = this->mc_DatablockWidgets[s32_DatablockCounter];

      // Check for application datablocks first
      if (pc_List->GetPrimaryFileCount() > 0U)
      {
         // No more files updated than current datablock. Do not check next section.
         if (pc_List->GetSectionState() != C_SyvUpPackageListNodeItemWidget::hu32_STATE_FINISHED)
         {
            q_ListFound = true;
            break;
         }
      }
   }

   if (q_ListFound == false)
   {
      for (s32_DatablockCounter = 0; s32_DatablockCounter < this->mc_DatablockWidgets.size(); ++s32_DatablockCounter)
      {
         pc_List = this->mc_DatablockWidgets[s32_DatablockCounter];

         // Check for psi datablocks after the application datablocks
         if (pc_List->GetParamSetFileCount() > 0U)
         {
            // No more files updated than current datablock. Do not check next section.
            if (pc_List->GetSectionState() != C_SyvUpPackageListNodeItemWidget::hu32_STATE_FINISHED)
            {
               q_ListFound = true;
               break;
            }
         }
      }
   }

   // More files updated than datablocks. Check next section.
   if (q_ListFound == false)
   {
      if (this->mpc_FilesWidget != NULL)
      {
         // Do not check for finished state here. If no list was already found, a state configuration error for a PEM
         // file was probably detected, so the error belongs to the "other files", but the finished state does not match

         // Current file is in file based section
         pc_List = this->mpc_FilesWidget;
      }
      else
      {
         // Shall not happen
         tgl_assert(false);
      }
   }

   tgl_assert(pc_List != NULL);

   return pc_List;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Sets all applications of the specific list unselected

   \param[in]  opc_List    List with application to adapt
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpUpdatePackageNodeWidget::m_SetApplicationsUnselected(const C_SyvUpPackageSectionNodeWidget * const opc_List)
const
{
   uint32 u32_AppCounter;

   for (u32_AppCounter = 0U; u32_AppCounter < opc_List->GetFileCount(); ++u32_AppCounter)
   {
      opc_List->SetApplicationSelect(u32_AppCounter, false);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Returns the list parent of a specific app

   \param[in]  opc_App  Application widget

   \return
   Valid pointer     Pointer to list widget with this app as child
   NULL              No parent was found
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpPackageSectionNodeWidget * C_SyvUpUpdatePackageNodeWidget::m_GetAppParentList(
   C_SyvUpPackageListNodeItemWidget * const opc_App) const
{
   C_SyvUpPackageSectionNodeWidget * pc_Parent = NULL;

   if (opc_App != NULL)
   {
      pc_Parent = dynamic_cast<C_SyvUpPackageSectionNodeWidget *>(opc_App->GetListParent());

      tgl_assert(pc_Parent != NULL);
   }

   return pc_Parent;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Compare all files in sections data blocks and files with input file.

   If file is already contained in update package, an information message is shown.

   \param[in]  orc_File    Input file

   \retval   true    File is already contained
   \retval   false   File is new
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_SyvUpUpdatePackageNodeWidget::m_CheckFileAlreadyContained(const QString & orc_File)
{
   const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);
   const C_PuiSvData * const pc_View = C_PuiSvHandler::h_GetInstance()->GetView(this->mu32_ViewIndex);
   bool q_Retval = false;

   if ((pc_Node != NULL) &&
       (pc_View != NULL))
   {
      const QString c_AbsoluteFile = C_PuiUtil::h_GetResolvedAbsPathFromProject(orc_File);
      std::vector<QString> c_Paths;

      // compare with existing data block files
      c_Paths = pc_View->GetNodeUpdateInformation(this->mu32_NodeIndex)->GetPaths(C_PuiSvNodeUpdate::eFTP_DATA_BLOCK);
      for (std::vector<QString>::const_iterator c_It = c_Paths.begin(); c_It != c_Paths.end(); ++c_It)
      {
         if (c_AbsoluteFile == C_PuiUtil::h_GetResolvedAbsPathFromProject(*c_It))
         {
            q_Retval = true;
            break;
         }
      }

      // compare with existing file based files
      if (q_Retval == false)
      {
         c_Paths =
            pc_View->GetNodeUpdateInformation(this->mu32_NodeIndex)->GetPaths(C_PuiSvNodeUpdate::eFTP_FILE_BASED);
         for (std::vector<QString>::const_iterator c_It = c_Paths.begin(); c_It != c_Paths.end(); ++c_It)
         {
            if (c_AbsoluteFile == C_PuiUtil::h_GetAbsolutePathFromProject(*c_It)) // variables resolve not necessary
            {
               q_Retval = true;
               break;
            }
         }
      }
   }

   if (q_Retval == true)
   {
      // inform user that file already exists
      C_OgeWiCustomMessage c_Message(this);
      c_Message.SetHeading(C_GtGetText::h_GetText("Add file"));
      c_Message.SetDescription(static_cast<QString>(C_GtGetText::h_GetText(
                                                       "The file is already contained in the Update Package "
                                                       "for this node and therefore not added again.")));
      c_Message.SetDetails(static_cast<QString>(C_GtGetText::h_GetText("%1"))
                           .arg(C_PuiUtil::h_GetResolvedAbsPathFromProject(orc_File)));
      c_Message.Execute();
   }

   return q_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Check if mime valid

   \param[in]   opc_Mime                  Mime to check
   \param[in]   orc_Pos                   Mouse position
   \param[out]  opc_FilePathsDatablocks   File paths which could be used for datablock list
   \param[out]  opc_FilePathsParamsets    File paths which could be used for parameter set image list
   \param[out]  opc_FilePathsFileBased    File paths which could be used for file based list
   \param[out]  opc_RelevantPemFilePaths  PEM file paths which could be used for file based list
   \param[out]  oppc_App                  Found application widget

   \return
   true  Valid
   false Invalid
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_SyvUpUpdatePackageNodeWidget::m_CheckMime(const QMimeData * const opc_Mime, const QPoint & orc_Pos,
                                                 QStringList * const opc_FilePathsDatablocks,
                                                 QStringList * const opc_FilePathsParamsets,
                                                 QStringList * const opc_FilePathsFileBased,
                                                 QStringList * const opc_RelevantPemFilePaths,
                                                 C_SyvUpPackageListNodeItemWidget ** const oppc_App) const
{
   bool q_Retval = false;

   // Check the mime parameter
   if ((opc_Mime != NULL) &&
       (opc_Mime->hasUrls() == true))
   {
      QStringList c_PathList;
      const QList<QUrl> c_UrlList = opc_Mime->urls();
      QFileInfo c_File;
      sintn sn_FileCounter;
      bool q_FilesValid = true;

      // extract the local paths of the files
      for (sintn sn_It = 0; sn_It < c_UrlList.size(); ++sn_It)
      {
         c_PathList.append(c_UrlList.at(sn_It).toLocalFile());
      }

      // Check all paths
      for (sn_FileCounter = 0U; sn_FileCounter < c_PathList.size(); ++sn_FileCounter)
      {
         c_File.setFile(c_PathList[sn_FileCounter]);
         if ((c_File.exists() == false) || (c_File.isFile() == false))
         {
            q_FilesValid = false;
            break;
         }
      }

      if (q_FilesValid == true)
      {
         sintn sn_DatablockCounter;

         // Check all possible types
         for (sn_DatablockCounter = 0; sn_DatablockCounter < this->mc_DatablockWidgets.size(); ++sn_DatablockCounter)
         {
            q_Retval = this->mc_DatablockWidgets[sn_DatablockCounter]->CheckMime(c_PathList, orc_Pos,
                                                                                 opc_FilePathsDatablocks,
                                                                                 opc_FilePathsParamsets,
                                                                                 NULL,
                                                                                 oppc_App);

            if (q_Retval == true)
            {
               // If a datablock is possible, no other datablock is possible
               break;
            }
         }

         // If a datablock is possible, no other list is possible
         if ((q_Retval == false) &&
             (this->mpc_FilesWidget != NULL))
         {
            // Check if no file/parameter set image operation is possible or the caller wants to know all possibilities
            // In this case both checks are necessary
            q_Retval = this->mpc_FilesWidget->CheckMime(c_PathList, orc_Pos, opc_FilePathsFileBased,
                                                        opc_FilePathsParamsets,
                                                        opc_RelevantPemFilePaths,
                                                        oppc_App);
         }
      }
   }

   return q_Retval;
}
