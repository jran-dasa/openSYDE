//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       File handler for device definition file data (V1)
   \copyright   Copyright 2021 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_OSCDEVICEDEFINITIONFILERV1_H
#define C_OSCDEVICEDEFINITIONFILERV1_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"
#include "CSCLString.h"
#include "C_OSCDeviceDefinition.h"
#include "C_OSCXMLParser.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_core
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_OSCDeviceDefinitionFilerV1
{
private:
   static const stw_types::uint16 mhu16_FILE_VERSION; ///< file version this class can read / write

   static void mh_ParseOpenSydeAvailability(const C_OSCXMLParser & orc_Parser, bool & orq_ProtocolSupportedCan,
                                            bool & orq_ProtocolSupportedEthernet);
   static void mh_ParseOpenSydeFlashloaderParameter(const C_OSCXMLParser & orc_Parser,
                                                    stw_types::uint32 & oru32_RequestDownloadTimeout,
                                                    stw_types::uint32 & oru32_TransferDataTimeout,
                                                    bool & orq_IsFileBased);
   static void mh_ParseSTWFlashloaderAvailability(const C_OSCXMLParser & orc_Parser, bool & orq_ProtocolSupportedCan);

public:
   static stw_types::sint32 h_Load(C_OSCDeviceDefinition & orc_DeviceDefinition, C_OSCXMLParser & orc_Parser,
                                   const stw_scl::C_SCLString & orc_Path);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
