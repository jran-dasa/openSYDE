//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Central handling for errors (implementation)

   Central handling for errors

   \copyright   Copyright 2016 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include <QEventLoop>
#include "stwerrors.h"
#include "constants.h"
#include "C_GtGetText.h"
#include "C_OgeWiError.h"
#include "C_Uti.h"
#include "C_OgePopUpDialog.h"
#include "C_PopErrorHandling.h"
#include "C_OSCLoggingHandler.h"
#include "C_OgeWiCustomMessage.h"
#include "C_OSCSystemDefinitionFiler.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */

using namespace stw_types;
using namespace stw_errors;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_core;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_gui_elements;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Load with error handling

   \param[in] ors32_Err                    Load error
   \param[in] orc_Path                     path of project to load
   \param[in] opc_Parent                   parent widget
   \param[in] ou16_SystemDefinitionVersion System definition version
*/
//----------------------------------------------------------------------------------------------------------------------
void C_PopErrorHandling::h_ProjectLoadErr(const sint32 & ors32_Err, const QString & orc_Path,
                                          QWidget * const opc_Parent, const uint16 ou16_SystemDefinitionVersion)
{
   if (ors32_Err == C_WARN)
   {
      // Do nothing here. Special case when a user cancels the password dialog for opening a service project.
      // Project file version is not known (because encrypted zip file), hence we don't want to hit the else
      // statement below.
   }
   else if ((ors32_Err != C_NO_ERR) && (ors32_Err != C_WARN))
   {
      QString c_Details;
      C_OgeWiCustomMessage c_Message(opc_Parent, C_OgeWiCustomMessage::eERROR);
      c_Message.SetHeading(C_GtGetText::h_GetText("Project load"));
      c_Message.SetDescription(C_GtGetText::h_GetText("Failed to load project: ") + orc_Path);
      c_Message.SetCustomMinHeight(250, 300);

      switch (ors32_Err)
      {
      case C_RD_WR:
         c_Details = C_GtGetText::h_GetText("There are problems accessing the file system.\n"
                                            "For example, there may be no read access to the file.");
         break;
      case C_RANGE:
         c_Details = C_GtGetText::h_GetText("At least one project file is missing.");
         break;
      case C_NOACT:
         c_Details = C_GtGetText::h_GetText("A project file is present but its structure is invalid.\n"
                                            "For example this can be caused by an invalid XML file.");
         break;
      case C_CONFIG:
         c_Details = C_GtGetText::h_GetText("The content of a project file is invalid or incomplete.");
         break;
      case C_CHECKSUM:
         c_Details = C_GtGetText::h_GetText("UI part does not match core part.");
         break;
      case C_COM:
         c_Details = C_GtGetText::h_GetText("The device definition for the project was not found.");
         break;
      case C_BUSY:
         c_Details = C_GtGetText::h_GetText("Problems with cleaning up temporary folders.");
         break;
      default:
         c_Details = C_GtGetText::h_GetText("Unknown cause.");
         break;
      }

      //Update log file
      C_OSCLoggingHandler::h_Flush();

      // Show error message
      c_Details += C_GtGetText::h_GetText("<br/>For more information see ");
      c_Details += C_Uti::h_GetLink(C_GtGetText::h_GetText("log file"), mc_STYLE_GUIDE_COLOR_LINK,
                                    C_OSCLoggingHandler::h_GetCompleteLogFileLocation().c_str());
      c_Details += ".";
      c_Message.SetDetails(c_Details);
      c_Message.Execute();
   }
   else
   {
      if ((ou16_SystemDefinitionVersion < C_OSCSystemDefinitionFiler::hu16_FILE_VERSION_LATEST) &&
          (orc_Path.isEmpty() == false))
      {
         //Handle version update
         C_OgeWiCustomMessage c_Message(opc_Parent, C_OgeWiCustomMessage::eINFORMATION);
         c_Message.SetHeading(C_GtGetText::h_GetText("Project load"));
         c_Message.SetDescription(C_GtGetText::h_GetText("Loading an older project version. On the next save the "
                                                         "project is updated to the new project version."));
         c_Message.SetCustomMinHeight(180, 180);
         c_Message.Execute();
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Save with error handling

   \param[in]   ors32_Err     Save error
   \param[in]   opc_Parent    parent widget
*/
//----------------------------------------------------------------------------------------------------------------------

void C_PopErrorHandling::h_ProjectSaveErr(const sint32 & ors32_Err, QWidget * const opc_Parent)
{
   if (ors32_Err != C_NO_ERR)
   {
      C_OgeWiCustomMessage c_Message(opc_Parent, C_OgeWiCustomMessage::E_Type::eERROR);
      c_Message.SetHeading(C_GtGetText::h_GetText("Project save"));
      switch (ors32_Err)
      {
      case C_RD_WR:
         c_Message.SetDescription(C_GtGetText::h_GetText("Problems accessing file system."));
         break;
      case C_RANGE:
         c_Message.SetDescription(C_GtGetText::h_GetText("Path is empty."));
         break;
      case C_NOACT:
         c_Message.SetDescription(C_GtGetText::h_GetText("Could not create project directory."));
         break;
      case C_COM:
         c_Message.SetDescription(C_GtGetText::h_GetText("Message sorting failed."));
         break;
      default:
         c_Message.SetDescription(C_GtGetText::h_GetText("Unknown cause."));
         break;
      }
      c_Message.SetCustomMinHeight(180, 180);
      c_Message.Execute();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  \brief   Create service project error handling

   \param[in]   ors32_Err     Save error
   \param[in]   opc_Parent    parent widget
*/
//----------------------------------------------------------------------------------------------------------------------
void C_PopErrorHandling::h_ServiceProjectSaveErr(const sint32 & ors32_Err, QWidget * const opc_Parent)
{
   if (ors32_Err != C_NO_ERR)
   {
      C_OgeWiCustomMessage c_Message(opc_Parent, C_OgeWiCustomMessage::E_Type::eERROR);
      c_Message.SetHeading(C_GtGetText::h_GetText("Create Service Project"));
      switch (ors32_Err)
      {
      case C_RANGE:
         c_Message.SetDescription(C_GtGetText::h_GetText("Invalid view configuration."));
         break;
      case C_RD_WR:
         c_Message.SetDescription(C_GtGetText::h_GetText("Problems accessing file system."));
         break;
      case C_NOACT:
         c_Message.SetDescription(C_GtGetText::h_GetText("Could not create temporary project directory."));
         break;
      case C_BUSY:
         c_Message.SetDescription(C_GtGetText::h_GetText("Could not delete temporary folder."));
         break;
      case C_COM:
         c_Message.SetDescription(C_GtGetText::h_GetText("Bus sorting failed."));
         break;
      case C_WARN:
         c_Message.SetDescription(C_GtGetText::h_GetText("No view or part of it is active."));
         break;
      case C_CONFIG:
         c_Message.SetDescription(C_GtGetText::h_GetText("Expected file for project generation missing."));
         break;
      default:
         c_Message.SetDescription(C_GtGetText::h_GetText("Unknown cause."));
         break;
      }
      c_Message.SetCustomMinHeight(180, 180);
      c_Message.Execute();
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Handle gettext initialize error

   \param[in]   ors32_Err   Error
*/
//----------------------------------------------------------------------------------------------------------------------
void C_PopErrorHandling::h_GetTextInitializeErr(const sint32 & ors32_Err)
{
   if (ors32_Err != C_NO_ERR)
   {
      C_OgeWiError * const pc_Widget = new C_OgeWiError();
      C_OgePopUpDialog c_Dialog;
      c_Dialog.SetTitle("Error occurred!");
      c_Dialog.SetWidget(pc_Widget);
      pc_Widget->SaveParentDialog(&c_Dialog);

      switch (ors32_Err)
      {
      case C_CONFIG:
         pc_Widget->SetErrorDescription(C_GtGetText::h_GetText("C_OSCGetText - h_Initialize:\n"
                                                               "orc_BasePath does not point to a valid directory"));
         c_Dialog.exec();
         break;
      case C_NOACT:
         pc_Widget->SetErrorDescription(C_GtGetText::h_GetText("C_OSCGetText - h_Initialize:\n"
                                                               "internal error in gettext library"));
         c_Dialog.exec();
         break;
      default:
         pc_Widget->SetErrorDescription(C_GtGetText::h_GetText("C_OSCGetText - h_Initialize: unknown error"));
         c_Dialog.exec();
         break;
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Constructor
*/
//----------------------------------------------------------------------------------------------------------------------
C_PopErrorHandling::C_PopErrorHandling()
{
}
