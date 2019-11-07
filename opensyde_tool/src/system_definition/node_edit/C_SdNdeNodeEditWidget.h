//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for node edit

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

#ifndef C_SDNDENODEEDITWIDGET_H
#define C_SDNDENODEEDITWIDGET_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QWidget>

#include "stwtypes.h"

#include "C_SdBueComIfDescriptionWidget.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */

namespace Ui
{
class C_SdNdeNodeEditWidget;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SdNdeNodeEditWidget :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SdNdeNodeEditWidget(const stw_types::uint32 ou32_NodeIndex, const stw_types::sintn osn_TabIndex,
                                  QWidget * const opc_Parent = NULL);
   virtual ~C_SdNdeNodeEditWidget();

   void InitStaticNames(void) const;
   bool WasChanged(void) const;
   void Save(void) const;
   void SetFlag(const stw_types::uint32 ou32_Flag) const;
   void OpenDetail(const stw_types::sint32 os32_MainIndex, const stw_types::sint32 os32_ListIndex,
                   const stw_types::sint32 os32_ElementIndex, const stw_types::sint32 os32_Flag);
   stw_types::sintn GetTabIndex(void) const;

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736

Q_SIGNALS:
   //lint -restore
   void SigChanged(void);
   void SigErrorChange(void);
   void SigSwitchToDataPool(const stw_types::uint32 & oru32_BusIndex, const QString & orc_BusName);
   void SigSwitchToBus(const stw_types::uint32 & oru32_BusIndex, const QString & orc_BusName);
   void SigNameChanged(const QString & orc_Name, const QString & orc_SubItemName,
                       const bool oq_CombineItemAndSubSubName);
   void SigSave(void);
   void SigSaveAs(void);

protected:
   // The naming of the Qt parameters can't be changed and are not compliant with the naming conventions
   //lint -save -e1960
   virtual void showEvent(QShowEvent * const opc_Event) override;
   virtual void hideEvent(QHideEvent * const opc_Event) override;
   //lint -restore

private:
   //Avoid call
   C_SdNdeNodeEditWidget(const C_SdNdeNodeEditWidget &);
   C_SdNdeNodeEditWidget & operator =(const C_SdNdeNodeEditWidget &);

   void m_SaveUserSettings(void) const;
   void m_DataChanged(void);
   void m_OnSwitchToBus(const stw_types::uint32 & oru32_BusIndex, const QString & orc_BusName);
   void m_EditComDataPool(const stw_types::uint32 ou32_DataPoolIndex, const stw_types::uint32 ou32_ListIndex);
   void m_CurrentTabChanged(const stw_types::sintn osn_Index) const;
   void m_ReloadDataPools(void) const;
   void m_OpenDataPool(const stw_types::uint32 ou32_DataPoolIndex);

   Ui::C_SdNdeNodeEditWidget * mpc_Ui;
   QWidget * mpc_TabThreeWidget;
   C_SdBueComIfDescriptionWidget * mpc_ComIfDescriptionWidget;
   const stw_types::uint32 mu32_NodeIndex;
   bool mq_DataChanged;
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
