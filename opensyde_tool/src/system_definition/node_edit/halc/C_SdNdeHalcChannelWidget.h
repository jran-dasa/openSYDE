//----------------------------------------------------------------------------------------------------------------------
/*!
   \file
   \brief       Widget for displaying and editing hardware configuration of a specific channel
   \copyright   Copyright 2019 Sensor-Technik Wiedemann GmbH. All rights reserved.
*/
//----------------------------------------------------------------------------------------------------------------------
#ifndef C_SDNDEHALCCHANNELWIDGET_H
#define C_SDNDEHALCCHANNELWIDGET_H

/* -- Includes ------------------------------------------------------------------------------------------------------ */
#include <QWidget>

#include "stwtypes.h"

#include "C_OSCHalcConfigDomain.h"

/* -- Namespace ----------------------------------------------------------------------------------------------------- */
namespace Ui
{
class C_SdNdeHalcChannelWidget;
}

namespace stw_opensyde_gui
{
/* -- Global Constants ---------------------------------------------------------------------------------------------- */

/* -- Types --------------------------------------------------------------------------------------------------------- */

class C_SdNdeHalcChannelWidget :
   public QWidget
{
   Q_OBJECT

public:
   explicit C_SdNdeHalcChannelWidget(QWidget * const opc_Parent = NULL);
   ~C_SdNdeHalcChannelWidget(void);

   void InitStaticNames(void) const;
   void SetNode(const stw_types::uint32 ou32_NodeIndex);
   void SetChannel(const stw_types::uint32 ou32_DomainIndex, const stw_types::uint32 ou32_ChannelIndex);
   void ReloadChannel(void);
   void SetNameFocus(void) const;
   void LoadUserSettings(void) const;
   void SaveUserSettings(void) const;

   //The signals keyword is necessary for Qt signal slot functionality
   //lint -save -e1736
Q_SIGNALS:
   //lint -restore
   void SigUpdateChannel(const stw_types::uint32 ou32_DomainIndex, const stw_types::uint32 ou32_ChannelIndex,
                         const bool oq_UseChannelIndex);
   void SigChannelSelected(const stw_types::uint32 ou32_DomainIndex, const stw_types::uint32 ou32_ChannelIndex,
                           const bool oq_UseChannelIndex);

private:
   Ui::C_SdNdeHalcChannelWidget * mpc_Ui;

   stw_types::uint32 mu32_NodeIndex;
   stw_types::uint32 mu32_DomainIndex;
   stw_types::uint32 mu32_ChannelIndex;
   bool mq_UseChannelIndex;
   std::vector<stw_types::uint32> mc_CbxUseCaseIndices;
   stw_types::sint32 ms32_LastComboboxUseCaseIndex;

   //Avoid call
   C_SdNdeHalcChannelWidget(const C_SdNdeHalcChannelWidget &);
   C_SdNdeHalcChannelWidget & operator =(const C_SdNdeHalcChannelWidget &);

   void m_OnNameEdited(void);
   void m_OnCommentEdited(void);
   void m_OnSafetyToggled(const bool oq_Checked) const;
   void m_HandleHalcNvmFlag(void);
   void m_OnUseCaseChanged(const stw_types::sint32 os32_NewIndex);
   void m_OnLinkedChannelClicked(const QString & orc_LinkedChannelName);
   void m_OnViewDatapoolDetailsClicked(void);
   void m_SetLinkedSymbolAndChannel(const bool oq_IsLinked,
                                    const std::vector<stw_types::uint32> & orc_LinkedChannelIndices) const;
   void m_LoadChannelData(void);
   void m_EmitUpdateSignal(void);
   void m_ConnectWidgets(const bool oq_Connect) const;
   void m_CheckName(const QString & orc_NewName) const;
   bool m_AskUserToContinueLinkingIfNecessary(const bool oq_IsLinkedOld, const bool oq_IsLinkedNew,
                                              const std::vector<QString> & orc_LinkedChannelNamesOld, const
                                              std::vector<QString> & orc_LinkedChannelNamesNew,
                                              const stw_types::uint32 ou32_NewUseCaseIndex);
};

/* -- Extern Global Variables --------------------------------------------------------------------------------------- */
} //end of namespace

#endif
