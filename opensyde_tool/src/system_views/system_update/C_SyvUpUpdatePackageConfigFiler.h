//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Update package configuration file reader/writer (header)

   See cpp file for detailed description

   \copyright   Copyright 2018 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SYVUPUPDATEPACKAGECONFIGFILER_H
#define C_SYVUPUPDATEPACKAGECONFIGFILER_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include "stwtypes.h"

#include "C_SyvUpUpdatePackageConfig.h"

#include "C_OSCXMLParser.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SyvUpUpdatePackageConfigFiler
{
public:
   static stw_types::sint32 h_LoadConfig(const QString & orc_FilePath, C_SyvUpUpdatePackageConfig & orc_Config);
   static stw_types::sint32 h_SaveConfig(const QString & orc_FilePath, const C_SyvUpUpdatePackageConfig & orc_Config);

   ///known file versions
   static const stw_types::uint16 hu16_FILE_VERSION_1 = 1U;
   static const stw_types::uint16 hu16_FILE_VERSION_LATEST = hu16_FILE_VERSION_1;

private:
   C_SyvUpUpdatePackageConfigFiler(void);

   static bool mh_SaveNode(stw_opensyde_core::C_OSCXMLParser & orc_XMLParser,
                           const C_SyvUpUpdatePackageConfigNode & orc_NodeConfig);
   static bool mh_SaveApp(stw_opensyde_core::C_OSCXMLParser & orc_XMLParser,
                          const C_SyvUpUpdatePackageConfigNodeApp & orc_AppConfig);
   static bool mh_SaveParamSet(stw_opensyde_core::C_OSCXMLParser & orc_XMLParser, const QString & orc_Path);
   static bool mh_SaveFile(stw_opensyde_core::C_OSCXMLParser & orc_XMLParser, const QString & orc_Path);
   static void mh_SaveNodeUpdateInformationPem(const C_SyvUpUpdatePackageConfigNode & orc_NodeConfig,
                                               stw_opensyde_core::C_OSCXMLParserBase & orc_XMLParser);

   static bool mh_LoadNodes(stw_opensyde_core::C_OSCXMLParser & orc_XMLParser, C_SyvUpUpdatePackageConfig & orc_Config);
   static bool mh_LoadApps(stw_opensyde_core::C_OSCXMLParser & orc_XMLParser,
                           C_SyvUpUpdatePackageConfigNode & orc_NodeConfig);
   static bool mh_LoadParamSets(stw_opensyde_core::C_OSCXMLParser & orc_XMLParser,
                                C_SyvUpUpdatePackageConfigNode & orc_NodeConfig);
   static bool mh_LoadFiles(stw_opensyde_core::C_OSCXMLParser & orc_XMLParser,
                            C_SyvUpUpdatePackageConfigNode & orc_NodeConfig);
   static stw_types::sint32 mh_LoadNodeUpdateInformationPem(C_SyvUpUpdatePackageConfigNode & orc_NodeConfig,
                                                            stw_opensyde_core::C_OSCXMLParserBase & orc_XMLParser);
   static stw_types::sint32 mh_LoadNodeUpdateInformationPemStates(C_SyvUpUpdatePackageConfigNode & orc_NodeConfig,
                                                                  stw_opensyde_core::C_OSCXMLParserBase & orc_XMLParser);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
