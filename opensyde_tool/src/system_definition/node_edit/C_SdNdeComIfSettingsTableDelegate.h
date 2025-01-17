//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       COM IF settings table delegate (header)

   See cpp file for detailed description

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SDNDECOMIFSETTINGSTABLEDELEGATE_H
#define C_SDNDECOMIFSETTINGSTABLEDELEGATE_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include <QStyledItemDelegate>

#include "stwtypes.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SdNdeComIfSettingsTableDelegate :
   public QStyledItemDelegate
{
   Q_OBJECT

public:
   enum E_Columns
   {
      //eERROR = 0,
      eINTERFACE = 0,
      eCONNECTION,
      eNODEID,
      eIPADDRESS,
      eUPDATE,
      eDIAGNOSTIC,
      eROUTING,
      //eCOMLAYER2,
      //eCOMECES,
      //eCOMECOS,
      eSIZE
   };

   C_SdNdeComIfSettingsTableDelegate(QObject * const opc_Parent = NULL, const stw_types::uint32 ou32_NodeIndex = 0);

   virtual QWidget * createEditor(QWidget * const opc_Parent, const QStyleOptionViewItem & orc_Option,
                                  const QModelIndex & orc_Index) const;
   virtual void setEditorData(QWidget * const opc_Editor, const QModelIndex & orc_Index) const;
   virtual void setModelData(QWidget * const opc_Editor, QAbstractItemModel * const opc_Model,
                             const QModelIndex & orc_Index) const;

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736

Q_SIGNALS:
   //lint -restore
   void SigEdit(const QModelIndex & orc_Index) const;

private:
   stw_types::uint32 mu32_NodeIndex;
   QModelIndex mc_Edit;

   void m_CheckValue(const stw_types::sintn & orsn_Value) const;
   void m_StoreEdit(const QModelIndex & orc_Index);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
