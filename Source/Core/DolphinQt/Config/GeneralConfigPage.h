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
class DGeneralConfigPage;
}

class DGeneralConfigPage : public QWidget, public ConfigPage
{
	Q_OBJECT

public:
	explicit DGeneralConfigPage(QWidget* parent_widget = nullptr);
	~DGeneralConfigPage();

	virtual void LoadFromConfig() override;
	virtual void Refresh() override ;

private:
	struct CPUCore
	{
		int CPUid;
		QString name;
	};
	std::vector<CPUCore> m_cpu_cores;
	QVector<QRadioButton*> m_cpu_cores_radios;
	std::unique_ptr<Ui::DGeneralConfigPage> m_ui;

private slots:
	void OnEnableDualCoreCheckBoxToggled(bool checked);
	void OnEnableSkipIdleCheckBoxToggled(bool checked);
	void OnEnableCheatsCheckBoxToggled(bool checked);
	void OnForceNTSCJCheckBoxToggled(bool checked);
	void OnFrameLimitIndexChanged(int index);
	void OnCPUEngineRadioToggled(bool checked);
};
