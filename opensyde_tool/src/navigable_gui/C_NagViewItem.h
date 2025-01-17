//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for view item in navigation bar (header)

   See cpp file for detailed description

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_NAGVIEWITEM_H
#define C_NAGVIEWITEM_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QIcon>
#include <QWidget>
#include "stwtypes.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace Ui
{
class C_NagViewItem;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_NagViewItem :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_NagViewItem(QWidget * const opc_Parent = NULL);
   ~C_NagViewItem(void);

   void LoadUserSettings(void);
   void SaveUserSettings(void) const;
   void UpdateName(void) const;
   void UpdateDeco(void) const;
   void InitStaticNames(void) const;
   void SetDragged(const bool oq_Active);
   void Init(const stw_types::uint32 ou32_ViewIndex);
   void SetActive(const bool oq_Active, const stw_types::sint32 os32_SubMode);
   void SetHovered(const bool oq_Hovered) const;
   void HandleServiceMode(void);

   virtual QSize sizeHint(void) const override;

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736

Q_SIGNALS:
   //lint -restore
   void SigStartDrag(const C_NagViewItem * const opc_Sender);
   void SigDuplicate(const C_NagViewItem * const opc_Sender);
   void SigDelete(const C_NagViewItem * const opc_Sender);
   void SigExpanded(const C_NagViewItem * const opc_Sender);
   void SigSetName(const C_NagViewItem * const opc_Sender, const QString & orc_Name);
   void SigSelect(const C_NagViewItem * const opc_Sender, const stw_types::sint32 os32_SubMode,
                  const QString & orc_Name, const QString & orc_SubSubMode);

protected:
   virtual bool event(QEvent * const opc_Event) override;
   virtual void paintEvent(QPaintEvent * const opc_Event) override;

private:
   Ui::C_NagViewItem * mpc_Ui;

   bool mq_Active;
   bool mq_IgnoreActiveOnExpand;
   stw_types::uint32 mu32_ViewIndex;
   static const stw_types::sintn mhsn_SizeTop;
   static const stw_types::sintn mhsn_SizeSub;
   static const stw_types::sintn mhsn_FixSizeBottom;
   bool mq_ButtonPressed;
   QMetaObject::Connection mc_Conn;
   const QIcon mc_IconBulletList;
   const QIcon mc_IconBulletListActive;

   //Avoid call
   C_NagViewItem(const C_NagViewItem &);
   C_NagViewItem & operator =(const C_NagViewItem &);

   bool m_IsExpanded(void) const;
   void m_OnNameEditFinished(void);
   void m_OnNameEditCancelled(void);
   void m_SetExpanded(const bool oq_Expanded);
   void m_HandleExpandIcon(const bool oq_Expanded);
   void m_HandleSubItemNames(void);
   void m_OnExpand(void);
   void m_OnDelete(void);
   void m_OnStartDrag(void);
   void m_OnDuplicate(void);
   void m_OnEditButton(void) const;
   void m_OnSetupClicked(void);
   void m_OnUpdateClicked(void);
   void m_OnDashboardClicked(void);
   void m_HandleServiceModeColors(void);

   void m_ButtonPressed(void);
   void m_ButtonReleased(void);
   static QIcon mh_PrepareIcon(const QString & orc_Path);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
