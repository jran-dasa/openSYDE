//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Group of device descriptions (header)

   Group of device descriptions (note: main module description should be in .cpp file)

   \copyright   Copyright 2016 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_OSCDEVICEGROUPH
#define C_OSCDEVICEGROUPH

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include "C_OSCDeviceDefinition.h"
#include "CSCLIniFile.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_core
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_OSCDeviceGroup
{
public:
   C_OSCDeviceGroup(void);

   const C_OSCDeviceDefinition * LookForDevice(const stw_scl::C_SCLString & orc_Name,
                                               const stw_scl::C_SCLString & orc_MainDeviceName,
                                               stw_types::uint32 & oru32_SubDeviceIndex) const;
   bool PreCheckDevice(const stw_scl::C_SCLString & orc_DeviceName, const stw_scl::C_SCLString & orc_DeviceNameAlias,
                       const stw_scl::C_SCLString & orc_DevicePath) const;
   stw_types::sint32 LoadGroup(stw_scl::C_SCLIniFile & orc_Ini, const stw_scl::C_SCLString & orc_BasePath);

   void SetGroupName(const stw_scl::C_SCLString & orc_GroupName);
   stw_scl::C_SCLString GetGroupName(void) const;

   std::vector<C_OSCDeviceDefinition> GetDevices(void) const;

private:
   stw_scl::C_SCLString mc_GroupName;             ///< Group name
   std::vector<C_OSCDeviceDefinition> mc_Devices; ///< All devices belonging to this group
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
