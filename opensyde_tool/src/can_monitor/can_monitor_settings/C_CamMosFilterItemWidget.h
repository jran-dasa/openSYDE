//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for Filter list item (header)

   See cpp file for detailed description

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_CAMMOSFILTERITEMWIDGET_H
#define C_CAMMOSFILTERITEMWIDGET_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "C_OgeWiOnlyBackground.h"
#include "C_CamProFilterData.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace Ui
{
class C_CamMosFilterItemWidget;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_CamMosFilterItemWidget :
   public stw_opensyde_gui_elements::C_OgeWiOnlyBackground
{
   Q_OBJECT

public:
   explicit C_CamMosFilterItemWidget(const stw_opensyde_gui_logic::C_CamProFilterData & orc_Filter,
                                     QWidget * const opc_Parent = NULL);
   ~C_CamMosFilterItemWidget(void);

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736
Q_SIGNALS:
   //lint -restore
   void SigRemoveFilter(C_CamMosFilterItemWidget * const opc_Sender);
   void SigEnableFilter(const C_CamMosFilterItemWidget * const opc_Sender, const bool & orq_Select);
   void SigUpdateFilter(const C_CamMosFilterItemWidget * const opc_Sender,
                        const stw_opensyde_gui_logic::C_CamProFilterData & orc_FilterOld,
                        const stw_opensyde_gui_logic::C_CamProFilterData & orc_FilterNew);

private:
   Ui::C_CamMosFilterItemWidget * mpc_Ui;
   stw_opensyde_gui_logic::C_CamProFilterData mc_Filter;
   bool mq_ButtonPressed;

   //Avoid call
   C_CamMosFilterItemWidget(const C_CamMosFilterItemWidget &);
   C_CamMosFilterItemWidget & operator =(const C_CamMosFilterItemWidget &);

   void m_CreateTooltipInformation(void);
   void m_OnEdit(void);
   void m_OnRemove(void);
   void m_OnChxToggle(const bool & orq_Checked);
   void m_ButtonPressed(void);
   void m_ButtonReleased(void);
   void m_SetFilterNameElided(void) const;

protected:
   virtual bool event(QEvent * const opc_Event) override;
   virtual void mouseDoubleClickEvent(QMouseEvent * const opc_Event) override;
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
