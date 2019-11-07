//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for new data block properties (header)

   See cpp file for detailed description

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SDNDEDBPROPERTIES_H
#define C_SDNDEDBPROPERTIES_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QWidget>
#include "stwtypes.h"
#include "C_OgePopUpDialog.h"
#include "C_OSCNodeApplication.h"
#include "C_SdNdeDbDataPoolEntry.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace Ui
{
class C_SdNdeDbProperties;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SdNdeDbProperties :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SdNdeDbProperties(const stw_types::uint32 ou32_NodeIndex, const stw_types::sint32 os32_ApplicationIndex,
                                stw_opensyde_gui_elements::C_OgePopUpDialog & orc_Parent);
   ~C_SdNdeDbProperties(void);

   static QSize h_GetBinaryWindowSize(void);
   static QSize h_GetDefaultWindowSize(void);
   void InitStaticNames(void) const;
   void LoadFromData(const stw_opensyde_core::C_OSCNodeApplication & orc_Application);
   void ApplyNewData(stw_opensyde_core::C_OSCNodeApplication & orc_Application) const;
   void HandleDataPools(const stw_types::uint32 ou32_ApplicationIndex) const;

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void keyPressEvent(QKeyEvent * const opc_KeyEvent) override;
   //lint -restore

private:
   Ui::C_SdNdeDbProperties * mpc_Ui;
   const stw_types::uint32 mu32_NodeIndex;
   const stw_types::sint32 ms32_ApplicationIndex;
   stw_opensyde_core::C_OSCNodeApplication::E_Type me_Type;
   std::vector<C_SdNdeDbDataPoolEntry *> mc_DataPoolWidgets;
   std::vector<stw_types::uint32> mc_SelectedDataPools;
   //lint -e{1725} Only problematic if copy or assignment is allowed
   stw_opensyde_gui_elements::C_OgePopUpDialog & mrc_ParentDialog;
   static const stw_types::sintn mhsn_VERSION_INDEX_V1;
   static const stw_types::sintn mhsn_VERSION_INDEX_V2;
   static const stw_types::sintn mhsn_VERSION_INDEX_V3;

   void m_OkClicked(void);
   void m_CancelClicked(void);
   QString m_CheckName(void) const;
   QString m_CheckID(void) const;
   QString m_CheckPaths(void) const;
   QString m_CheckPath(const QString & orc_Path) const;
   QString m_GetDialogPath(const QString & orc_CurrentPath) const;

   void m_LoadData(const stw_types::uint32 ou32_NodeIndex, const stw_types::uint32 ou32_ApplicationIndex);
   void m_LoadFromData(const stw_opensyde_core::C_OSCNodeApplication & orc_Application);
   void m_SetVisibilityOfProgrammableLines(const bool & orq_Visible) const;

   void m_OnClickProject(void);
   void m_OnClickOutput(void);
   void m_OnClickGenerator(void);
   void m_OnClickGenerate(void);
   void m_OnClickClearProject(void) const;

   void m_OnNameEdited(void) const;
   void m_OnProcessIDChanged(void) const;

   void m_CleanUpDataPoolWidgets(void);
   void m_InitDataPoolsSection(void);
   void m_HandleAddDataPools(void);
   void m_HandleRevertCodeGenerator(void) const;
   void m_HandleDeleteDataPool(C_SdNdeDbDataPoolEntry * const opc_Source, const stw_types::uint32 ou32_Index);
   void m_UpdateOwnedDpsCount(void) const;

   void m_UpdatePathsRelativeToProject(void) const;

   //Avoid call
   C_SdNdeDbProperties(const C_SdNdeDbProperties &);
   C_SdNdeDbProperties & operator =(const C_SdNdeDbProperties &);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
