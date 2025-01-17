#-------------------------------------------------
#
# Project created by QtCreator 2016-05-03T11:00:30
#
#-------------------------------------------------

QT       += core gui
CONFIG   += precompile_header
CONFIG   += no_keywords

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets svg

TARGET = openSYDE_CAN_Monitor
TEMPLATE = app

DESTDIR = ../result/tool/CAN_Monitor

#version info:
RC_FILE = ../src/can_monitor/cam_resources.rc

#include relevant openSYDE core modules
opensyde_core_skip_modules += opensyde_core_skip_code_generation
include(../libs/opensyde_core/opensyde_core.pri)

SOURCES += \
    ../src/gettext/C_GtGetText.cpp \
    ../src/util/C_Uti.cpp \
    ../src/system_views/communication/C_SyvComMessageMonitor.cpp \
    ../src/system_views/communication/C_SyvComMessageLoggerFileBlf.cpp \
    ../src/system_views/communication/C_SyvComDriverThread.cpp \
    ../src/com_import_export/C_CieImportDbc.cpp \
    ../src/system_definition/node_edit/datapools/C_SdNdeDpContentUtil.cpp\
    ../src/can_monitor/cam_main.cpp \
    ../src/project_operations/C_PopErrorHandling.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/Attribute.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/AttributeDefinition.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/AttributeRelation.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/BitTiming.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/EnvironmentVariable.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/ExtendedMultiplexor.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/File.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/FileLoad.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/FileSave.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/Message.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/Network.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/Node.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/Signal.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/SignalGroup.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/SignalType.cpp \
    ../libs/dbc_driver_library/src/Vector/DBC/ValueTable.cpp \
    ../src/opensyde_gui_elements/widget/C_OgeWiCustomMessage.cpp \
    ../src/opensyde_gui_elements/widget/C_OgeWiError.cpp \
    ../src/opensyde_gui_elements/C_OgePopUpDialog.cpp \
    ../src/opensyde_gui_elements/C_OgeOverlay.cpp \
    ../src/opensyde_gui_elements/group_box/C_OgeGbxMouseMove.cpp \
    ../src/opensyde_gui_elements/label/C_OgeLabHeadingMessage.cpp \
    ../src/opensyde_gui_elements/label/C_OgeLabDescriptionMessage.cpp \
    ../src/opensyde_gui_elements/push_button/C_OgePubCancel.cpp \
    ../src/opensyde_gui_elements/push_button/C_OgePubDialog.cpp \
    ../src/opensyde_gui_elements/push_button/C_OgePubIconText.cpp \
    ../src/opensyde_gui_elements/text_browser/C_OgeTebMessageDetails.cpp \
    ../src/navigable_gui/C_NagToolTip.cpp \
    ../src/opensyde_gui_elements/widget/C_OgeWiUtil.cpp \
    ../src/opensyde_gui_elements/label/C_OgeLabPopUpTitle.cpp \
    ../src/help_engine/C_HeHandler.cpp \
    ../src/opensyde_gui_elements/group_box/C_OgeGbxHover.cpp \
    ../src/opensyde_gui_elements/group_box/C_OgeGbxTransparent.cpp \
    ../src/opensyde_gui_elements/label/C_OgeLabContextMenuBase.cpp \
    ../src/opensyde_gui_elements/label/C_OgeLabPopUpSubTitle.cpp \
    ../src/opensyde_gui_elements/frame/C_OgeFraBorder.cpp \
    ../src/opensyde_gui_elements/push_button/C_OgePubStandard.cpp \
    ../src/opensyde_gui_elements/text_browser/C_OgeTebContextMenuBase.cpp \
    ../src/opensyde_gui_elements/group_box/C_OgeGbxToolTip.cpp \
    ../src/opensyde_gui_elements/group_box/C_OgeGbxToolTipBase.cpp \
    ../src/opensyde_gui_elements/label/C_OgeLabHeadingToolTip.cpp \
    ../src/opensyde_gui_elements/text_edit/C_OgeTedToolTipContent.cpp \
    ../src/opensyde_gui_elements/C_OgeContextMenu.cpp \
    ../src/opensyde_gui_elements/C_OgeToolTipBase.cpp \
    ../src/opensyde_gui_elements/push_button/C_OgePubToolTipBase.cpp \
    ../src/can_monitor/C_CamMainWindow.cpp \
    ../src/opensyde_gui_elements/widget/C_OgeWiOnlyBackground.cpp \
    ../src/opensyde_gui_elements/widget/C_OgeWiWithToolTip.cpp \
    ../src/can_monitor/message_trace/C_CamMetTreeDelegate.cpp \
    ../src/can_monitor/message_trace/C_CamMetTreeGUIBuffer.cpp \
    ../src/can_monitor/message_trace/C_CamMetTreeModel.cpp \
    ../src/can_monitor/message_trace/C_CamMetTreeView.cpp \
    ../src/table_base/tree_base/C_TblTreItem.cpp \
    ../src/table_base/tree_base/C_TblTreModel.cpp \
    ../src/opensyde_gui_elements/push_button/C_OgePubSvgIconOnly.cpp \
    ../src/opensyde_gui_elements/label/C_OgeLabSvgOnly.cpp \
    ../src/can_monitor/user_settings/C_UsHandler.cpp \
    ../src/can_monitor/user_settings/C_UsFiler.cpp \
    ../src/navigable_gui/C_NagAboutDialog.cpp \
    ../src/opensyde_gui_elements/group_box/C_OgeGbx.cpp \
    ../src/opensyde_gui_elements/label/C_OgeLabGroupItem.cpp \
    ../src/opensyde_gui_elements/label/C_OgeLabGroupItemValue.cpp \
    ../src/opensyde_gui_elements/label/C_OgeLabToolTipBase.cpp \
    ../src/can_monitor/C_CamComDriverBase.cpp \
    ../src/can_monitor/message_trace/C_CamMetWidget.cpp \
    ../src/can_monitor/message_trace/C_CamMetStatusBarWidget.cpp \
    ../src/can_monitor/message_trace/C_CamMetControlBarWidget.cpp \
    ../src/can_monitor/message_generator/C_CamGenWidget.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosWidget.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosDllWidget.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosFilterWidget.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseWidget.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosLoggingWidget.cpp \
    ../src/can_monitor/C_CamTitleBarWidget.cpp \
    ../src/can_monitor/util/C_CamUtiStyleSheets.cpp \
    ../src/can_monitor/can_monitor_gui_elements/label/C_CamOgeLabStatusBar.cpp \
    ../src/util/C_UtiClipBoardHelper.cpp \
    ../src/can_monitor/message_trace/C_CamMetClipBoardHelper.cpp \
    ../src/can_monitor/can_monitor_gui_elements/splitter/C_CamOgeSpi.cpp \
    ../src/opensyde_gui_elements/splitter/C_OgeSpiBase.cpp \
    ../src/can_monitor/can_monitor_gui_elements/widget/C_CamOgeWiSectionHeader.cpp \
    ../src/opensyde_gui_elements/label/C_OgeLabGenericNoPaddingNoMargins.cpp \
    ../src/can_monitor/can_monitor_gui_elements/widget/C_CamOgeWiSettingSubSection.cpp \
    ../src/opensyde_gui_elements/radio_button/C_OgeRabProperties.cpp \
    ../src/opensyde_gui_elements/radio_button/C_OgeRabToolTipBase.cpp \
    ../src/opensyde_gui_elements/frame/C_OgeFraSeparator.cpp \
    ../src/opensyde_gui_elements/push_button/C_OgePubProjAction.cpp \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxToolTipBase.cpp \
    ../src/can_monitor/can_monitor_gui_elements/combo_box/C_CamOgeCbxWhite.cpp \
    ../src/can_monitor/can_monitor_gui_elements/push_button/C_CamOgePubAbout.cpp \
    ../src/can_monitor/message_generator/C_CamGenTableView.cpp \
    ../src/can_monitor/message_generator/C_CamGenTableModel.cpp \
    ../src/opensyde_gui_elements/line_edit/C_OgeLeContextMenuBase.cpp \
    ../src/opensyde_gui_elements/line_edit/C_OgeLeDark.cpp \
    ../src/util/C_UtiFindNameHelper.cpp \
    ../src/can_monitor/message_generator/C_CamGenMessagesWidget.cpp \
    ../src/can_monitor/project/C_CamProHandler.cpp \
    ../src/can_monitor/project/C_CamProHandlerFiler.cpp \
    ../src/can_monitor/project/C_CamProMessageData.cpp \
    ../src/can_monitor/can_monitor_gui_elements/push_button/C_CamOgePubProjOp.cpp \
    ../src/can_monitor/project/C_CamProClipBoardHelper.cpp \
    ../src/table_base/C_TblModelAction.cpp \
    ../src/can_monitor/can_monitor_gui_elements/push_button/C_CamOgePubSettingsAdd.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosFilterItemWidget.cpp \
    ../src/opensyde_gui_elements/C_OgeTransparentScrollArea.cpp \
    ../src/can_monitor/can_monitor_gui_elements/check_box/C_CamOgeChxSettings.cpp \
    ../src/opensyde_gui_elements/check_box/C_OgeChxToolTipBase.cpp \
    ../src/table_base/C_TblViewScroll.cpp \
    ../src/table_base/C_TblViewToolTipBase.cpp \
    ../src/table_base/C_TblDelegate.cpp \
    ../src/opensyde_gui_elements/line_edit/C_OgeLeToolTipBase.cpp \
    ../src/can_monitor/can_monitor_gui_elements/combo_box/C_CamOgeCbxTable.cpp \
    ../src/can_monitor/can_monitor_gui_elements/line_edit/C_CamOgeLeTableEdit.cpp \
    ../src/table_base/C_TblEditLineEditBase.cpp \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxIconDelegate.cpp \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxResizingView.cpp \
    ../src/can_monitor/can_monitor_gui_elements/group_box/C_CamOgeGbxMessageGeneratorBackground.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosFilterPopup.cpp \
    ../src/opensyde_gui_elements/line_edit/C_OgeLePropertiesName.cpp \
    ../src/opensyde_gui_elements/text_edit/C_OgeTedPropertiesComment.cpp \
    ../src/opensyde_gui_elements/text_edit/C_OgeTedToolTipBase.cpp \
    ../src/opensyde_gui_elements/text_edit/C_OgeTedContextMenuBase.cpp \
    ../src/opensyde_gui_elements/push_button/C_OgePubConfigure.cpp \
    ../src/can_monitor/can_monitor_gui_elements/line_edit/C_CamOgeLeIDPopUp.cpp \
    ../src/opensyde_gui_elements/check_box/C_OgeChxProperties.cpp \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxText.cpp \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxIconOnly.cpp \
    ../src/can_monitor/project/C_CamProFilterData.cpp \
    ../src/opensyde_gui_elements/push_button/C_OgePubIconOnly.cpp \
    ../src/opensyde_gui_elements/scroll_area/C_OgeSaMain.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosFilterTableView.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosFilterTableModel.cpp \
    ../src/can_monitor/message_generator/signals/C_CamGenSigTableView.cpp \
    ../src/can_monitor/message_generator/signals/C_CamGenSigTableModel.cpp \
    ../src/can_monitor/message_generator/signals/C_CamGenSigWidget.cpp \
    ../src/table_base/C_TblViewInteraction.cpp \
    ../src/can_monitor/message_trace/C_CamMetTreeLoggerData.cpp \
    ../src/can_monitor/can_monitor_gui_elements/group_box/C_CamOgeGbxMessageSignalsBorder.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseItemWidget.cpp \
    ../src/can_monitor/project/C_CamProDatabaseData.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseBusSelectionPopup.cpp \
    ../src/opensyde_gui_elements/label/C_OgeLabHeadingGroupBold.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosSectionPopup.cpp \
    ../src/can_monitor/message_generator/signals/C_CamGenSigTableDelegate.cpp \
    ../src/can_monitor/message_generator/C_CamGenKeySelect.cpp \
    ../src/can_monitor/can_monitor_gui_elements/spin_box/C_CamOgeSpxProperties.cpp \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxAllBase.cpp \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxToolTipBase.cpp \
    ../src/opensyde_gui_elements/widget/C_OgeWiSpinBoxGroup.cpp \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxAutoFixBase.cpp \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxDoubleAutoFix.cpp \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxInt64.cpp \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxInt64AutoFix.cpp \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxDoubleToolTipBase.cpp \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxInt64ToolTipBase.cpp \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxDoubleDynamicDecimalsBase.cpp \
    ../src/can_monitor/can_monitor_gui_elements/widget/C_CamOgeWiSpinBoxGroupTable.cpp \
    ../src/can_monitor/can_monitor_gui_elements/combo_box/C_CamOgeCbxDark.cpp \
    ../src/can_monitor/can_monitor_gui_elements/push_button/C_CamOgePubDarkBrowse.cpp \
    ../src/can_monitor/project/C_CamProLoggingData.cpp \
    ../src/can_monitor/data_base/C_CamDbDbc.cpp \
    ../src/can_monitor/data_base/C_CamDbDbcMessageId.cpp \
    ../src/can_monitor/data_base/C_CamDbHandler.cpp \
    ../src/can_monitor/data_base/C_CamDbOsy.cpp \
    ../src/can_monitor/message_generator/signals/C_CamGenSigUtil.cpp \
    ../src/can_monitor/can_monitor_gui_elements/menu/C_CamOgeMuRecentProjects.cpp \
    ../src/opensyde_gui_elements/line_edit/C_OgeLeProperties.cpp \
    ../src/table_base/tree_base/C_TblTreDataElementSortFilter.cpp \
    ../src/opensyde_gui_elements/C_OgeTreeViewToolTipBase.cpp \
    ../src/system_definition/C_SdTooltipUtil.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseSelectionModel.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseSelectionPopup.cpp \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseSelectionView.cpp \
    ../src/can_monitor/can_monitor_gui_elements/combo_box/C_CamOgeCbxTableSmall.cpp \
    ../src/can_monitor/can_monitor_gui_elements/tool_button/C_CamOgeTobRecentProjects.cpp \
    ../src/opensyde_gui_elements/tool_button/C_OgeTobToolTipBase.cpp \
    ../src/can_monitor/util/C_CamUti.cpp \
    ../src/can_monitor/message_generator/C_CamGenTableDelegate.cpp \
    ../src/table_base/C_TblDelegateUtil.cpp \
    ../src/opensyde_gui_elements/C_OgeTreeToolTipBase.cpp \
    ../src/can_monitor/can_monitor_gui_elements/push_button/C_CamOgePubPathVariables.cpp \
    ../src/opensyde_gui_elements/line_edit/C_OgeLeFilePathBase.cpp \
    ../src/can_monitor/can_monitor_gui_elements/line_edit/C_CamOgeLeFilePath.cpp \
    ../src/opensyde_gui_elements/menu/C_OgeMuSections.cpp \
    ../src/table_base/tree_base/C_TblTreSimpleItem.cpp \
    ../src/table_base/tree_base/C_TblTreSimpleModel.cpp \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxTableBase.cpp \
    ../src/can_monitor/message_trace/C_CamMetTreeLoggerDataGreyOutInformation.cpp \
    ../src/opensyde_gui_elements/push_button/C_OgePubMessageCancel.cpp \
    ../src/opensyde_gui_elements/push_button/C_OgePubMessageOk.cpp \
    ../src/can_monitor/table_base/C_CamTblDelegate.cpp \
    ../src/can_monitor/message_trace/C_CamMetUtil.cpp \
    ../src/can_monitor/message_trace/C_CamMetSettingsPopup.cpp \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxMultiSelect.cpp \
    ../src/opensyde_gui_elements/label/C_OgeLabElided.cpp \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxDoubleAutoFixCustomTrigger.cpp \
    ../src/can_monitor/data_base/C_CamDbOsyMessageId.cpp \
    ../src/can_monitor/data_base/C_CamDbOsyListId.cpp \
    ../src/can_monitor/data_base/C_CamDbDbcUnmappedMessageId.cpp

PRECOMPILED_HEADER = ../src/precompiled_headers/can_monitor/precomp_headers.h

HEADERS  += \
    ../src/gettext/C_GtGetText.h \
    ../src/util/C_Uti.h \
    ../src/system_views/communication/C_SyvComMessageMonitor.h \
    ../src/system_views/communication/C_SyvComMessageLoggerFileBlf.h \
    ../src/system_views/communication/C_SyvComDriverThread.h \
    ../libs/dbc_driver_library/src/Vector/DBC/Attribute.h \
    ../libs/dbc_driver_library/src/Vector/DBC/AttributeDefinition.h \
    ../libs/dbc_driver_library/src/Vector/DBC/AttributeRelation.h \
    ../libs/dbc_driver_library/src/Vector/DBC/AttributeValueType.h \
    ../libs/dbc_driver_library/src/Vector/DBC/BitTiming.h \
    ../libs/dbc_driver_library/src/Vector/DBC/ByteOrder.h \
    ../libs/dbc_driver_library/src/Vector/DBC/config.h \
    ../libs/dbc_driver_library/src/Vector/DBC/Doxygen.h \
    ../libs/dbc_driver_library/src/Vector/DBC/EnvironmentVariable.h \
    ../libs/dbc_driver_library/src/Vector/DBC/ExtendedMultiplexor.h \
    ../libs/dbc_driver_library/src/Vector/DBC/File.h \
    ../libs/dbc_driver_library/src/Vector/DBC/Message.h \
    ../libs/dbc_driver_library/src/Vector/DBC/Network.h \
    ../libs/dbc_driver_library/src/Vector/DBC/Node.h \
    ../libs/dbc_driver_library/src/Vector/DBC/platform.h \
    ../libs/dbc_driver_library/src/Vector/DBC/Signal.h \
    ../libs/dbc_driver_library/src/Vector/DBC/SignalGroup.h \
    ../libs/dbc_driver_library/src/Vector/DBC/SignalType.h \
    ../libs/dbc_driver_library/src/Vector/DBC/Status.h \
    ../libs/dbc_driver_library/src/Vector/DBC/ValueDescriptions.h \
    ../libs/dbc_driver_library/src/Vector/DBC/ValueTable.h \
    ../libs/dbc_driver_library/src/Vector/DBC/ValueType.h \
    ../libs/dbc_driver_library/src/Vector/DBC/vector_dbc_export.h \
    ../libs/dbc_driver_library/src/Vector/DBC.h \
    ../src/com_import_export/C_CieImportDbc.h \
    ../src/com_import_export/C_CieConverter.h \
    ../src/system_definition/node_edit/datapools/C_SdNdeDpContentUtil.h \
    ../src/project_operations/C_PopErrorHandling.h \
    ../src/opensyde_gui_elements/widget/C_OgeWiCustomMessage.h \
    ../src/opensyde_gui_elements/widget/C_OgeWiError.h \
    ../src/opensyde_gui_elements/C_OgePopUpDialog.h \
    ../src/opensyde_gui_elements/C_OgeOverlay.h \
    ../src/opensyde_gui_elements/group_box/C_OgeGbxMouseMove.h \
    ../src/opensyde_gui_elements/label/C_OgeLabHeadingMessage.h \
    ../src/opensyde_gui_elements/label/C_OgeLabDescriptionMessage.h \
    ../src/opensyde_gui_elements/push_button/C_OgePubCancel.h \
    ../src/opensyde_gui_elements/push_button/C_OgePubDialog.h \
    ../src/opensyde_gui_elements/push_button/C_OgePubIconText.h \
    ../src/opensyde_gui_elements/text_browser/C_OgeTebMessageDetails.h \
    ../src/navigable_gui/C_NagToolTip.h \
    ../src/opensyde_gui_elements/widget/C_OgeWiUtil.h \
    ../src/opensyde_gui_elements/label/C_OgeLabPopUpTitle.h \
    ../src/help_engine/C_HeHandler.h \
    ../src/opensyde_gui_elements/group_box/C_OgeGbxHover.h \
    ../src/opensyde_gui_elements/group_box/C_OgeGbxTransparent.h \
    ../src/opensyde_gui_elements/label/C_OgeLabContextMenuBase.h \
    ../src/opensyde_gui_elements/label/C_OgeLabPopUpSubTitle.h \
    ../src/opensyde_gui_elements/frame/C_OgeFraBorder.h \
    ../src/opensyde_gui_elements/push_button/C_OgePubStandard.h \
    ../src/opensyde_gui_elements/text_browser/C_OgeTebContextMenuBase.h \
    ../src/opensyde_gui_elements/group_box/C_OgeGbxToolTip.h \
    ../src/opensyde_gui_elements/group_box/C_OgeGbxToolTipBase.h \
    ../src/opensyde_gui_elements/label/C_OgeLabHeadingToolTip.h \
    ../src/opensyde_gui_elements/text_edit/C_OgeTedToolTipContent.h \
    ../src/opensyde_gui_elements/C_OgeContextMenu.h \
    ../src/opensyde_gui_elements/C_OgeToolTipBase.h \
    ../src/opensyde_gui_elements/push_button/C_OgePubToolTipBase.h \
    ../src/can_monitor/C_CamMainWindow.h \
    ../src/opensyde_gui_elements/widget/C_OgeWiOnlyBackground.h \
    ../src/opensyde_gui_elements/widget/C_OgeWiWithToolTip.h \
    ../src/can_monitor/message_trace/C_CamMetTreeDelegate.h \
    ../src/can_monitor/message_trace/C_CamMetTreeGUIBuffer.h \
    ../src/can_monitor/message_trace/C_CamMetTreeModel.h \
    ../src/can_monitor/message_trace/C_CamMetTreeView.h \
    ../src/table_base/tree_base/C_TblTreItem.h \
    ../src/table_base/tree_base/C_TblTreModel.h \
    ../src/opensyde_gui_elements/push_button/C_OgePubSvgIconOnly.h \
    ../src/opensyde_gui_elements/label/C_OgeLabSvgOnly.h \
    ../src/can_monitor/cam_constants.h \
    ../src/can_monitor/user_settings/C_UsHandler.h \
    ../src/can_monitor/user_settings/C_UsFiler.h \
    ../src/navigable_gui/C_NagAboutDialog.h \
    ../src/opensyde_gui_elements/group_box/C_OgeGbx.h \
    ../src/opensyde_gui_elements/label/C_OgeLabGroupItem.h \
    ../src/opensyde_gui_elements/label/C_OgeLabGroupItemValue.h \
    ../src/opensyde_gui_elements/label/C_OgeLabToolTipBase.h \
    ../src/can_monitor/C_CamComDriverBase.h \
    ../src/can_monitor/message_trace/C_CamMetWidget.h \
    ../src/can_monitor/message_trace/C_CamMetStatusBarWidget.h \
    ../src/can_monitor/message_trace/C_CamMetControlBarWidget.h \
    ../src/can_monitor/message_generator/C_CamGenWidget.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosWidget.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosDllWidget.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosFilterWidget.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseWidget.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosLoggingWidget.h \
    ../src/can_monitor/C_CamTitleBarWidget.h \
    ../src/can_monitor/util/C_CamUtiStyleSheets.h \
    ../src/can_monitor/can_monitor_gui_elements/label/C_CamOgeLabStatusBar.h \
    ../src/constants.h \
    ../src/util/C_UtiClipBoardHelper.h \
    ../src/can_monitor/message_trace/C_CamMetClipBoardHelper.h \
    ../src/can_monitor/can_monitor_gui_elements/splitter/C_CamOgeSpi.h \
    ../src/opensyde_gui_elements/splitter/C_OgeSpiBase.h \
    ../src/can_monitor/can_monitor_gui_elements/widget/C_CamOgeWiSectionHeader.h \
    ../src/opensyde_gui_elements/label/C_OgeLabGenericNoPaddingNoMargins.h \
    ../src/can_monitor/can_monitor_gui_elements/widget/C_CamOgeWiSettingSubSection.h \
    ../src/opensyde_gui_elements/radio_button/C_OgeRabProperties.h \
    ../src/opensyde_gui_elements/radio_button/C_OgeRabToolTipBase.h \
    ../src/opensyde_gui_elements/frame/C_OgeFraSeparator.h \
    ../src/opensyde_gui_elements/push_button/C_OgePubProjAction.h \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxToolTipBase.h \
    ../src/can_monitor/can_monitor_gui_elements/combo_box/C_CamOgeCbxWhite.h \
    ../src/can_monitor/can_monitor_gui_elements/push_button/C_CamOgePubAbout.h \
    ../src/can_monitor/message_generator/C_CamGenTableView.h \
    ../src/can_monitor/message_generator/C_CamGenTableModel.h \
    ../src/opensyde_gui_elements/line_edit/C_OgeLeContextMenuBase.h \
    ../src/opensyde_gui_elements/line_edit/C_OgeLeDark.h \
    ../src/util/C_UtiFindNameHelper.h \
    ../src/can_monitor/message_generator/C_CamGenMessagesWidget.h \
    ../src/can_monitor/project/C_CamProHandler.h \
    ../src/can_monitor/project/C_CamProHandlerFiler.h \
    ../src/can_monitor/project/C_CamProMessageData.h \
    ../src/can_monitor/can_monitor_gui_elements/push_button/C_CamOgePubProjOp.h \
    ../src/can_monitor/project/C_CamProClipBoardHelper.h \
    ../src/table_base/C_TblModelAction.h \
    ../src/can_monitor/can_monitor_gui_elements/push_button/C_CamOgePubSettingsAdd.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosFilterItemWidget.h \
    ../src/opensyde_gui_elements/C_OgeTransparentScrollArea.h \
    ../src/can_monitor/can_monitor_gui_elements/check_box/C_CamOgeChxSettings.h \
    ../src/opensyde_gui_elements/check_box/C_OgeChxToolTipBase.h \
    ../src/table_base/C_TblViewScroll.h \
    ../src/table_base/C_TblViewToolTipBase.h \
    ../src/table_base/C_TblDelegate.h \
    ../src/opensyde_gui_elements/line_edit/C_OgeLeToolTipBase.h \
    ../src/can_monitor/can_monitor_gui_elements/combo_box/C_CamOgeCbxTable.h \
    ../src/can_monitor/can_monitor_gui_elements/line_edit/C_CamOgeLeTableEdit.h \
    ../src/table_base/C_TblEditLineEditBase.h \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxIconDelegate.h \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxResizingView.h \
    ../src/can_monitor/can_monitor_gui_elements/group_box/C_CamOgeGbxMessageGeneratorBackground.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosFilterPopup.h \
    ../src/opensyde_gui_elements/line_edit/C_OgeLePropertiesName.h \
    ../src/opensyde_gui_elements/text_edit/C_OgeTedPropertiesComment.h \
    ../src/opensyde_gui_elements/text_edit/C_OgeTedToolTipBase.h \
    ../src/opensyde_gui_elements/text_edit/C_OgeTedContextMenuBase.h \
    ../src/opensyde_gui_elements/push_button/C_OgePubConfigure.h \
    ../src/can_monitor/can_monitor_gui_elements/line_edit/C_CamOgeLeIDPopUp.h \
    ../src/opensyde_gui_elements/check_box/C_OgeChxProperties.h \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxText.h \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxIconOnly.h \
    ../src/can_monitor/project/C_CamProFilterData.h \
    ../src/opensyde_gui_elements/push_button/C_OgePubIconOnly.h \
    ../src/opensyde_gui_elements/scroll_area/C_OgeSaMain.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosFilterTableView.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosFilterTableModel.h \
    ../src/can_monitor/message_generator/signals/C_CamGenSigTableView.h \
    ../src/can_monitor/message_generator/signals/C_CamGenSigTableModel.h \
    ../src/can_monitor/message_generator/signals/C_CamGenSigWidget.h \
    ../src/table_base/C_TblViewInteraction.h \
    ../src/can_monitor/message_trace/C_CamMetTreeLoggerData.h \
    ../src/can_monitor/can_monitor_gui_elements/group_box/C_CamOgeGbxMessageSignalsBorder.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseItemWidget.h \
    ../src/can_monitor/project/C_CamProDatabaseData.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseBusSelectionPopup.h \
    ../libs/blf_driver_library/BLF.h \
    ../libs/blf_driver_library/BLF/AbstractFile.h \
    ../libs/blf_driver_library/BLF/AfdxFrame.h \
    ../libs/blf_driver_library/BLF/AfdxStatistic.h \
    ../libs/blf_driver_library/BLF/AppText.h \
    ../libs/blf_driver_library/BLF/AppTrigger.h \
    ../libs/blf_driver_library/BLF/CanDriverError.h \
    ../libs/blf_driver_library/BLF/CanDriverErrorExt.h \
    ../libs/blf_driver_library/BLF/CanDriverHwSync.h \
    ../libs/blf_driver_library/BLF/CanDriverStatistic.h \
    ../libs/blf_driver_library/BLF/CanErrorFrame.h \
    ../libs/blf_driver_library/BLF/CanErrorFrameExt.h \
    ../libs/blf_driver_library/BLF/CanFdErrorFrame64.h \
    ../libs/blf_driver_library/BLF/CanFdMessage.h \
    ../libs/blf_driver_library/BLF/CanFdMessage64.h \
    ../libs/blf_driver_library/BLF/CanMessage.h \
    ../libs/blf_driver_library/BLF/CanMessage2.h \
    ../libs/blf_driver_library/BLF/CanOverloadFrame.h \
    ../libs/blf_driver_library/BLF/CompressedFile.h \
    ../libs/blf_driver_library/BLF/config.h \
    ../libs/blf_driver_library/BLF/Doxygen.h \
    ../libs/blf_driver_library/BLF/DriverOverrun.h \
    ../libs/blf_driver_library/BLF/EnvironmentVariable.h \
    ../libs/blf_driver_library/BLF/EthernetFrame.h \
    ../libs/blf_driver_library/BLF/EthernetRxError.h \
    ../libs/blf_driver_library/BLF/EthernetStatus.h \
    ../libs/blf_driver_library/BLF/EventComment.h \
    ../libs/blf_driver_library/BLF/Exceptions.h \
    ../libs/blf_driver_library/BLF/File.h \
    ../libs/blf_driver_library/BLF/FileStatistics.h \
    ../libs/blf_driver_library/BLF/FlexRayData.h \
    ../libs/blf_driver_library/BLF/FlexRayStatusEvent.h \
    ../libs/blf_driver_library/BLF/FlexRaySync.h \
    ../libs/blf_driver_library/BLF/FlexRayV6Message.h \
    ../libs/blf_driver_library/BLF/FlexRayV6StartCycleEvent.h \
    ../libs/blf_driver_library/BLF/FlexRayVFrError.h \
    ../libs/blf_driver_library/BLF/FlexRayVFrReceiveMsg.h \
    ../libs/blf_driver_library/BLF/FlexRayVFrReceiveMsgEx.h \
    ../libs/blf_driver_library/BLF/FlexRayVFrStartCycle.h \
    ../libs/blf_driver_library/BLF/FlexRayVFrStatus.h \
    ../libs/blf_driver_library/BLF/GlobalMarker.h \
    ../libs/blf_driver_library/BLF/GpsEvent.h \
    ../libs/blf_driver_library/BLF/J1708Message.h \
    ../libs/blf_driver_library/BLF/KLineStatusEvent.h \
    ../libs/blf_driver_library/BLF/LinBaudrateEvent.h \
    ../libs/blf_driver_library/BLF/LinBusEvent.h \
    ../libs/blf_driver_library/BLF/LinChecksumInfo.h \
    ../libs/blf_driver_library/BLF/LinCrcError.h \
    ../libs/blf_driver_library/BLF/LinCrcError2.h \
    ../libs/blf_driver_library/BLF/LinDatabyteTimestampEvent.h \
    ../libs/blf_driver_library/BLF/LinDisturbanceEvent.h \
    ../libs/blf_driver_library/BLF/LinDlcInfo.h \
    ../libs/blf_driver_library/BLF/LinLongDomSignalEvent.h \
    ../libs/blf_driver_library/BLF/LinLongDomSignalEvent2.h \
    ../libs/blf_driver_library/BLF/LinMessage.h \
    ../libs/blf_driver_library/BLF/LinMessage2.h \
    ../libs/blf_driver_library/BLF/LinMessageDescriptor.h \
    ../libs/blf_driver_library/BLF/LinReceiveError.h \
    ../libs/blf_driver_library/BLF/LinReceiveError2.h \
    ../libs/blf_driver_library/BLF/LinSchedulerModeChange.h \
    ../libs/blf_driver_library/BLF/LinSendError.h \
    ../libs/blf_driver_library/BLF/LinSendError2.h \
    ../libs/blf_driver_library/BLF/LinShortOrSlowResponse.h \
    ../libs/blf_driver_library/BLF/LinSlaveTimeout.h \
    ../libs/blf_driver_library/BLF/LinSleepModeEvent.h \
    ../libs/blf_driver_library/BLF/LinSpikeEvent.h \
    ../libs/blf_driver_library/BLF/LinSpikeEvent2.h \
    ../libs/blf_driver_library/BLF/LinStatisticEvent.h \
    ../libs/blf_driver_library/BLF/LinSyncError.h \
    ../libs/blf_driver_library/BLF/LinSyncError2.h \
    ../libs/blf_driver_library/BLF/LinSynchFieldEvent.h \
    ../libs/blf_driver_library/BLF/LinUnexpectedWakeup.h \
    ../libs/blf_driver_library/BLF/LinWakeupEvent.h \
    ../libs/blf_driver_library/BLF/LinWakeupEvent2.h \
    ../libs/blf_driver_library/BLF/LogContainer.h \
    ../libs/blf_driver_library/BLF/Most50Message.h \
    ../libs/blf_driver_library/BLF/Most50Pkt.h \
    ../libs/blf_driver_library/BLF/Most150AllocTab.h \
    ../libs/blf_driver_library/BLF/Most150Message.h \
    ../libs/blf_driver_library/BLF/Most150MessageFragment.h \
    ../libs/blf_driver_library/BLF/Most150Pkt.h \
    ../libs/blf_driver_library/BLF/Most150PktFragment.h \
    ../libs/blf_driver_library/BLF/MostAllocTab.h \
    ../libs/blf_driver_library/BLF/MostCtrl.h \
    ../libs/blf_driver_library/BLF/MostDataLost.h \
    ../libs/blf_driver_library/BLF/MostEcl.h \
    ../libs/blf_driver_library/BLF/MostEthernetPkt.h \
    ../libs/blf_driver_library/BLF/MostEthernetPktFragment.h \
    ../libs/blf_driver_library/BLF/MostGenReg.h \
    ../libs/blf_driver_library/BLF/MostHwMode.h \
    ../libs/blf_driver_library/BLF/MostLightLock.h \
    ../libs/blf_driver_library/BLF/MostNetState.h \
    ../libs/blf_driver_library/BLF/MostPkt.h \
    ../libs/blf_driver_library/BLF/MostPkt2.h \
    ../libs/blf_driver_library/BLF/MostReg.h \
    ../libs/blf_driver_library/BLF/MostSpy.h \
    ../libs/blf_driver_library/BLF/MostStatistic.h \
    ../libs/blf_driver_library/BLF/MostStatisticEx.h \
    ../libs/blf_driver_library/BLF/MostStress.h \
    ../libs/blf_driver_library/BLF/MostSystemEvent.h \
    ../libs/blf_driver_library/BLF/MostTrigger.h \
    ../libs/blf_driver_library/BLF/MostTxLight.h \
    ../libs/blf_driver_library/BLF/ObjectHeader.h \
    ../libs/blf_driver_library/BLF/ObjectHeader2.h \
    ../libs/blf_driver_library/BLF/ObjectHeaderBase.h \
    ../libs/blf_driver_library/BLF/platform.h \
    ../libs/blf_driver_library/BLF/RealtimeClock.h \
    ../libs/blf_driver_library/BLF/SerialEvent.h \
    ../libs/blf_driver_library/BLF/SystemVariable.h \
    ../libs/blf_driver_library/BLF/UncompressedFile.h \
    ../libs/blf_driver_library/BLF/vector_blf_export.h \
    ../libs/blf_driver_library/BLF/VectorTypes.h \
    ../libs/blf_driver_library/BLF/WlanFrame.h \
    ../libs/blf_driver_library/BLF/WlanStatistic.h \
    ../src/opensyde_gui_elements/label/C_OgeLabHeadingGroupBold.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosSectionPopup.h \
    ../src/can_monitor/message_generator/signals/C_CamGenSigTableDelegate.h \
    ../src/can_monitor/message_generator/C_CamGenKeySelect.h \
    ../src/can_monitor/can_monitor_gui_elements/spin_box/C_CamOgeSpxProperties.h \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxAllBase.h \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxToolTipBase.h \
    ../src/opensyde_gui_elements/widget/C_OgeWiSpinBoxGroup.h \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxAutoFixBase.h \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxDoubleAutoFix.h \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxInt64.h \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxInt64AutoFix.h \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxDoubleToolTipBase.h \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxInt64ToolTipBase.h \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxDoubleDynamicDecimalsBase.h \
    ../src/can_monitor/can_monitor_gui_elements/widget/C_CamOgeWiSpinBoxGroupTable.h \
    ../src/can_monitor/can_monitor_gui_elements/combo_box/C_CamOgeCbxDark.h \
    ../src/can_monitor/can_monitor_gui_elements/push_button/C_CamOgePubDarkBrowse.h \
    ../src/can_monitor/project/C_CamProLoggingData.h \
    ../src/can_monitor/data_base/C_CamDbDbc.h \
    ../src/can_monitor/data_base/C_CamDbDbcMessageId.h \
    ../src/can_monitor/data_base/C_CamDbHandler.h \
    ../src/can_monitor/data_base/C_CamDbOsy.h \
    ../src/can_monitor/message_generator/signals/C_CamGenSigUtil.h \
    ../src/can_monitor/can_monitor_gui_elements/menu/C_CamOgeMuRecentProjects.h \
    ../src/opensyde_gui_elements/line_edit/C_OgeLeProperties.h \
    ../src/table_base/tree_base/C_TblTreDataElementSortFilter.h \
    ../src/opensyde_gui_elements/C_OgeTreeViewToolTipBase.h \
    ../src/system_definition/C_SdTooltipUtil.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseSelectionModel.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseSelectionPopup.h \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseSelectionView.h \
    ../src/can_monitor/can_monitor_gui_elements/combo_box/C_CamOgeCbxTableSmall.h \
    ../src/can_monitor/can_monitor_gui_elements/tool_button/C_CamOgeTobRecentProjects.h \
    ../src/opensyde_gui_elements/tool_button/C_OgeTobToolTipBase.h \
    ../src/can_monitor/util/C_CamUti.h \
    ../src/can_monitor/message_generator/C_CamGenTableDelegate.h \
    ../src/table_base/C_TblDelegateUtil.h \
    ../src/opensyde_gui_elements/C_OgeTreeToolTipBase.h \
    ../src/can_monitor/can_monitor_gui_elements/push_button/C_CamOgePubPathVariables.h \
    ../src/opensyde_gui_elements/line_edit/C_OgeLeFilePathBase.h \
    ../src/can_monitor/can_monitor_gui_elements/line_edit/C_CamOgeLeFilePath.h \
    ../src/opensyde_gui_elements/menu/C_OgeMuSections.h \
    ../src/table_base/tree_base/C_TblTreSimpleItem.h \
    ../src/table_base/tree_base/C_TblTreSimpleModel.h \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxTableBase.h \
    ../src/can_monitor/message_trace/C_CamMetTreeLoggerDataGreyOutInformation.h \
    ../src/opensyde_gui_elements/push_button/C_OgePubMessageCancel.h \
    ../src/opensyde_gui_elements/push_button/C_OgePubMessageOk.h \
    ../src/can_monitor/table_base/C_CamTblDelegate.h \
    ../src/can_monitor/message_trace/C_CamMetUtil.h \
    ../src/can_monitor/message_trace/C_CamMetSettingsPopup.h \
    ../src/opensyde_gui_elements/combo_box/C_OgeCbxMultiSelect.h \
    ../src/opensyde_gui_elements/label/C_OgeLabElided.h \
    ../src/opensyde_gui_elements/spin_box/C_OgeSpxDoubleAutoFixCustomTrigger.h \
    ../src/can_monitor/data_base/C_CamDbOsyMessageId.h \
    ../src/can_monitor/data_base/C_CamDbOsyListId.h \
    ../src/can_monitor/data_base/C_CamDbDbcUnmappedMessageId.h

FORMS    += \
    ../src/opensyde_gui_elements/widget/C_OgeWiCustomMessage.ui \
    ../src/opensyde_gui_elements/widget/C_OgeWiError.ui \
    ../src/opensyde_gui_elements/C_OgePopUpDialog.ui \
    ../src/navigable_gui/C_NagToolTip.ui \
    ../src/can_monitor/C_CamMainWindow.ui \
    ../src/navigable_gui/C_NagAboutDialog.ui \
    ../src/can_monitor/message_trace/C_CamMetWidget.ui \
    ../src/can_monitor/message_trace/C_CamMetStatusBarWidget.ui \
    ../src/can_monitor/message_trace/C_CamMetControlBarWidget.ui \
    ../src/can_monitor/message_generator/C_CamGenWidget.ui \
    ../src/can_monitor/can_monitor_settings/C_CamMosWidget.ui \
    ../src/can_monitor/can_monitor_settings/C_CamMosDllWidget.ui \
    ../src/can_monitor/can_monitor_settings/C_CamMosFilterWidget.ui \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseWidget.ui \
    ../src/can_monitor/can_monitor_settings/C_CamMosLoggingWidget.ui \
    ../src/can_monitor/C_CamTitleBarWidget.ui \
    ../src/can_monitor/can_monitor_gui_elements/widget/C_CamOgeWiSectionHeader.ui \
    ../src/can_monitor/can_monitor_gui_elements/widget/C_CamOgeWiSettingSubSection.ui \
    ../src/can_monitor/message_generator/C_CamGenMessagesWidget.ui \
    ../src/can_monitor/can_monitor_settings/C_CamMosFilterItemWidget.ui \
    ../src/can_monitor/can_monitor_settings/C_CamMosFilterPopup.ui \
    ../src/can_monitor/message_generator/signals/C_CamGenSigWidget.ui \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseItemWidget.ui \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseBusSelectionPopup.ui \
    ../src/can_monitor/message_generator/C_CamGenKeySelect.ui \
    ../src/opensyde_gui_elements/widget/C_OgeWiSpinBoxGroup.ui \
    ../src/can_monitor/can_monitor_settings/C_CamMosDatabaseSelectionPopup.ui \
    ../src/can_monitor/message_trace/C_CamMetSettingsPopup.ui

INCLUDEPATH += ../src \
               ../src/can_monitor \
               ../src/can_monitor/data_base \
               ../src/can_monitor/project \
               ../src/can_monitor/user_settings \
               ../src/can_monitor/message_trace \
               ../src/can_monitor/can_monitor_settings \
               ../src/can_monitor/message_generator \
               ../src/can_monitor/message_generator/signals \
               ../src/can_monitor/can_monitor_gui_elements \
               ../src/can_monitor/can_monitor_gui_elements/check_box \
               ../src/can_monitor/can_monitor_gui_elements/combo_box \
               ../src/can_monitor/can_monitor_gui_elements/group_box \
               ../src/can_monitor/can_monitor_gui_elements/push_button \
               ../src/can_monitor/can_monitor_gui_elements/label \
               ../src/can_monitor/can_monitor_gui_elements/line_edit \
               ../src/can_monitor/can_monitor_gui_elements/menu \
               ../src/can_monitor/can_monitor_gui_elements/splitter \
               ../src/can_monitor/can_monitor_gui_elements/spin_box \
               ../src/can_monitor/can_monitor_gui_elements/tool_button \
               ../src/can_monitor/can_monitor_gui_elements/widget \
               ../src/can_monitor/table_base \
               ../src/can_monitor/util \
               ../src/help_engine \
               ../src/opensyde_gui_elements \
               ../src/opensyde_gui_elements/check_box \
               ../src/opensyde_gui_elements/combo_box \
               ../src/opensyde_gui_elements/frame \
               ../src/opensyde_gui_elements/group_box \
               ../src/opensyde_gui_elements/label \
               ../src/opensyde_gui_elements/line_edit \
               ../src/opensyde_gui_elements/menu \
               ../src/opensyde_gui_elements/push_button \
               ../src/opensyde_gui_elements/radio_button \
               ../src/opensyde_gui_elements/splitter \
               ../src/opensyde_gui_elements/spin_box \
               ../src/opensyde_gui_elements/scroll_area \
               ../src/opensyde_gui_elements/text_browser \
               ../src/opensyde_gui_elements/text_edit \
               ../src/opensyde_gui_elements/tool_button \
               ../src/opensyde_gui_elements/widget \
               ../src/navigable_gui \
               ../src/com_import_export \
               ../src/table_base \
               ../src/table_base/tree_base \
               ../src/system_definition \
               ../src/system_definition/node_edit/datapools \
               ../src/system_views/communication \
               ../src/project_operations/ \
               ../src/gettext \
               ../src/util \
               ../src/precompiled_headers/can_monitor \
               ../libs/gettext \
               ../libs/dbc_driver_library/src \
               ../libs/dbc_driver_library/src/Vector \
               ../libs/blf_driver_library \
               ../libs/openssl/include

#do not issue deprecation warnings (tested code can contain deprecated functions which we do want to provide)
win32-g++ {
QMAKE_CXXFLAGS += -Wno-deprecated-declarations
}

LIBS += -L../libs/gettext -lintl \
        -L../libs/blf_driver_library -lvector_blf \
        -lz

LIBS += -lws2_32   #WinSock
LIBS += -lIphlpapi #IP helper API

#openssl
LIBS += -L../libs/openssl -lcrypto

#add windows API libraries
LIBS += -lversion

QMAKE_TARGET_COMPANY = STW
QMAKE_TARGET_PRODUCT = openSYDE CAN Monitor
#QMAKE_TARGET_DESCRIPTION = TODO
#See C coding rules v4.00r0
QMAKE_TARGET_COPYRIGHT = STW (c) 2018

DISTFILES +=

RESOURCES += \
    ../src/can_monitor/cam_application.qrc

#compiler with function intrumentation (for "Embedded Profiler")
#QMAKE_CXXFLAGS += -finstrument-functions
#workaround for https://bugreports.qt.io/browse/QTBUG-35884 (for "Embedded Profiler")
#QMAKE_CXXFLAGS += -finstrument-functions-exclude-function-list=load,store
#create map file
#QMAKE_LFLAGS += -Wl,-Map=qt_test.map
#libraries for "Embedded Profiler"
#LIBS += -L$$PWD/eprofiler-windows32-mingw-intel-1.2.3/EProfiler/windows32-mingw-intel/lib/ -lEProfilerStatic
#LIBS += -lpthread
