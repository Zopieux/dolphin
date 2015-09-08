// Copyright 2014 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <memory>
#include <QDialog>

// Predefinitions
namespace Ui
{
class DConfigDialog;
}

class DConfigDialog : public QDialog
{
	Q_OBJECT

public:
	explicit DConfigDialog(QWidget* parent_widget = nullptr);
	~DConfigDialog();

	enum Page
	{
		ID_GENERALPAGE = 0,
		ID_DISPLAYPAGE,
		ID_AUDIOPAGE,
		ID_GAMECUBEPAGE,
		ID_WIIPAGE,
		ID_PATHSPAGE,
		ID_ADVANCEDPAGE,
	};

	void SetCurrentTab(Page page);

private:
	std::unique_ptr<Ui::DConfigDialog> m_ui;
};
