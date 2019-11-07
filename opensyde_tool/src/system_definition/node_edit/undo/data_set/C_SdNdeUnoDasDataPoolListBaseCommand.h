//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Data pool list data set base command (header)

   See cpp file for detailed description

   \copyright   Copyright 2017 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SDNDEUNODASDATAPOOLLISTBASECOMMAND_H
#define C_SDNDEUNODASDATAPOOLLISTBASECOMMAND_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */

#include <QString>
#include <QUndoCommand>
#include "C_SdNdeDpListModelViewManager.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace stw_opensyde_gui_logic
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SdNdeUnoDasDataPoolListBaseCommand :
   public QUndoCommand
{
public:
   C_SdNdeUnoDasDataPoolListBaseCommand(const stw_types::uint32 & oru32_NodeIndex,
                                        const stw_types::uint32 & oru32_DataPoolIndex,
                                        const stw_types::uint32 & oru32_DataPoolListIndex,
                                        C_SdNdeDpListModelViewManager * const opc_DataPoolListModelViewManager,
                                        const QString & orc_Text = "", QUndoCommand * const opc_Parent = NULL);

protected:
   const stw_types::uint32 mu32_NodeIndex;
   const stw_types::uint32 mu32_DataPoolIndex;
   const stw_types::uint32 mu32_DataPoolListIndex;
   C_SdNdeDpListModelViewManager * const mpc_DataPoolListModelViewManager;
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
