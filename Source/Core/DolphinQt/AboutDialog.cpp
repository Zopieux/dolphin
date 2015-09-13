// Copyright 2014 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <memory>
#include <QDesktopServices>
#include <QUrl>

#include "ui_AboutDialog.h"

#include "Common/Common.h"
#include "UICommon/UICommon.h"

#include "DolphinQt/AboutDialog.h"
#include "DolphinQt/Utils/Resources.h"
#include "DolphinQt/Utils/Utils.h"

DAboutDialog::DAboutDialog(QWidget* parent_widget)
	: QDialog(parent_widget)
{
	setWindowModality(Qt::WindowModal);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

	m_ui = std::make_unique<Ui::DAboutDialog>();
	m_ui->setupUi(this);
	m_ui->lblGitRev->setText(SC(scm_desc_str));
	m_ui->lblGitInfo->setText(m_ui->lblGitInfo->text().arg(SC(scm_branch_str), SC(scm_rev_git_str),
		SL(__DATE__), SL(__TIME__)));
	m_ui->lblFinePrint->setText(m_ui->lblFinePrint->text().arg(SL("2015")));
	m_ui->lblChkForUpdates->setText(m_ui->lblChkForUpdates->text().arg(
		SC(UICommon::URL_DOWNLOAD), SC(UICommon::URL_DOWNLOAD_TEXT)));
	m_ui->lblLicenseAuthorsSupport->setText(m_ui->lblLicenseAuthorsSupport->text().arg(
		SC(UICommon::URL_LICENSE), SC(UICommon::URL_AUTHORS), SC(UICommon::URL_SUPPORT)));
	m_ui->lblLogo->setPixmap(Resources::GetPixmap(Resources::DOLPHIN_LOGO_LARGE));
}

DAboutDialog::~DAboutDialog()
{
}
