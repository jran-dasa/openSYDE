//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Max performance view (header)

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     28.08.2018  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_CAMMETTREEVIEW_H
#define C_CAMMETTREEVIEW_H

/* -- Includes ------------------------------------------------------------- */
#include <mutex>
#include <QTimer>
#include <QThread>
#include <QTreeView>
#include <QAction>

#include "C_CamMetTreeModel.h"
#include "C_CamMetTreeDelegate.h"
#include "C_CamMetTreeGUIBuffer.h"
#include "C_OgePubIconOnly.h"
#include "C_SyvComMessageMonitor.h"
#include "C_CamMetTreeLoggerData.h"
#include "C_OgeTreeViewToolTipBase.h"
#include "C_OgeContextMenu.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_CamMetTreeView :
   public stw_opensyde_gui_elements::C_OgeTreeViewToolTipBase,
   public stw_opensyde_gui_logic::C_SyvComMessageMonitor
{
   Q_OBJECT

public:
   C_CamMetTreeView(QWidget * const opc_Parent = NULL);
   ~C_CamMetTreeView(void);

   virtual void Continue(void) override;
   virtual void Pause(void) override;
   virtual void Stop(void) override;
   virtual void Start(void) override;

   void ActionClearData(void);
   virtual void SetProtocol(const stw_cmon_protocol::e_CMONL7Protocols oe_Protocol) override;
   void SetDisplayTree(const bool oq_Value);
   void SetDisplayUniqueMessages(const bool oq_Value);
   void SetDisplayAsHex(const bool oq_Value);
   void SetDisplayTimestampRelative(const bool oq_Value);
   bool GetDisplayAsHex(void);
   bool GetDisplayTimestampRelative(void);

   void SaveUserSettings(void) const;
   void SetCurrentColumnWidths(const std::vector<stw_types::sint32> & orc_ColumnWidths);
   std::vector<stw_types::sint32> GetCurrentColumnWidths(void) const;

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void showEvent(QShowEvent * const opc_Event) override;
   virtual void resizeEvent(QResizeEvent * const opc_Event) override;
   virtual void selectionChanged(const QItemSelection & orc_Selected, const QItemSelection & orc_Deselected);
   virtual void keyPressEvent(QKeyEvent * const opc_Event) override;
   virtual void drawBranches(QPainter * const opc_Painter, const QRect & orc_Rect,
                             const QModelIndex & orc_Index) const override;
   //lint -restore

private:
   static const QString mhc_Stylesheet;
   std::mutex mc_MutexUpdate;
   QThread mc_ThreadGUIBuffer;
   QTimer mc_TimerHandleMessages;
   stw_opensyde_gui_logic::C_CamMetTreeModel mc_Model;
   stw_opensyde_gui_logic::C_CamMetTreeDelegate mc_Delegate;
   stw_opensyde_gui_logic::C_CamMetTreeGUIBuffer mc_GUIBuffer;
   stw_opensyde_gui_elements::C_OgePubIconOnly * mpc_PushButtonScrollTop;
   stw_opensyde_gui_elements::C_OgePubIconOnly * mpc_PushButtonScrollBottom;
   stw_opensyde_gui_elements::C_OgeContextMenu * mpc_ContextMenu;
   QAction * mpc_ActionCopy;
   QAction * mpc_ActionExpandAll;
   QAction * mpc_ActionCollapseAll;
   bool mq_UniqueMessageMode;

   void m_SetupContextMenu(void);
   void m_OnCustomContextMenuRequested(const QPoint & orc_Pos);
   void m_CopySelection(void);
   void m_HandleMessages(void);
   void m_UpdateUi(const std::list<stw_opensyde_gui_logic::C_CamMetTreeLoggerData> & orc_Data);
   void m_SetChildColumns(const std::vector<stw_types::sint32> & orc_Indices);
   void m_SetAllChildren(void);
   void m_RepositionButtons(void);
   void m_DoScrollTop(void);
   void m_DoScrollBottom(void);
   void m_RestoreUserSettings(void);
   void m_ExpandAll(void);
   void m_CollapseAll(void);
   void m_OnCollapse(const QModelIndex & orc_Index);
   std::vector<stw_types::sint32> m_GetCurrentColumnPositionIndices(void) const;
   void m_SetColumnPositionIndices(const std::vector<stw_types::sint32> & orc_NewColPositionIndices);
   bool m_ColumnsSortedAsExpected(const std::vector<stw_types::sint32> & orc_NewColPositionIndices) const;
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif