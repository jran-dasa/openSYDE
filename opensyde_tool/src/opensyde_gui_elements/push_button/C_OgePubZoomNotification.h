//-----------------------------------------------------------------------------
/*!
   \file
   \brief       Current zoom level notification item (header)

   Current zoom level notification item (note: main module description should be in .cpp file)

   \implementation
   project     openSYDE
   copyright   STW (c) 1999-20xx
   license     use only under terms of contract / confidential

   created     26.09.2016  STW/M.Echtler
   \endimplementation
*/
//-----------------------------------------------------------------------------
#ifndef C_OGEPUBZOOMNOTIFICATION_H
#define C_OGEPUBZOOMNOTIFICATION_H

/* -- Includes ------------------------------------------------------------- */

#include <QPushButton>

/* -- Namespace ------------------------------------------------------------ */
namespace stw_opensyde_gui_elements
{
/* -- Global Constants ----------------------------------------------------- */

/* -- Types ---------------------------------------------------------------- */

class C_OgePubZoomNotification :
   public QPushButton
{
   Q_OBJECT

public:
   C_OgePubZoomNotification(QWidget * const opc_Parent = NULL);
};

/* -- Extern Global Variables ---------------------------------------------- */
} //end of namespace

#endif