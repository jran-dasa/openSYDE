//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget to display one data pool list content (implementation)

   Widget to display one data pool list content

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include "stwerrors.h"
#include "C_SdNdeDpListTableWidget.h"
#include "C_SdNdeDpListTableWidget.h"
#include "ui_C_SdNdeDpListTableWidget.h"
#include "C_GtGetText.h"
#include "C_PuiSdHandler.h"
#include "C_SdNdeDpListsTreeWidget.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_opensyde_gui;
using namespace stw_opensyde_gui_logic;
using namespace stw_opensyde_core;
using namespace stw_errors;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor

   Set up GUI with all elements.

   \param[in,out] opc_Parent         Optional pointer to parent
   \param[in,out] opc_TreeWidget     Tree widget
   \param[in,out] opc_UndoStack      Undo stack
   \param[in]     oq_PopUp           optional flag for "in pop up" state
*/
//----------------------------------------------------------------------------------------------------------------------
C_SdNdeDpListTableWidget::C_SdNdeDpListTableWidget(QWidget * const opc_Parent, QTreeWidget * const opc_TreeWidget,
                                                   C_SdNdeUnoDataPoolManager * const opc_UndoStack,
                                                   const bool oq_PopUp) :
   QWidget(opc_Parent),
   mpc_Ui(new Ui::C_SdNdeDpListTableWidget),
   mpc_TreeWidget(opc_TreeWidget),
   mpc_UndoStack(opc_UndoStack),
   mu32_NodeIndex(0),
   mu32_DataPoolIndex(0),
   mu32_ListIndex(0),
   mq_PopUp(oq_PopUp)

{
   mpc_Ui->setupUi(this);

   InitStaticNames();

   m_SetCRCVisibility(false);
   this->mpc_Ui->pc_TableView->SetUndoStack(opc_UndoStack);

   //Connects
   connect(this->mpc_Ui->pc_TableView, &C_SdNdeDpListTableView::SigErrorChangePossible, this,
           &C_SdNdeDpListTableWidget::m_OnErrorChangePossible);
   connect(this->mpc_Ui->pc_TableView, &C_SdNdeDpListTableView::SigSizeChangePossible, this,
           &C_SdNdeDpListTableWidget::m_OnSizeChangePossible);
   connect(this->mpc_Ui->pc_TableView, &C_SdNdeDpListTableView::SigSelectionChanged, this,
           &C_SdNdeDpListTableWidget::m_OnSelectionChanged);
   connect(this->mpc_Ui->pc_TableView, &C_SdNdeDpListTableView::SigButtonChange, this,
           &C_SdNdeDpListTableWidget::m_OnButtonChange);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default destructor

   Clean up.
*/
//----------------------------------------------------------------------------------------------------------------------
//lint -e{1540}  no memory leak because never took ownership
C_SdNdeDpListTableWidget::~C_SdNdeDpListTableWidget(void)

{
   delete mpc_Ui;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Initialize all displayed static names
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::InitStaticNames(void) const
{
   this->mpc_Ui->pc_CheckBoxCRC->SetToolTipInformation(C_GtGetText::h_GetText(
                                                          "Add checksum"), C_GtGetText::h_GetText(
                                                          "CRC16 Checksum Option. If active, 2 bytes are reserved at the beginning of the list."));
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set tree widget & undo manager

   This function provides an alternative to the constructor parameters.

   \param[in,out] opc_TreeWidget Tree widget
   \param[in,out] opc_UndoStack  Undo stack
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::SetTreeWidgetAndUndoManager(QTreeWidget * const opc_TreeWidget,
                                                           C_SdNdeUnoDataPoolManager * const opc_UndoStack)
{
   this->mpc_TreeWidget = opc_TreeWidget;
   this->mpc_UndoStack = opc_UndoStack;
   this->mpc_Ui->pc_TableView->SetUndoStack(opc_UndoStack);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Specify associated list

   \param[in] oru32_NodeIndex     Node index
   \param[in] oru32_DataPoolIndex Node data pool index
   \param[in] oru32_ListIndex     Node data pool list index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::SetList(const stw_types::uint32 & oru32_NodeIndex,
                                       const stw_types::uint32 & oru32_DataPoolIndex,
                                       const stw_types::uint32 & oru32_ListIndex)
{
   //Internal
   this->mu32_NodeIndex = oru32_NodeIndex;
   this->mu32_DataPoolIndex = oru32_DataPoolIndex;
   this->mu32_ListIndex = oru32_ListIndex;
   //Init CRC option
   m_InitCRCOption();
   //Init table
   this->mpc_Ui->pc_TableView->SetList(oru32_NodeIndex, oru32_DataPoolIndex,
                                       oru32_ListIndex);
   m_AdjustToItems(true);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Clear table view selection
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::ClearSelection(void) const
{
   this->mpc_Ui->pc_TableView->clearSelection();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Forward action
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::Copy(void) const
{
   this->mpc_Ui->pc_TableView->Copy();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Forward action
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::Cut(void) const
{
   this->mpc_Ui->pc_TableView->Cut();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Forward action
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::Paste(void) const
{
   this->mpc_Ui->pc_TableView->Paste();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Forward action
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::Delete(void) const
{
   this->mpc_Ui->pc_TableView->Delete();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Insert triggered via action
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::InsertAction(void) const
{
   Insert(true);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Forward action

   \param[in] orq_SetFocus Optional flag if inital focus should be set
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::Insert(const bool & orq_SetFocus) const
{
   this->mpc_Ui->pc_TableView->Insert(orq_SetFocus);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Forward action
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::DoMoveUp(void) const
{
   this->mpc_Ui->pc_TableView->DoMoveUp();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Forward action
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::DoMoveDown(void) const
{
   this->mpc_Ui->pc_TableView->DoMoveDown();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Enter name edit
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::Edit(void) const
{
   this->mpc_Ui->pc_TableView->EditInitial();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set model view manager

   \param[in,out] opc_Value Model view manager
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::SetModelViewManager(C_SdNdeDpListModelViewManager * const opc_Value) const
{
   this->mpc_Ui->pc_TableView->SetModelViewManager(opc_Value);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Function to select a concrete dataelement

   \param[in] os32_DataElementIndex   Data element index
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::SelectDataElement(const sint32 os32_DataElementIndex) const
{
   this->mpc_Ui->pc_TableView->SelectDataElement(os32_DataElementIndex);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Check if table is selected

   \return
   True  Selected
   False Not selected
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_SdNdeDpListTableWidget::IsSelected(void) const
{
   return this->mpc_Ui->pc_TableView->IsSelected();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Register model data reset
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::Reset(void)
{
   this->mpc_Ui->pc_TableView->Reset();
   this->m_AdjustToItems();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set selected variable names

   \param[in] orc_VariableNames New selected variable names
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::SetSelectedVariableNames(const std::vector<QString> & orc_VariableNames) const
{
   this->mpc_Ui->pc_TableView->SetSelectedVariableNames(orc_VariableNames);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Get selected variable names

   \return
   Current selected variable names
*/
//----------------------------------------------------------------------------------------------------------------------
std::vector<QString> C_SdNdeDpListTableWidget::GetSelectedVariableNames(void) const
{
   return this->mpc_Ui->pc_TableView->GetSelectedVariableNames();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Forward signal
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::m_OnErrorChangePossible(void)
{
   Q_EMIT this->SigErrorChangePossible();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Forward signal
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::m_OnSizeChangePossible(void)
{
   Q_EMIT this->SigSizeChangePossible();

   m_AdjustToItems();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Forward signal
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::m_OnSelectionChanged(const uint32 & oru32_Count)
{
   Q_EMIT this->SigSelectionChanged(this->mu32_ListIndex, oru32_Count);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Forward signal

   \param[in] orq_AddActive      Push button add active
   \param[in] orq_PasteActive    Push button paste active
   \param[in] orq_MoveDownActive Push button move down active
   \param[in] orq_MoveUpActive   Push button move up active
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::m_OnButtonChange(const bool & orq_AddActive, const bool & orq_CutActive,
                                                const bool & orq_CopyActive, const bool & orq_PasteActive,
                                                const bool & orq_DeleteActive, const bool & orq_MoveDownActive,
                                                const bool & orq_MoveUpActive)
{
   Q_EMIT this->SigButtonChange(orq_AddActive, orq_CutActive, orq_CopyActive, orq_PasteActive, orq_DeleteActive,
                                orq_MoveDownActive, orq_MoveUpActive);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Handle CRC option checkbox value change

   \param[in] orq_Value New checkbox value
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::m_OnCRCOptionChange(const bool & orq_Value)
{
   if (this->mpc_UndoStack != NULL)
   {
      this->mpc_UndoStack->DoChangeListData(this->mu32_NodeIndex,
                                            this->mu32_DataPoolIndex,
                                            dynamic_cast<C_SdNdeDpListsTreeWidget * const>(this->
                                                                                           mpc_TreeWidget),
                                            this->mu32_ListIndex, static_cast<QVariant>(orq_Value),
                                            C_SdNdeDpUtil::eLIST_CRC);
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Init CRC Option check box
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::m_InitCRCOption(void)
{
   const C_OSCNodeDataPool * const pc_DataPool = C_PuiSdHandler::h_GetInstance()->GetOSCDataPool(this->mu32_NodeIndex,
                                                                                                 this->mu32_DataPoolIndex);

   m_SetCRCVisibility(false);
   if (pc_DataPool != NULL)
   {
      if (pc_DataPool->e_Type == C_OSCNodeDataPool::eNVM)
      {
         const C_OSCNodeDataPoolList * const pc_List = C_PuiSdHandler::h_GetInstance()->GetOSCDataPoolList(
            this->mu32_NodeIndex,
            this->mu32_DataPoolIndex,
            this->mu32_ListIndex);

         if (pc_List != NULL)
         {
            disconnect(this->mpc_Ui->pc_CheckBoxCRC, &QCheckBox::toggled, this,
                       &C_SdNdeDpListTableWidget::m_OnCRCOptionChange);
            m_SetCRCVisibility(true);
            this->mpc_Ui->pc_CheckBoxCRC->setChecked(pc_List->q_NvMCRCActive);
            if (pc_DataPool->q_IsSafety == true)
            {
               this->mpc_Ui->pc_CheckBoxCRC->setText(C_GtGetText::h_GetText(
                                                        "Add Checksum (Cannot be disabled for safety Datapools)"));
               this->mpc_Ui->pc_CheckBoxCRC->setEnabled(false);
            }
            else
            {
               this->mpc_Ui->pc_CheckBoxCRC->setText(C_GtGetText::h_GetText("Add Checksum"));
               this->mpc_Ui->pc_CheckBoxCRC->setEnabled(true);
            }
            connect(this->mpc_Ui->pc_CheckBoxCRC, &QCheckBox::toggled, this,
                    &C_SdNdeDpListTableWidget::m_OnCRCOptionChange);
         }
      }
   }
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Handle CRC option visibility

   \param[in] orq_Visible Flag for visibility
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::m_SetCRCVisibility(const bool & orq_Visible) const
{
   this->mpc_Ui->pc_WidgetCRC->setVisible(orq_Visible);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Adjust widget size to items

   \param[in] orq_Initial Flag if initial call
*/
//----------------------------------------------------------------------------------------------------------------------
void C_SdNdeDpListTableWidget::m_AdjustToItems(const bool & orq_Initial)
{
   if ((this->mpc_TreeWidget != NULL) && (this->mq_PopUp == false))
   {
      //Configure
      const sint32 s32_Height = C_SdNdeDpUtil::h_GetTableSize(this->mu32_NodeIndex, this->mu32_DataPoolIndex,
                                                              this->mu32_ListIndex,
                                                              this->mpc_TreeWidget->height());
      //Change size (necessary to reach minimum!)
      this->setMinimumHeight(s32_Height);
      this->setMaximumHeight(s32_Height);

      if (orq_Initial == false)
      {
         this->mpc_TreeWidget->adjustSize();
      }
   }
}
