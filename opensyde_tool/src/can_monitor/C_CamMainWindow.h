//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Main window for project openSYDE CAN Monitor (header)

   See cpp file for detailed description

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_CAMMAINWINDOW_H
#define C_CAMMAINWINDOW_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QMainWindow>
#include <QMimeData>

#include "C_CamComDriverBase.h"

#include "CCAN.h"
#include "C_SyvComDriverThread.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace Ui
{
class C_CamMainWindow;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_CamMainWindow :
   public QMainWindow
{
   Q_OBJECT

public:
   explicit C_CamMainWindow(QWidget * const opc_Parent = NULL);
   ~C_CamMainWindow(void);

protected:
   virtual void closeEvent(QCloseEvent * const opc_Event) override;
   virtual void keyPressEvent(QKeyEvent * const opc_KeyEvent) override;
   virtual void dragEnterEvent(QDragEnterEvent * const opc_Event) override;
   virtual void dragMoveEvent(QDragMoveEvent * const opc_Event) override;
   virtual void dropEvent(QDropEvent * const opc_Event) override;

private:
   Ui::C_CamMainWindow * mpc_Ui;

   static const stw_types::sint32 mhs32_SettingsSplitterMax;
   static const stw_types::sint32 mhs32_MessageGenSplitterMax;

   stw_opensyde_gui_logic::C_CamComDriverBase mc_ComDriver;
   stw_can::C_CAN * mpc_CanDllDispatcher;
   stw_opensyde_gui_logic::C_SyvComDriverThread * mpc_CanThread;
   bool mq_LoggingStarted;

   // For loading files
   QString mc_CurrentLoadedFile;
   QString mc_CurrentLoadedFileOrg;
   std::vector<stw_scl::C_SCLString> mc_OsySysDefs;
   std::vector<stw_types::uint32> mc_OsySysDefBuses;
   std::vector<stw_scl::C_SCLString> mc_DbcFiles;

   // For custom splitter resizing
   stw_types::sint32 ms32_SettingsSplitterPosition;
   stw_types::sint32 ms32_MessageGenSplitterPosition;

   //Avoid call
   C_CamMainWindow(const C_CamMainWindow &);
   C_CamMainWindow & operator =(const C_CamMainWindow &);

   void m_StartLogging(void);
   void m_PauseLogging(void);
   void m_ContinueLogging(void);
   void m_StopLogging(void);

   stw_types::sint32 m_InitCan(stw_types::sint32 & ors32_Bitrate);
   void m_CloseCan(void);

   void m_LoadInitialProject(void);
   void m_LoadUserSettings(void);
   void m_SaveUserSettings(void);
   static bool mh_CheckMime(const QMimeData * const opc_Mime, QString * const opc_FilePath = NULL);

   static void mh_ThreadFunc(void * const opv_Instance);
   void m_ThreadFunc(void);

   void m_OnExpandSettings(const bool oq_Expand);
   void m_OnSettingsSplitterHandleDoubleClick(void) const;
   void m_OnSettingsSplitterMoved(const stw_types::sint32 & ors32_Pos, const stw_types::sint32 & ors32_Index);

   void m_OnExpandMessageGen(const bool oq_Expand);
   void m_OnMessageGenSplitterHandleDoubleClick(void) const;
   void m_OnMessageGenSplitterMoved(const stw_types::sint32 & ors32_Pos, const stw_types::sint32 & ors32_Index);

   void m_RegisterCyclicMessage(const stw_types::uint32 ou32_MessageIndex, const bool oq_Active);
   void m_SendMessage(const stw_types::uint32 ou32_MessageIndex, const stw_types::uint32 ou32_TimeToSend);
   void m_RemoveAllCyclicMessages(void);

   void m_OnClearOldConfiguration(void) const;
   void m_OnNewConfiguration(void);
   void m_OnDatabaseLoadStarted(const QString & orc_File, const QString & orc_OrgPath);
   void m_OnDatabaseLoadFinished(const stw_types::sint32 os32_Result);
   void m_OnDatabaseRemove(const QString & orc_File, const QString & orc_OrgPath, const bool oq_IsUpdate);
   void m_OnActivateDatabase(const QString & orc_File, const QString & orc_OrgPath, const bool oq_Active);
   void m_OnOsyChangeBus(const QString & orc_File, const QString & orc_OrgPath, const stw_types::uint32 ou32_BusIndex);
   void m_OnCanDllConfigChange(void);
   void m_CheckMessagesForLoadedDatabase(const QString & orc_DatabasePath);
   void m_CheckForLastDatabaseLoaded(const QString & orc_DatabasePath);
   void m_DisplayCheckMessagesDialog(const QString & orc_DatabasePath,
                                     const std::vector<stw_types::uint32> & orc_Indices);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
