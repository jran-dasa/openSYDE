//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Custom combo box for param tree (implementation)

   Custom combo box for param tree.
   This class does not contain any functionality,
   but needs to exist, to have a unique group,
   to apply a specific stylesheet for.

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include "C_OgeWiUtil.h"
#include "C_OgeCbxParam.h"
#include "C_OgeCbxIconDelegate.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_gui_elements;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in,out] opc_Parent Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_OgeCbxParam::C_OgeCbxParam(QWidget * const opc_Parent) :
   QComboBox(opc_Parent)
{
   //this code allows to handle the QAbstractItemView::item in stylesheets
   C_OgeCbxIconDelegate * const pc_ItemDelegate = new C_OgeCbxIconDelegate();

   pc_ItemDelegate->SetPaddingLeft(0);

   this->setItemDelegate(pc_ItemDelegate);
} //lint !e429  //no memory leak because of the parent all elements and the Qt memory management

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default destructor
*/
//----------------------------------------------------------------------------------------------------------------------
C_OgeCbxParam::~C_OgeCbxParam(void)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten key press event slot

   Here: Explicitly ignore events which are globally handled (Won't be forwarded otherwise!)

   \param[in,out] opc_Event Event identification and information
*/
//----------------------------------------------------------------------------------------------------------------------
void C_OgeCbxParam::keyPressEvent(QKeyEvent * const opc_Event)
{
   if (C_OgeWiUtil::h_CheckGlobalKey(opc_Event) == true)
   {
      opc_Event->ignore();
   }
   else
   {
      QComboBox::keyPressEvent(opc_Event);
   }
}
