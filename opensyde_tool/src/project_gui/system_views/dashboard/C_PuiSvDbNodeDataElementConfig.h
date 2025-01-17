//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       short description (header)

   See cpp file for detailed description

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_PUISVDBNODEDATAELEMENTCONFIG_H
#define C_PUISVDBNODEDATAELEMENTCONFIG_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "C_PuiSvDbNodeDataPoolListElementId.h"
#include "C_PuiSvDbDataElementScaling.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_PuiSvDbNodeDataElementConfig
{
public:
   C_PuiSvDbNodeDataElementConfig(void);
   virtual ~C_PuiSvDbNodeDataElementConfig(void);

   virtual void CalcHash(stw_types::uint32 & oru32_HashValue) const;

   C_PuiSvDbNodeDataPoolListElementId c_ElementId;
   C_PuiSvDbDataElementScaling c_ElementScaling;
   QString c_DisplayName;
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
