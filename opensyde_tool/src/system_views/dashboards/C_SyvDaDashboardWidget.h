//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for showing a concrete dashboard

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SYVDADASHBOARDWIDGET_H
#define C_SYVDADASHBOARDWIDGET_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include <QWidget>

#include "stwtypes.h"

#include "C_SyvDaDashboardContentBaseWidget.h"
#include "C_SyvComDriverDiag.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */

namespace Ui
{
class C_SyvDaDashboardWidget;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SyvDaDashboardWidget :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SyvDaDashboardWidget(const stw_types::uint32 ou32_ViewIndex, const stw_types::uint32 ou32_DashboardIndex,
                                   const QString & orc_Name, const bool oq_Window, QWidget * const opc_Parent = NULL);
   ~C_SyvDaDashboardWidget();

   stw_types::uint32 GetViewIndex(void) const;
   stw_types::uint32 GetDashboardIndex(void) const;
   void SetDashboardIndex(const stw_types::uint32 ou32_Value);
   QString GetName(void) const;

   void SetEditMode(const bool oq_Active);
   void SetDarkMode(const bool oq_Active);
   void SetDrawingActive(const bool oq_Active) const;
   void Save(void) const;

   void RegisterWidgets(stw_opensyde_gui_logic::C_SyvComDriverDiag & orc_ComDriver) const;
   void ConnectionActiveChanged(const bool oq_Active) const;
   void UpdateShowValues(void) const;
   void UpdateTransmissionConfiguration(void) const;
   void HandleManualOperationFinished(const stw_types::sint32 os32_Result, const stw_types::uint8 ou8_NRC) const;

   void SetFocus(void) const;

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736

Q_SIGNALS:
   //lint -restore
   void SigErrorChange(void);
   void SigTriggerUpdateTransmissionConfiguration(void);
   void SigDataPoolWrite(const stw_types::uint32 ou32_NodeIndex, const stw_types::uint8 ou8_DataPoolIndex,
                         const stw_types::uint16 ou16_ListIndex, const stw_types::uint16 ou16_ElementIndex);
   void SigDataPoolRead(const stw_opensyde_core::C_OSCNodeDataPoolListElementId & orc_Index);
   void SigNvmReadList(const stw_opensyde_core::C_OSCNodeDataPoolListId & orc_Id);

private:
   //Avoid call
   C_SyvDaDashboardWidget(const C_SyvDaDashboardWidget &);
   C_SyvDaDashboardWidget & operator =(const C_SyvDaDashboardWidget &);

   void m_InitSceneWidget(void);
   void m_InitChartWidget(void);

   Ui::C_SyvDaDashboardWidget * mpc_Ui;
   C_SyvDaDashboardContentBaseWidget * mpc_Content; // Pointer to scene or chart
   const stw_types::uint32 mu32_ViewIndex;
   stw_types::uint32 mu32_DashboardIndex;
   const QString mc_Name;
};
}

#endif
