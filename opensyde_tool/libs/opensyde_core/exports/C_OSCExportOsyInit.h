//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Export initialization module for DPD and DPH.

   See cpp file for detailed description

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     23.03.2018  STW/A.Stangl
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_OSCEXPORTOSYINITH
#define C_OSCEXPORTOSYINITH

/* -- Includes ------------------------------------------------------------- */
#include <vector>

#include "stwtypes.h"

#include "CSCLStringList.h"
#include "C_OSCNode.h"

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_core
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */
///Code exporter for server side DPD and DPH init structures
class C_OSCExportOsyInit
{
public:
   C_OSCExportOsyInit(void);
   virtual ~C_OSCExportOsyInit(void);

   static stw_types::sint32 h_CreateSourceCode(const stw_scl::C_SCLString & orc_FilePath, const C_OSCNode & orc_Node,
                                               const bool oq_RunsDpd, const stw_types::uint16 ou16_ApplicationIndex);

private:
   static bool mh_IsDpdInitRequired(const C_OSCNodeComInterfaceSettings & orc_Settings);

   static stw_types::uint32 mh_GetSizeOfLargestDataPoolElement(const std::vector<C_OSCNodeDataPool> & orc_DataPools);
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif