// Copyright 2014 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <memory>
#include <QVector>
#include <QRadioButton>
#include <QWidget>

#include "DolphinQt/Config/ConfigPage.h"

// Predefinitions
namespace Ui
{
class DInterfaceConfigPage;
}

class DInterfaceConfigPage : public QWidget, public ConfigPage
{
	Q_OBJECT

public:
	explicit DInterfaceConfigPage(QWidget* parent_widget = nullptr);
	~DInterfaceConfigPage();

	virtual void LoadFromConfig() override;
	virtual void Refresh() override ;

private:
	QStringList m_languages;
	std::unique_ptr<Ui::DInterfaceConfigPage> m_ui;
	void LoadThemes();

private slots:
	void OnConfirmStopToggled(bool checked);
	void OnUsePanicHandlersToggled(bool checked);
	void OnOnScreenDisplayMessagesToggled(bool checked);
	void OnPauseOnFocusLostToggled(bool checked);
	void OnThemeIndexChanged(int index);
	void OnLanguageIndexChanged(int index);
};
