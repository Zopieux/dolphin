// Copyright 2014 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "ui_ConfigDialog.h"

#include "Common/Common.h"

#include "DolphinQt/Config/ConfigDialog.h"
#include "DolphinQt/Config/GeneralConfigPage.h"
#include "DolphinQt/Config/InterfaceConfigPage.h"
#include "DolphinQt/Utils/Resources.h"

DConfigDialog::DConfigDialog(QWidget* parent_widget)
	: QDialog(parent_widget)
{
	setWindowModality(Qt::WindowModal);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	m_ui = std::make_unique<Ui::DConfigDialog>();
	m_ui->setupUi(this);

	DGeneralConfigPage* const general_page = new DGeneralConfigPage(this);
	DInterfaceConfigPage* const interface_page = new DInterfaceConfigPage(this);
	m_ui->confWidget->addTab(general_page, tr("General"));
	m_ui->confWidget->addTab(interface_page, tr("Interface"));
}

DConfigDialog::~DConfigDialog()
{
}

void DConfigDialog::SetCurrentTab(DConfigDialog::Page page)
{
	m_ui->confWidget->setCurrentIndex(page);
}
