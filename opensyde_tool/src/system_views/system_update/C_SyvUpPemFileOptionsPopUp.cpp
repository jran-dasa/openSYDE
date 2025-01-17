//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Screen for pem file options

   Screen for pem file options

   \copyright   Copyright 2021 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include "stwtypes.h"
#include "TGLUtils.h"
#include "C_GtGetText.h"
#include "C_SyvUpPemFileOptionsPopUp.h"
#include "ui_C_SyvUpPemFileOptionsPopUp.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_tgl;
using namespace stw_types;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_gui_elements;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */
const stw_types::sintn C_SyvUpPemFileOptionsPopUp::mhsn_SEC_INDEX_NO_CHANGE = 0;
const stw_types::sintn C_SyvUpPemFileOptionsPopUp::mhsn_SEC_INDEX_ACTIVATE = 1;
const stw_types::sintn C_SyvUpPemFileOptionsPopUp::mhsn_SEC_INDEX_DEACTIVATE = 2;
const stw_types::sintn C_SyvUpPemFileOptionsPopUp::mhsn_DEB_INDEX_NO_CHANGE = 0;
const stw_types::sintn C_SyvUpPemFileOptionsPopUp::mhsn_DEB_INDEX_ACTIVATE = 1;
const stw_types::sintn C_SyvUpPemFileOptionsPopUp::mhsn_DEB_INDEX_DEACTIVATE = 2;

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in,out]  orc_Parent          Reference to parent
   \param[in]      oe_StateSecurity    State security
   \param[in]      oe_StateDebugger    State debugger
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpPemFileOptionsPopUp::C_SyvUpPemFileOptionsPopUp(stw_opensyde_gui_elements::C_OgePopUpDialog & orc_Parent,
                                                       const C_PuiSvNodeUpdate::E_StateSecurity oe_StateSecurity,
                                                       const C_PuiSvNodeUpdate::E_StateDebugger oe_StateDebugger) :
   QWidget(&orc_Parent),
   mpc_Ui(new Ui::C_SyvUpPemFileOptionsPopUp),
   mrc_ParentDialog(orc_Parent)
{
   this->mpc_Ui->setupUi(this);

   InitStaticNames();

   this->m_InitComboBoxSec(oe_StateSecurity);
   this->m_InitComboBoxDeb(oe_StateDebugger);

   // register the widget for showing
   this->mrc_ParentDialog.SetWidget(this);

   connect(this->mpc_Ui->pc_PushButtonOk, &QPushButton::clicked, this, &C_SyvUpPemFileOptionsPopUp::m_OkClicked);
   connect(this->mpc_Ui->pc_PushButtonCancel, &QPushButton::clicked, this,
           &C_SyvUpPemFileOptionsPopUp::m_CancelClicked);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default destructor
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpPemFileOptionsPopUp::~C_SyvUpPemFileOptionsPopUp(void)
{
   delete this->mpc_Ui;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize all displayed static names
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPemFileOptionsPopUp::InitStaticNames(void) const
{
   const QString c_ToolTipContent = C_GtGetText::h_GetText("Explanation:\n"
                                                           "No Change: Do not send service\n"
                                                           "Activate: Change current state to %1 active\n"
                                                           "Deactivate: Change current state to %1 inactive");

   this->mrc_ParentDialog.SetTitle(C_GtGetText::h_GetText("PEM File"));
   this->mrc_ParentDialog.SetSubTitle(C_GtGetText::h_GetText("Settings"));
   this->mpc_Ui->pc_LabelHeading->setText(C_GtGetText::h_GetText("Services"));
   this->mpc_Ui->pc_LabelSecurity->setText(C_GtGetText::h_GetText("Activate Security"));
   this->mpc_Ui->pc_LabelDebugger->setText(C_GtGetText::h_GetText("Activate Debugger"));

   this->mpc_Ui->pc_PushButtonCancel->setText(C_GtGetText::h_GetText("Cancel"));
   this->mpc_Ui->pc_PushButtonOk->setText(C_GtGetText::h_GetText("OK"));

   //Combo Box
   this->mpc_Ui->pc_ComboBoxSecurity->addItem(C_GtGetText::h_GetText("No Change"));
   this->mpc_Ui->pc_ComboBoxSecurity->addItem(C_GtGetText::h_GetText("Activate"));
   this->mpc_Ui->pc_ComboBoxSecurity->addItem(C_GtGetText::h_GetText("Deactivate"));
   this->mpc_Ui->pc_ComboBoxDebugger->addItem(C_GtGetText::h_GetText("No Change"));
   this->mpc_Ui->pc_ComboBoxDebugger->addItem(C_GtGetText::h_GetText("Activate"));
   this->mpc_Ui->pc_ComboBoxDebugger->addItem(C_GtGetText::h_GetText("Deactivate"));

   //ToolTips
   this->mpc_Ui->pc_LabelSecurity->SetToolTipInformation(C_GtGetText::h_GetText(
                                                            "Activate Security"),
                                                         c_ToolTipContent.arg(C_GtGetText::h_GetText("Security")));
   this->mpc_Ui->pc_LabelDebugger->SetToolTipInformation(C_GtGetText::h_GetText(
                                                            "Activate Debugger"),
                                                         c_ToolTipContent.arg(C_GtGetText::h_GetText("Debugger")));
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get combo box sec state

   \return
   Combo box sec state
*/
//----------------------------------------------------------------------------------------------------------------------
C_PuiSvNodeUpdate::E_StateSecurity C_SyvUpPemFileOptionsPopUp::GetComboBoxSecState() const
{
   C_PuiSvNodeUpdate::E_StateSecurity e_Retval = C_PuiSvNodeUpdate::eST_SEC_NO_CHANGE;
   switch (this->mpc_Ui->pc_ComboBoxSecurity->currentIndex())
   {
   case C_SyvUpPemFileOptionsPopUp::mhsn_SEC_INDEX_NO_CHANGE:
      e_Retval = C_PuiSvNodeUpdate::eST_SEC_NO_CHANGE;
      break;
   case C_SyvUpPemFileOptionsPopUp::mhsn_SEC_INDEX_ACTIVATE:
      e_Retval = C_PuiSvNodeUpdate::eST_SEC_ACTIVATE;
      break;
   case C_SyvUpPemFileOptionsPopUp::mhsn_SEC_INDEX_DEACTIVATE:
      e_Retval = C_PuiSvNodeUpdate::eST_SEC_DEACTIVATE;
      break;
   default:
      tgl_assert(false);
      break;
   }
   return e_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Get combo box deb state

   \return
   Combo box deb state
*/
//----------------------------------------------------------------------------------------------------------------------
C_PuiSvNodeUpdate::E_StateDebugger C_SyvUpPemFileOptionsPopUp::GetComboBoxDebState() const
{
   C_PuiSvNodeUpdate::E_StateDebugger e_Retval = C_PuiSvNodeUpdate::eST_DEB_NO_CHANGE;
   switch (this->mpc_Ui->pc_ComboBoxDebugger->currentIndex())
   {
   case C_SyvUpPemFileOptionsPopUp::mhsn_DEB_INDEX_NO_CHANGE:
      e_Retval = C_PuiSvNodeUpdate::eST_DEB_NO_CHANGE;
      break;
   case C_SyvUpPemFileOptionsPopUp::mhsn_DEB_INDEX_ACTIVATE:
      e_Retval = C_PuiSvNodeUpdate::eST_DEB_ACTIVATE;
      break;
   case C_SyvUpPemFileOptionsPopUp::mhsn_DEB_INDEX_DEACTIVATE:
      e_Retval = C_PuiSvNodeUpdate::eST_DEB_DEACTIVATE;
      break;
   default:
      tgl_assert(false);
      break;
   }
   return e_Retval;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten key press event slot

   Here: Handle specific enter key cases

   \param[in,out]  opc_KeyEvent  Event identification and information
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPemFileOptionsPopUp::keyPressEvent(QKeyEvent * const opc_KeyEvent)
{
   bool q_CallOrg = true;

   //Handle all enter key cases manually
   if ((opc_KeyEvent->key() == static_cast<sintn>(Qt::Key_Enter)) ||
       (opc_KeyEvent->key() == static_cast<sintn>(Qt::Key_Return)))
   {
      if (((opc_KeyEvent->modifiers().testFlag(Qt::ControlModifier) == true) &&
           (opc_KeyEvent->modifiers().testFlag(Qt::AltModifier) == false)) &&
          (opc_KeyEvent->modifiers().testFlag(Qt::ShiftModifier) == false))
      {
         this->mrc_ParentDialog.accept();
      }
      else
      {
         q_CallOrg = false;
      }
   }
   if (q_CallOrg == true)
   {
      QWidget::keyPressEvent(opc_KeyEvent);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Slot of Ok button click
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPemFileOptionsPopUp::m_OkClicked(void)
{
   this->mrc_ParentDialog.accept();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Slot of Cancel button click
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPemFileOptionsPopUp::m_CancelClicked()
{
   this->mrc_ParentDialog.reject();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Init combo box sec

   \param[in]  oe_StateSecurity  State security
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPemFileOptionsPopUp::m_InitComboBoxSec(const C_PuiSvNodeUpdate::E_StateSecurity oe_StateSecurity)
{
   //lint -e{9042} Warning wanted if new cases are added
   switch (oe_StateSecurity)
   {
   case C_PuiSvNodeUpdate::eST_SEC_NO_CHANGE:
      this->mpc_Ui->pc_ComboBoxSecurity->setCurrentIndex(C_SyvUpPemFileOptionsPopUp::mhsn_SEC_INDEX_NO_CHANGE);
      break;
   case C_PuiSvNodeUpdate::eST_SEC_ACTIVATE:
      this->mpc_Ui->pc_ComboBoxSecurity->setCurrentIndex(C_SyvUpPemFileOptionsPopUp::mhsn_SEC_INDEX_ACTIVATE);
      break;
   case C_PuiSvNodeUpdate::eST_SEC_DEACTIVATE:
      this->mpc_Ui->pc_ComboBoxSecurity->setCurrentIndex(C_SyvUpPemFileOptionsPopUp::mhsn_SEC_INDEX_DEACTIVATE);
      break;
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief  Init combo box deb

   \param[in]  oe_StateDebugger  State debugger
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SyvUpPemFileOptionsPopUp::m_InitComboBoxDeb(const C_PuiSvNodeUpdate::E_StateDebugger oe_StateDebugger)
{
   //lint -e{9042} Warning wanted if new cases are added
   switch (oe_StateDebugger)
   {
   case C_PuiSvNodeUpdate::eST_DEB_NO_CHANGE:
      this->mpc_Ui->pc_ComboBoxDebugger->setCurrentIndex(C_SyvUpPemFileOptionsPopUp::mhsn_DEB_INDEX_NO_CHANGE);
      break;
   case C_PuiSvNodeUpdate::eST_DEB_ACTIVATE:
      this->mpc_Ui->pc_ComboBoxDebugger->setCurrentIndex(C_SyvUpPemFileOptionsPopUp::mhsn_DEB_INDEX_ACTIVATE);
      break;
   case C_PuiSvNodeUpdate::eST_DEB_DEACTIVATE:
      this->mpc_Ui->pc_ComboBoxDebugger->setCurrentIndex(C_SyvUpPemFileOptionsPopUp::mhsn_DEB_INDEX_DEACTIVATE);
      break;
   }
}
