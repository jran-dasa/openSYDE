//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Slider with tool tip (implementation)

   Slider with tool tip

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include <QEvent>
#include <QHelpEvent>
#include <QStyle>
#include "C_OgeSliToolTipBase.h"
#include "C_SdNdeDpContentUtil.h"
#include "C_OSCNodeDataPoolContentUtil.h"
#include "C_GtGetText.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_types;
using namespace stw_opensyde_core;
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

   \param[in,out]  opc_Parent    Optional pointer to parent
*/
//----------------------------------------------------------------------------------------------------------------------
C_OgeSliToolTipBase::C_OgeSliToolTipBase(QWidget * const opc_Parent) :
   QSlider(opc_Parent),
   mf64_ToolTipRangeOffset(0.0),
   mf64_ToolTipOffset(0.0),
   mf64_ToolTipFactor(1.0),
   me_RepresentationType(C_OSCNodeDataPoolContent::eSINT32),
   ms32_ToolTipCoordY(0)
{
   connect(this, &C_OgeSliToolTipBase::valueChanged, this, &C_OgeSliToolTipBase::m_OnValueChange);
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set value

   \param[in]  osn_Value   New value
*/
//----------------------------------------------------------------------------------------------------------------------
void C_OgeSliToolTipBase::SetValueCustom(const sintn osn_Value)
{
   this->setValue(osn_Value);
   m_OnValueChange();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Set parameter for tooltip display format

   Formula: ((Value - of64_ToolTipRangeOffset) * of64_ToolTipFactor) + of64_ToolTipOffset

   \param[in]  of64_ToolTipRangeOffset    See formula
   \param[in]  of64_ToolTipOffset         See formula
   \param[in]  of64_ToolTipFactor         See formula
   \param[in]  oe_RepresentationType      Representation type
*/
//----------------------------------------------------------------------------------------------------------------------
void C_OgeSliToolTipBase::SetToolTipParameters(const stw_types::float64 of64_ToolTipRangeOffset,
                                               const stw_types::float64 of64_ToolTipOffset,
                                               const stw_types::float64 of64_ToolTipFactor,
                                               const C_OSCNodeDataPoolContent::E_Type oe_RepresentationType)
{
   this->mf64_ToolTipRangeOffset = of64_ToolTipRangeOffset;
   this->mf64_ToolTipOffset = of64_ToolTipOffset;
   this->mf64_ToolTipFactor = of64_ToolTipFactor;
   this->me_RepresentationType = oe_RepresentationType;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Overwritten default event slot

   Here: Handle tool tip

   \param[in,out]  opc_Event  Event identification and information

   \return
   True  Event was recognized and processed
   False Event ignored
*/
//----------------------------------------------------------------------------------------------------------------------
bool C_OgeSliToolTipBase::event(QEvent * const opc_Event)
{
   bool q_Return;

   if (this->mq_ToolTipActive == true)
   {
      if (opc_Event->type() == QEvent::ToolTip)
      {
         //show tooltip
         if (this->m_GetToolTip()->isVisible() == false)
         {
            QHelpEvent * const pc_HelpEvent = dynamic_cast<QHelpEvent * const>(opc_Event);

            if (pc_HelpEvent != NULL)
            {
               this->setMouseTracking(true);

               this->m_GetToolTip()->SetHeading(this->mc_ToolTipHeading);
               this->m_GetToolTip()->SetContent(this->mc_ToolTipContent);
               this->m_GetToolTip()->SetType(this->me_ToolTipType);
               this->m_GetToolTip()->show();
               this->ms32_ToolTipCoordY = pc_HelpEvent->pos().y();
               this->m_MoveToolTip();
            }
         }
         //Accept event because of Qt dynamic tooltip time based on the fact if there was a tooltip in this widget
         // already
         opc_Event->accept();
         q_Return = true;
      }
      else if (opc_Event->type() == QEvent::Leave)
      {
         //hide on leave
         this->m_HideToolTip();

         this->setMouseTracking(false);

         opc_Event->accept();
         q_Return = true;
      }
      else
      {
         q_Return = QSlider::event(opc_Event);
      }
   }
   else
   {
      q_Return = QSlider::event(opc_Event);
   }

   return q_Return;
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Handle slider value change
*/
//----------------------------------------------------------------------------------------------------------------------
void C_OgeSliToolTipBase::m_OnValueChange(void)
{
   QString c_Content;
   C_OSCNodeDataPoolContent c_Tmp;

   c_Tmp.SetArray(false);
   if (this->me_RepresentationType < stw_opensyde_core::C_OSCNodeDataPoolContent::E_Type::eFLOAT32)
   {
      //For all integer types:
      //Use UINT64 data type for slider values to prevent getting in max datatype range
      //Example of an SINT8 value
      //   slider value: 255 (=128)
      //   Trying to convert 255 to sint8 the engine cuts the value to 128 (=sint8 max).
      //   Resulting into Tooltip value = -1 for slider values >= 127
      c_Tmp.SetType(stw_opensyde_core::C_OSCNodeDataPoolContent::E_Type::eUINT64);
   }
   else
   {
      c_Tmp.SetType(this->me_RepresentationType);
   }

   //set slider value in content (c_Tmp)
   C_OSCNodeDataPoolContentUtil::h_SetValueInContent(
      static_cast<float64>(this->value()) - this->mf64_ToolTipRangeOffset, c_Tmp);

   //apply factor and offset
   C_SdNdeDpContentUtil::h_GetValueAsScaledString(c_Tmp, this->mf64_ToolTipFactor, this->mf64_ToolTipOffset,
                                                  c_Content, 0UL);

   this->SetToolTipInformation(C_GtGetText::h_GetText("Value"), c_Content);
   //Update directly
   if (this->m_GetToolTip() != NULL)
   {
      this->m_GetToolTip()->SetContent(c_Content);
   }

   // Update tooltip position
   this->m_MoveToolTip();
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Move tooltip to slider-handle position
*/
//----------------------------------------------------------------------------------------------------------------------
void C_OgeSliToolTipBase::m_MoveToolTip(void)
{
   if (this->m_GetToolTip() != NULL)
   {
      QPoint c_ToolTipPos;
      c_ToolTipPos.setX(QStyle::sliderPositionFromValue(this->minimum(), this->maximum(), this->value(),
                                                        this->width() - 12)); // 2x border width + 2x margin
      c_ToolTipPos.setY(ms32_ToolTipCoordY);
      this->m_GetToolTip()->DoMove(this->mapToGlobal(c_ToolTipPos));
   }
}
