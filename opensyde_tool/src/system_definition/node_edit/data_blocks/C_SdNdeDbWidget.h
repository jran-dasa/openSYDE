//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for showing an application in the application list (header)

   \copyright   Copyright 2016 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SDNDEDBWIDGET_H
#define C_SDNDEDBWIDGET_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QWidget>
#include "stwtypes.h"
#include "C_OgeLabGroupItem.h"
#include "C_OSCNodeApplication.h"

namespace Ui
{
class C_SdNdeDbWidget;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SdNdeDbWidget :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SdNdeDbWidget(const stw_types::uint32 ou32_NodeIndex, const stw_types::uint32 ou32_ApplicationIndex,
                            QWidget * const opc_Parent = NULL);
   virtual ~C_SdNdeDbWidget();

   void InitStaticNames(void) const;
   void UpdateApplication(void);
   void UpdateApplicationIndex(const stw_types::uint32 ou32_ApplicationIndex);
   stw_types::uint32 GetNodeIndex(void) const;
   stw_types::uint32 GetApplicationIndex(void) const;
   void CheckProcessIdError(void) const;

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736

Q_SIGNALS:
   //lint -restore
   void SigDelete(const stw_types::uint32 ou32_NodeIndex, const stw_types::uint32 ou32_ApplicationIndex);
   void SigCheckNodeId(void);
   void SigOwnedDataPoolsChanged(void) const;

protected:
   virtual void mouseDoubleClickEvent(QMouseEvent * const opc_Event) override;

private:
   //Avoid call
   C_SdNdeDbWidget(const C_SdNdeDbWidget &);
   C_SdNdeDbWidget & operator =(const C_SdNdeDbWidget &);

   void m_LoadData(void);
   void m_OnEdit(void);
   void m_OnDelete(void);
   void m_OnOpenIdeClicked(void);
   void m_OnCommentToggled(const bool oq_Checked);
   void m_UnassignAllAssociatedDataPools(void) const;
   stw_opensyde_core::C_OSCNodeApplication::E_Type m_GetType(void) const;
   stw_types::uint32 m_CountAllAssociatedDataPools(void) const;
   QString m_GetAllAssociatedDataPoolNames(void) const;
   QString m_GetAllOutputFiles(void) const;

   Ui::C_SdNdeDbWidget * mpc_Ui;
   stw_types::uint32 mu32_ApplicationIndex;
   const stw_types::uint32 mu32_NodeIndex;
};
}

#endif
