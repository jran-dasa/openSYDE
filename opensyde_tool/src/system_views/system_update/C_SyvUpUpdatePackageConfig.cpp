//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Data class, containing the update package configuration (implementation)

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "precomp_headers.h"

#include "C_SyvUpUpdatePackageConfig.h"

/* -- Used Namespaces ----------------------------------------------------------------------------------------------- */
using namespace stw_opensyde_gui_logic;

/* -- Module Global Constants --------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

/* -- Global Variables ---------------------------------------------------------------------------------------------- */

/* -- Module Global Variables --------------------------------------------------------------------------------------- */

/* -- Module Global Function Prototypes ----------------------------------------------------------------------------- */

/* -- Implementation ------------------------------------------------------------------------------------------------ */

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpUpdatePackageConfigNodeApp::C_SyvUpUpdatePackageConfigNodeApp() :
   e_Type(stw_opensyde_core::C_OSCNodeApplication::eBINARY),
   c_Name(""),
   c_Path("")
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpUpdatePackageConfigNode::C_SyvUpUpdatePackageConfigNode() :
   c_Name(""),
   c_DeviceType(""),
   c_PemFilePath(""),
   e_StateSecurity(C_PuiSvNodeUpdate::eST_SEC_NO_CHANGE),
   e_StateDebugger(C_PuiSvNodeUpdate::eST_DEB_NO_CHANGE)
{
}

//----------------------------------------------------------------------------------------------------------------------
/*! \brief   Default constructor
*/
//----------------------------------------------------------------------------------------------------------------------
C_SyvUpUpdatePackageConfig::C_SyvUpUpdatePackageConfig()
{
}
