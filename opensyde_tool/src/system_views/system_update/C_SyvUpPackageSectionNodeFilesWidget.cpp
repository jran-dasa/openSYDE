//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for listing and handling any files

   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include <QFileInfo>

#include "stwtypes.h"
#include "stwerrors.h"
#include "constants.h"
#include "C_GtGetText.h"
#include "TGLUtils.h"
#include "C_ImpUtil.h"
#include "C_OSCLoggingHandler.h"

#include "C_SyvUpPackageSectionNodeFilesWidget.h"
#include "ui_C_SyvUpPackageSectionNodeWidget.h"
#include "C_SyvUpPackageListNodeItemFileWidget.h"
#include "C_SyvUpPackageListNodeItemParamSetWidget.h"
#include "C_SyvUpPackageListNodeItemPemFileWidget.h"
#include "C_PuiSvNodeUpdateParamInfo.h"
#include "C_SyvUpPemFileOptionsPopUp.h"

#include "C_OgeWiCustomMessage.h"
#include "C_PuiSdHandler.h"
#include "C_PuiSvHandler.h"
#include "C_PuiSvData.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_errors;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_gui_elements;
using namespace stw_opensyde_core;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Default constructor

   Set up GUI with all elements.

   \param[in,out]  opc_Parent    Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpPackageSectionNodeFilesWidget::C_SyvUpPackageSectionNodeFilesWidget(QWidget * const opc_Parent) :
   C_SyvUpPackageSectionNodeWidget(opc_Parent),
   mu32_CountSkippedParamSetFiles(0U)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Adds a new file for the file container

   Only possible for file based devices

   \param[in]  orc_File    New path
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPackageSectionNodeFilesWidget::AddFile(const QString & orc_File)
{
   const bool q_ParamSet = mh_IsFileParamSetFile(orc_File);
   const bool q_PemFile = mh_IsFilePemFile(orc_File);

   if ((q_ParamSet == false) &&
       (q_PemFile == false))
   {
      const C_OSCNode * const pc_Node = C_PuiSdHandler::h_GetInstance()->GetOSCNodeConst(this->mu32_NodeIndex);
      const C_PuiSvData * const pc_View = C_PuiSvHandler::h_GetInstance()->GetView(this->mu32_ViewIndex);

      if ((pc_Node != NULL) &&
          (pc_View != NULL))
      {
         // Add the file
         C_SyvUpPackageListNodeItemFileWidget * const pc_FileWidget =
            new C_SyvUpPackageListNodeItemFileWidget(this->mu32_ViewIndex, this->mu32_NodeIndex, this->mc_DeviceType,
                                                     this->mq_FileBased, this->mq_StwFlashloader, this);

         // Pre initialization of the size. If this is not set, the eliding of the path label will cause a
         // visible resizing
         pc_FileWidget->setGeometry(this->width(), 23, 0, 0);

         pc_FileWidget->SetAppFile(orc_File, false);
         pc_FileWidget->SetAppNumber(this->mu32_PrimaryFileCount);
         pc_FileWidget->SetOwnerSectionName(this->mc_SectionName);

         this->mpc_Ui->pc_FileVerticalLayout->insertWidget(this->mpc_Ui->pc_FileVerticalLayout->count(),
                                                           pc_FileWidget);

         // Save the new file as application path
         tgl_assert(C_PuiSvHandler::h_GetInstance()->AddNodeUpdateInformationPath(
                       this->mu32_ViewIndex, this->mu32_NodeIndex, orc_File,
                       C_PuiSvNodeUpdate::eFTP_FILE_BASED) == C_NO_ERR);

         ++this->mu32_PrimaryFileCount;
         ++this->mu32_FileCount;
      } //lint !e429  //no memory leak because of the parent of pc_AppWidget and the Qt memory management
   }
   else if (q_PemFile == true)
   {
      tgl_assert(q_ParamSet == false);
      if (this->mu32_PemFileCount == 0)
      {
         C_SyvUpPackageListNodeItemPemFileWidget * const pc_PemWidget =
            new C_SyvUpPackageListNodeItemPemFileWidget(this->mu32_ViewIndex, this->mu32_NodeIndex,
                                                        this->mc_DeviceType,
                                                        this->mq_FileBased, this->mq_StwFlashloader, this);
         // Set default values
         const C_PuiSvNodeUpdate::E_StateSecurity e_StateSecurity = C_PuiSvNodeUpdate::eST_SEC_ACTIVATE;
         const C_PuiSvNodeUpdate::E_StateDebugger e_StateDebugger = C_PuiSvNodeUpdate::eST_DEB_NO_CHANGE;

         pc_PemWidget->SetAppFile(orc_File, false);
         pc_PemWidget->SetAppNumber(0U);
         pc_PemWidget->SetOwnerSectionName(this->mc_SectionName);
         pc_PemWidget->SetPemStates(e_StateSecurity, e_StateDebugger);

         this->mpc_Ui->pc_FileVerticalLayout->insertWidget(this->mu32_PemFileCount,
                                                           pc_PemWidget);

         // Save the new file as application path
         tgl_assert(C_PuiSvHandler::h_GetInstance()->SetNodeUpdateInformationPemFilePath(
                       this->mu32_ViewIndex, this->mu32_NodeIndex, orc_File) == C_NO_ERR);
         // Set the new default states as well
         tgl_assert(C_PuiSvHandler::h_GetInstance()->SetNodeUpdateInformationStates(
                       this->mu32_ViewIndex, this->mu32_NodeIndex,
                       e_StateSecurity, e_StateDebugger) == C_NO_ERR);

         ++this->mu32_PemFileCount;
         ++this->mu32_FileCount;
      } //lint !e429  //no memory leak because of the parent of pc_AppWidget and the Qt memory management
      else
      {
         // Only one PEM file allowed
         const QString c_Message = static_cast<QString>(
            C_GtGetText::h_GetText("A PEM file was already added. "
                                   "Only one PEM file for a node is allowed. The file %1 was not added."))
                                   .arg(orc_File);
         C_OgeWiCustomMessage c_MessageBox(this, C_OgeWiCustomMessage::E_Type::eWARNING);
         c_MessageBox.SetDescription(c_Message);
         c_MessageBox.SetHeading(C_GtGetText::h_GetText("Adding PEM file"));
         c_MessageBox.Execute();
      }
   }
   else
   {
      C_PuiSvNodeUpdateParamInfo c_ParamFileInfo;
      if (this->m_GetParamsetFileInfo(orc_File, c_ParamFileInfo) == C_NO_ERR)
      {
         C_SyvUpPackageListNodeItemParamSetWidget * const pc_ParamWidget =
            new C_SyvUpPackageListNodeItemParamSetWidget(this->mu32_ViewIndex, this->mu32_NodeIndex,
                                                         this->mc_DeviceType,
                                                         this->mq_FileBased,
                                                         this->mq_StwFlashloader,
                                                         this);

         // add file
         // Pre initialization of the size. If this is not set, the eliding of the path label will cause a
         // visible resizing
         pc_ParamWidget->setGeometry(this->width(), 40, 0, 0);

         pc_ParamWidget->SetParamInfo(c_ParamFileInfo);
         pc_ParamWidget->SetAppNumber(this->mu32_CountSkippedParamSetFiles + this->mu32_ParamSetFileCount);
         pc_ParamWidget->SetOwnerSectionName(this->mc_SectionName);

         // Inserting psi files before all "other" files
         this->mpc_Ui->pc_FileVerticalLayout->insertWidget(static_cast<uint32>(this->mu32_PemFileCount +
                                                                               this->mu32_ParamSetFileCount),
                                                           pc_ParamWidget);

         // Save the new file as parameter path
         tgl_assert(C_PuiSvHandler::h_GetInstance()->AddNodeUpdateInformationParamInfo(
                       this->mu32_ViewIndex,
                       this->mu32_NodeIndex,
                       c_ParamFileInfo) == C_NO_ERR);

         ++this->mu32_ParamSetFileCount;
         ++this->mu32_FileCount;
      } //lint !e429  //no memory leak because of the parent of pc_ParamWidget and the Qt memory management
   }

   this->mpc_Ui->pc_WidgetAdd->setVisible(false);

   this->m_FileCountChanged();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Sets a new file for the application

   \param[in]  orc_File    New path
   \param[in]  opc_App     Application widget
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPackageSectionNodeFilesWidget::AdaptFile(const QString & orc_File,
                                                     C_SyvUpPackageListNodeItemWidget * const opc_App)
{
   const bool q_NewFileParamSet = mh_IsFileParamSetFile(orc_File);
   const bool q_NewFilePem = mh_IsFilePemFile(orc_File);

   if (opc_App != NULL)
   {
      if (((opc_App->GetType() == mu32_UPDATE_PACKAGE_NODE_SECTION_TYPE_FILE) &&
           (q_NewFileParamSet == false) && (q_NewFilePem == false)) ||
          ((opc_App->GetType() == mu32_UPDATE_PACKAGE_NODE_SECTION_TYPE_PARAMSET) &&
           (q_NewFileParamSet == true) && (q_NewFilePem == false)) ||
          ((opc_App->GetType() == mu32_UPDATE_PACKAGE_NODE_SECTION_TYPE_PEM) &&
           (q_NewFileParamSet == false) && (q_NewFilePem == true)))
      {
         if ((q_NewFileParamSet == false) &&
             (q_NewFilePem == false))
         {
            C_SyvUpPackageSectionNodeWidget::AdaptFile(orc_File, opc_App);
            tgl_assert(C_PuiSvHandler::h_GetInstance()->SetNodeUpdateInformationPath(
                          this->mu32_ViewIndex, this->mu32_NodeIndex, opc_App->GetAppNumber(), orc_File,
                          C_PuiSvNodeUpdate::eFTP_FILE_BASED) == C_NO_ERR);
         }
         else if (q_NewFilePem == true)
         {
            C_SyvUpPackageSectionNodeWidget::AdaptFile(orc_File, opc_App);
            tgl_assert(C_PuiSvHandler::h_GetInstance()->SetNodeUpdateInformationPemFilePath(
                          this->mu32_ViewIndex, this->mu32_NodeIndex, orc_File) == C_NO_ERR);
         }
         else
         {
            this->m_AdaptParamSetFile(orc_File, opc_App);
         }
      }
      else
      {
         // The type of item type is changed, remove the old one and add it as new
         if ((q_NewFilePem == false) ||
             (this->mu32_PemFileCount == 0U))
         {
            // In case of a new PEM file no other PEM may exist already
            this->RemoveFile(opc_App);
            this->AddFile(orc_File);
         }
         else
         {
            // Only one PEM file allowed
            const QString c_Message = static_cast<QString>(C_GtGetText::h_GetText("A PEM file was already added. "
                                                                                  "Only one PEM file for a node is allowed. The file %1 was not added."))
                                      .arg(orc_File);
            C_OgeWiCustomMessage c_MessageBox(this, C_OgeWiCustomMessage::E_Type::eWARNING);
            c_MessageBox.SetDescription(c_Message);
            c_MessageBox.SetHeading(C_GtGetText::h_GetText("Adding PEM file"));
            c_MessageBox.Execute();
         }
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Sets a new file for the application

   \param[in]  oq_Skip  New skip flag
   \param[in]  opc_App  Application widget
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPackageSectionNodeFilesWidget::SetSkipOfUpdateFile(const bool oq_Skip,
                                                               C_SyvUpPackageListNodeItemWidget * const opc_App)
{
   if (opc_App != NULL)
   {
      C_SyvUpPackageSectionNodeWidget::SetSkipOfUpdateFile(oq_Skip, opc_App);

      if (opc_App->GetType() == mu32_UPDATE_PACKAGE_NODE_SECTION_TYPE_FILE)
      {
         tgl_assert(C_PuiSvHandler::h_GetInstance()->SetNodeUpdateInformationSkipUpdateOfPath(
                       this->mu32_ViewIndex,
                       this->mu32_NodeIndex,
                       opc_App->GetAppNumber(), oq_Skip,
                       C_PuiSvNodeUpdate::eFTP_FILE_BASED) == C_NO_ERR);
      }
      else if (opc_App->GetType() == mu32_UPDATE_PACKAGE_NODE_SECTION_TYPE_PEM)
      {
         tgl_assert(C_PuiSvHandler::h_GetInstance()->SetNodeUpdateInformationSkipUpdateOfPemFile(
                       this->mu32_ViewIndex,
                       this->mu32_NodeIndex,
                       oq_Skip) == C_NO_ERR);
      }
      else
      {
         tgl_assert(C_PuiSvHandler::h_GetInstance()->SetNodeUpdateInformationSkipUpdateOfParamInfo(
                       this->mu32_ViewIndex,
                       this->mu32_NodeIndex,
                       opc_App->GetAppNumber(), oq_Skip) == C_NO_ERR);
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Removes the current application

   \param[in]  opc_App  Application widget
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPackageSectionNodeFilesWidget::RemoveFile(C_SyvUpPackageListNodeItemWidget * const opc_App)
{
   if (opc_App != NULL)
   {
      const uint32 u32_Number = opc_App->GetAppNumber();
      if (opc_App->GetType() == mu32_UPDATE_PACKAGE_NODE_SECTION_TYPE_FILE)
      {
         // Remove the application file path
         tgl_assert(C_PuiSvHandler::h_GetInstance()->RemoveNodeUpdateInformationPath(this->mu32_ViewIndex,
                                                                                     this->mu32_NodeIndex,
                                                                                     u32_Number,
                                                                                     C_PuiSvNodeUpdate::
                                                                                     eFTP_FILE_BASED) == C_NO_ERR);
      }
      else if (opc_App->GetType() == mu32_UPDATE_PACKAGE_NODE_SECTION_TYPE_PEM)
      {
         tgl_assert(C_PuiSvHandler::h_GetInstance()->RemoveNodeUpdateInformationPemFilePath(
                       this->mu32_ViewIndex,
                       this->mu32_NodeIndex) == C_NO_ERR);
      }
      else
      {
         tgl_assert(C_PuiSvHandler::h_GetInstance()->RemoveNodeUpdateInformationParamInfo(this->mu32_ViewIndex,
                                                                                          this->mu32_NodeIndex,
                                                                                          u32_Number) == C_NO_ERR);
      }
   }

   C_SyvUpPackageSectionNodeWidget::RemoveFile(opc_App);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Opens the settings for a PEM file

   \param[in]  opc_App  Application widget
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPackageSectionNodeFilesWidget::OpenPemFileSettings(C_SyvUpPackageListNodeItemWidget * const opc_App)
{
   C_SyvUpPackageListNodeItemPemFileWidget * const pc_PemApp =
      dynamic_cast<C_SyvUpPackageListNodeItemPemFileWidget *>(opc_App);

   if (pc_PemApp != NULL)
   {
      C_PuiSvNodeUpdate::E_StateSecurity e_StateSecurity;
      C_PuiSvNodeUpdate::E_StateDebugger e_StateDebugger;
      // Get the current PEM states
      pc_PemApp->GetPemStates(e_StateSecurity, e_StateDebugger);

      // Adapt the states
      {
         QPointer<C_OgePopUpDialog> const c_New = new C_OgePopUpDialog(this, this);
         C_SyvUpPemFileOptionsPopUp * const pc_InfoDialog = new C_SyvUpPemFileOptionsPopUp(*c_New, e_StateSecurity,
                                                                                           e_StateDebugger);

         //Resize
         c_New->SetSize(QSize(500, 400));

         if (c_New->exec() == static_cast<sintn>(QDialog::Accepted))
         {
            //No confirmation
            e_StateSecurity = pc_InfoDialog->GetComboBoxSecState();
            e_StateDebugger = pc_InfoDialog->GetComboBoxDebState();
         }

         if (c_New != NULL)
         {
            c_New->HideOverlay();
            c_New->deleteLater();
         }
      } //lint !e429  //no memory leak because of the parent of pc_InfoDialog and the Qt memory management

      pc_PemApp->SetPemStates(e_StateSecurity, e_StateDebugger);
      tgl_assert(C_PuiSvHandler::h_GetInstance()->SetNodeUpdateInformationStates(this->mu32_ViewIndex,
                                                                                 this->mu32_NodeIndex,
                                                                                 e_StateSecurity,
                                                                                 e_StateDebugger) == C_NO_ERR);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Prepares the update package node configuration

   \param[out]  orc_NodeConfig   Node configuration
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPackageSectionNodeFilesWidget::PrepareExportConfig(C_SyvUpUpdatePackageConfigNode & orc_NodeConfig) const
{
   if (this->mpc_Ui->pc_FileVerticalLayout->count() > 0)
   {
      sintn sn_Counter;
      orc_NodeConfig.c_FileConfigs.reserve(this->mu32_PrimaryFileCount);
      orc_NodeConfig.c_ParamSetConfigs.reserve(orc_NodeConfig.c_ParamSetConfigs.size() + this->mu32_ParamSetFileCount);

      for (sn_Counter = 0; sn_Counter < this->mpc_Ui->pc_FileVerticalLayout->count(); ++sn_Counter)
      {
         QLayoutItem * const pc_Item = this->mpc_Ui->pc_FileVerticalLayout->itemAt(sn_Counter);

         if (pc_Item != NULL)
         {
            C_SyvUpPackageListNodeItemWidget * const pc_App =
               dynamic_cast<C_SyvUpPackageListNodeItemWidget *>(pc_Item->widget());

            if (pc_App != NULL)
            {
               if (pc_App->GetType() == mu32_UPDATE_PACKAGE_NODE_SECTION_TYPE_PEM)
               {
                  C_SyvUpPackageListNodeItemPemFileWidget * const pc_PemApp =
                     dynamic_cast<C_SyvUpPackageListNodeItemPemFileWidget *>(pc_App);

                  if (pc_PemApp != NULL)
                  {
                     orc_NodeConfig.c_PemFilePath = pc_App->GetAppFilePath();
                     pc_PemApp->GetPemStates(orc_NodeConfig.e_StateSecurity, orc_NodeConfig.e_StateDebugger);
                  }
               }
               else if (pc_App->GetType() != mu32_UPDATE_PACKAGE_NODE_SECTION_TYPE_PARAMSET)
               {
                  orc_NodeConfig.c_FileConfigs.push_back(pc_App->GetAppFilePath());
               }
               else
               {
                  // Special case: PSI File
                  orc_NodeConfig.c_ParamSetConfigs.push_back(pc_App->GetAppFilePath());
               }
            }
         }
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Loads all matching configurations for this node of the import configuration

   \param[out]  orc_Config    Import configuration
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPackageSectionNodeFilesWidget::LoadImportConfig(const C_SyvUpUpdatePackageConfig & orc_Config)
{
   uint32 u32_ConfigCounter;

   // Search the matching configuration
   for (u32_ConfigCounter = 0; u32_ConfigCounter < orc_Config.c_NodeConfigs.size(); ++u32_ConfigCounter)
   {
      if ((orc_Config.c_NodeConfigs[u32_ConfigCounter].c_Name == this->mc_NodeName) &&
          (orc_Config.c_NodeConfigs[u32_ConfigCounter].c_DeviceType == this->mc_DeviceType))
      {
         uintn un_FileCounter;

         // Remove the previous applications
         this->RemoveAllFiles();

         // Add the imported configuration as new parameter set image files
         for (un_FileCounter = this->mu32_CountSkippedParamSetFiles;
              un_FileCounter < orc_Config.c_NodeConfigs[u32_ConfigCounter].c_ParamSetConfigs.size();
              ++un_FileCounter)
         {
            const QString c_Path = orc_Config.c_NodeConfigs[u32_ConfigCounter].c_ParamSetConfigs[un_FileCounter];

            if (mh_IsFileParamSetFile(c_Path) == true)
            {
               // Update UI and update view data handling
               this->AddFile(c_Path);
            }
         }

         // Add the imported configuration as new applications
         if (this->mq_FileBased == true)
         {
            for (un_FileCounter = 0U;
                 un_FileCounter < orc_Config.c_NodeConfigs[u32_ConfigCounter].c_FileConfigs.size();
                 ++un_FileCounter)
            {
               // Update UI and update view data handling
               this->AddFile(orc_Config.c_NodeConfigs[u32_ConfigCounter].c_FileConfigs[un_FileCounter]);
            }
         }

         if (!orc_Config.c_NodeConfigs[u32_ConfigCounter].c_PemFilePath.isEmpty())
         {
            this->AddFile(orc_Config.c_NodeConfigs[u32_ConfigCounter].c_PemFilePath);
            //Configure PEM states
            QLayoutItem * const pc_Item = this->mpc_Ui->pc_FileVerticalLayout->itemAt(0);

            if (pc_Item != NULL)
            {
               C_SyvUpPackageListNodeItemPemFileWidget * const pc_PemApp =
                  dynamic_cast<C_SyvUpPackageListNodeItemPemFileWidget *>(pc_Item->widget());

               if (pc_PemApp != NULL)
               {
                  pc_PemApp->SetPemStates(orc_Config.c_NodeConfigs[u32_ConfigCounter].e_StateSecurity,
                                          orc_Config.c_NodeConfigs[u32_ConfigCounter].e_StateDebugger);
               }
            }
         }

         break;
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Check if mime valid

   File based list accept multiple files

   \param[in,out]  orc_PathList                    File paths to analyze, relevant paths in opc_RelevantFilePaths in
                                                   will be removed here
   \param[in]      orc_Pos                         Mouse position
   \param[out]     opc_RelevantFilePaths           File paths which could be used for this list
   \param[out]     opc_RelevantParamSetImagePaths  Parameter set image paths which could be used for this list
   \param[out]     opc_RelevantPemFilePaths        PEM file paths which could be used for this list
   \param[out]     oppc_App                        Found application widget

   \return
   true  Valid
   false Invalid
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_SyvUpPackageSectionNodeFilesWidget::CheckMime(QStringList & orc_PathList, const QPoint & orc_Pos,
                                                     QStringList * const opc_RelevantFilePaths,
                                                     QStringList * const opc_RelevantParamSetImagePaths,
                                                     QStringList * const opc_RelevantPemFilePaths,
                                                     C_SyvUpPackageListNodeItemWidget ** const oppc_App) const
{
   bool q_Retval = false;

   if (orc_PathList.size() > 0)
   {
      C_SyvUpPackageListNodeItemWidget * pc_App = NULL;

      q_Retval = C_SyvUpPackageSectionNodeWidget::CheckMime(orc_PathList, orc_Pos, opc_RelevantFilePaths,
                                                            opc_RelevantParamSetImagePaths, opc_RelevantPemFilePaths,
                                                            &pc_App);

      if (q_Retval == true)
      {
         sintn sn_PathCounter = 0;

         q_Retval = false;

         // The information of the return value is relevant only
         // Assign all file paths to the parameter set files if it is a file with the correct extension or
         // add it to the "other files"
         do
         {
            const QString & rc_CurrentPath = orc_PathList[sn_PathCounter];

            if (mh_IsFileParamSetFile(rc_CurrentPath) == true)
            {
               if (opc_RelevantParamSetImagePaths != NULL)
               {
                  opc_RelevantParamSetImagePaths->push_back(rc_CurrentPath);
               }
               orc_PathList.erase(orc_PathList.begin() + sn_PathCounter);
               q_Retval = true;
            }
            else if (mh_IsFilePemFile(rc_CurrentPath) == true)
            {
               if (opc_RelevantPemFilePaths != NULL)
               {
                  opc_RelevantPemFilePaths->push_back(rc_CurrentPath);
               }
               orc_PathList.erase(orc_PathList.begin() + sn_PathCounter);
               q_Retval = true;
            }
            else if (this->mq_FileBased == true)
            {
               if (opc_RelevantFilePaths != NULL)
               {
                  opc_RelevantFilePaths->push_back(rc_CurrentPath);
               }
               orc_PathList.erase(orc_PathList.begin() + sn_PathCounter);
               q_Retval = true;
            }
            else
            {
               // File not relevant
               ++sn_PathCounter;
            }
         }
         while (sn_PathCounter < orc_PathList.size());

         if ((q_Retval == true) &&
             (oppc_App != NULL))
         {
            *oppc_App = pc_App;
         }
      }
   }

   return q_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get type of subsection (data block/parameter set file/file based node file)

   \retval type file of a file based node
*/
//----------------------------------------------------------------------------------------------------------------------
uint32 C_SyvUpPackageSectionNodeFilesWidget::Type(void) const
{
   return mu32_UPDATE_PACKAGE_NODE_SECTION_TYPE_FILE;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Sets the number of skipped parameter set image files

   \param[in]  ou32_CountSkippedFiles  Number of parameter set image files which are used by the
                                       datablock sections
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPackageSectionNodeFilesWidget::SetCountSkippedParamSetFiles(const uint32 ou32_CountSkippedFiles)
{
   this->mu32_CountSkippedParamSetFiles = ou32_CountSkippedFiles;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Initialization of the applications in case of datablock configuration

   \param[in]  orc_Node          Current node
   \param[in]  orc_UpdateInfo    Configured update configuration of view
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPackageSectionNodeFilesWidget::m_InitSpecificItem(const stw_opensyde_core::C_OSCNode & orc_Node,
                                                              const stw_opensyde_gui_logic::C_PuiSvNodeUpdate & orc_UpdateInfo)
{
   const std::vector<QString> c_ViewAppPaths = orc_UpdateInfo.GetPaths(C_PuiSvNodeUpdate::eFTP_FILE_BASED);
   const std::vector<C_PuiSvNodeUpdateParamInfo> c_ViewParamsetPaths = orc_UpdateInfo.GetParamInfos();
   const QString c_ViewPemPath = orc_UpdateInfo.GetPemFilePath();

   std::vector<bool> c_ViewFileSkipFlags = orc_UpdateInfo.GetSkipUpdateOfPathsFlags(
      C_PuiSvNodeUpdate::eFTP_FILE_BASED);
   std::vector<bool> c_ViewParamSetSkipFlags = orc_UpdateInfo.GetSkipUpdateOfParamInfosFlags();
   const bool q_ViewPemSkipFlag = orc_UpdateInfo.GetSkipUpdateOfPemFile();
   uint32 u32_Counter;

   Q_UNUSED(orc_Node)

   if (c_ViewFileSkipFlags.size() != c_ViewAppPaths.size())
   {
      c_ViewFileSkipFlags.resize(c_ViewAppPaths.size(), false);
   }
   if (c_ViewParamSetSkipFlags.size() != c_ViewParamsetPaths.size())
   {
      c_ViewParamSetSkipFlags.resize(c_ViewParamsetPaths.size(), false);
   }

   // PEM file first
   if (c_ViewPemPath != "")
   {
      C_SyvUpPackageListNodeItemPemFileWidget * const pc_PemWidget =
         new C_SyvUpPackageListNodeItemPemFileWidget(this->mu32_ViewIndex, this->mu32_NodeIndex,
                                                     this->mc_DeviceType,
                                                     this->mq_FileBased, this->mq_StwFlashloader, this);
      C_PuiSvNodeUpdate::E_StateSecurity e_StateSecurity;
      C_PuiSvNodeUpdate::E_StateDebugger e_StateDebugger;

      pc_PemWidget->SetAppFile(c_ViewPemPath, false);
      pc_PemWidget->SetSkipOfUpdateFile(q_ViewPemSkipFlag);
      pc_PemWidget->SetAppNumber(0U);
      pc_PemWidget->SetOwnerSectionName(this->mc_SectionName);

      orc_UpdateInfo.GetStates(e_StateSecurity, e_StateDebugger);
      pc_PemWidget->SetPemStates(e_StateSecurity, e_StateDebugger);

      ++this->mu32_FileCount;
      this->mu32_PemFileCount = 1U;
      this->mpc_Ui->pc_FileVerticalLayout->addWidget(pc_PemWidget);
   } //lint !e429  //no memory leak because of the parent of pc_AppWidget and the Qt memory management
   else
   {
      this->mu32_PemFileCount = 0U;
   }

   // PSI files second
   for (u32_Counter = this->mu32_CountSkippedParamSetFiles; u32_Counter < c_ViewParamsetPaths.size(); ++u32_Counter)
   {
      C_PuiSvNodeUpdateParamInfo c_ParamInfo;
      const C_PuiSvNodeUpdateParamInfo & rc_FileInfo = c_ViewParamsetPaths[u32_Counter];
      C_SyvUpPackageListNodeItemParamSetWidget * const pc_ParamWidget =
         new C_SyvUpPackageListNodeItemParamSetWidget(this->mu32_ViewIndex, this->mu32_NodeIndex,
                                                      this->mc_DeviceType,
                                                      this->mq_FileBased, this->mq_StwFlashloader, this);

      pc_ParamWidget->SetAppFile(rc_FileInfo.GetPath(), false);
      pc_ParamWidget->SetSkipOfUpdateFile(c_ViewParamSetSkipFlags[u32_Counter]);
      pc_ParamWidget->SetAppNumber(u32_Counter);
      //Set initial param set info
      c_ParamInfo.SetContent(rc_FileInfo.GetPath(), rc_FileInfo.GetLastKnownCrc());
      pc_ParamWidget->SetParamInfo(c_ParamInfo);
      pc_ParamWidget->SetOwnerSectionName(this->mc_SectionName);

      ++this->mu32_FileCount;
      ++this->mu32_ParamSetFileCount;
      this->mpc_Ui->pc_FileVerticalLayout->addWidget(pc_ParamWidget);
   } //lint !e429  //no memory leak because of the parent of pc_AppWidget and the Qt memory management

   // All other files at the end
   for (u32_Counter = 0U; u32_Counter < c_ViewAppPaths.size(); ++u32_Counter)
   {
      C_SyvUpPackageListNodeItemFileWidget * const pc_AppWidget =
         new C_SyvUpPackageListNodeItemFileWidget(this->mu32_ViewIndex, this->mu32_NodeIndex,
                                                  this->mc_DeviceType,
                                                  this->mq_FileBased, this->mq_StwFlashloader, this);

      pc_AppWidget->SetAppFile(c_ViewAppPaths[u32_Counter], false);
      pc_AppWidget->SetSkipOfUpdateFile(c_ViewFileSkipFlags[u32_Counter]);
      pc_AppWidget->SetAppNumber(u32_Counter);
      pc_AppWidget->SetOwnerSectionName(this->mc_SectionName);

      ++this->mu32_FileCount;
      ++this->mu32_PrimaryFileCount;
      this->mpc_Ui->pc_FileVerticalLayout->addWidget(pc_AppWidget);
   } //lint !e429  //no memory leak because of the parent of pc_AppWidget and the Qt memory management

   this->m_UpdateTitle();
   this->mq_ShowAddButton = true;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize all displayed static names
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPackageSectionNodeFilesWidget::m_InitStaticNames(void)
{
   this->mc_SectionName = C_GtGetText::h_GetText("Other Files (Optional)");
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Update of the title label
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPackageSectionNodeFilesWidget::m_UpdateTitle(void)
{
   this->mc_Title = this->mc_SectionName;
   C_SyvUpPackageSectionNodeWidget::m_UpdateTitle();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Adapts the number of a param set file if necessary

   In case of param set files in the files widget, the skipped count must be used for the real number of
   the param set file

   \param[in]  ou32_Number    Unedited number of param set file

   \return
   Adapted number of param set file
*/
//----------------------------------------------------------------------------------------------------------------------
uint32 C_SyvUpPackageSectionNodeFilesWidget::m_AdaptParamSetNumber(const uint32 ou32_Number)
{
   return (this->mu32_CountSkippedParamSetFiles + ou32_Number);
}
