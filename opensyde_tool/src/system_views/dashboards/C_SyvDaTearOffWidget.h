//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for showing a concrete instance of C_SyvDaDashboardWidget in a seperated window

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SYVDATEAROFFWIDGET_H
#define C_SYVDATEAROFFWIDGET_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QWidget>

#include "stwtypes.h"

#include "C_SyvDaDashboardWidget.h"
#include "C_OgeWiDashboardTab.h"
#include "C_SyvComDriverDiag.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */

namespace Ui
{
class C_SyvDaTearOffWidget;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SyvDaTearOffWidget :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SyvDaTearOffWidget(const stw_types::uint32 ou32_ViewIndex, const stw_types::uint32 ou32_DataIndex,
                                 const QString & orc_Name, /* const QString & orc_Comment,*/
                                 C_SyvDaDashboardWidget * const opc_Widget, QWidget * const opc_Parent = NULL);
   ~C_SyvDaTearOffWidget();

   void InitText() const;

   stw_types::uint32 GetIndex(void) const;
   void SetDataIndex(const stw_types::uint32 ou32_Value);
   QString GetName(void) const;

   C_SyvDaDashboardWidget * GetWidget(void);

   void SetEditMode(const bool oq_Active);
   void SetDarkMode(const bool oq_Active);
   void SetEnabled(const bool oq_Enabled) const;
   void Save(void) const;
   void HandleManualOperationFinished(const stw_types::sint32 os32_Result, const stw_types::uint8 ou8_NRC) const;
   void RegisterWidgets(stw_opensyde_gui_logic::C_SyvComDriverDiag & orc_ComDriver) const;
   void ConnectionActiveChanged(const bool oq_Active, const bool oq_WidgetTabOnly = false) const;
   void UpdateShowValues(void) const;
   void UpdateTransmissionConfiguration(void) const;

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736

Q_SIGNALS:
   //lint -restore
   void SigConfirmClicked(void);
   void SigCancelClicked(void);
   void SigChanged(void);
   void SigErrorChange(void);
   void SigTriggerUpdateTransmissionConfiguration(void);
   void SigDataPoolWrite(const stw_types::uint32 ou32_NodeIndex, const stw_types::uint8 ou8_DataPoolIndex,
                         const stw_types::uint16 ou16_ListIndex, const stw_types::uint16 ou16_ElementIndex);
   void SigDataPoolRead(const stw_opensyde_core::C_OSCNodeDataPoolListElementId & orc_Index);
   void SigNvmReadList(const stw_opensyde_core::C_OSCNodeDataPoolListId & orc_Id);

   void SigWidgetComeBack(C_SyvDaTearOffWidget * const opc_Widget);
   void SigMySig(void);

protected:
   virtual void paintEvent(QPaintEvent * const opc_Event) override;
   virtual void closeEvent(QCloseEvent * const opc_Event) override;

private:
   void m_OnActiveChange(const stw_opensyde_gui_elements::C_OgeWiDashboardTab * const opc_Source, const bool oq_Active);
   void m_OnCopy(const stw_opensyde_gui_elements::C_OgeWiDashboardTab * const opc_Source) const;
   void m_OnEditProperties(stw_opensyde_gui_elements::C_OgeWiDashboardTab * const opc_Source);
   void m_CheckError(void) const;
   void m_AdaptSpaceHolderWidgetColor(void) const;

   //Avoid call
   C_SyvDaTearOffWidget(const C_SyvDaTearOffWidget &);
   C_SyvDaTearOffWidget & operator =(const C_SyvDaTearOffWidget &);

   Ui::C_SyvDaTearOffWidget * mpc_Ui;
   C_SyvDaDashboardWidget * mpc_Dashboard;
   bool mq_EditModeActive;
   bool mq_DarkModeActive;
};
}

#endif
