// Copyright 2014 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include "ui_GeneralConfigPage.h"

#include "Common/Common.h"
#include "Common/StdMakeUnique.h"
#include "Core/ConfigManager.h"
#include "Core/Core.h"
#include "Core/PowerPC/PowerPC.h"

#include "DolphinQt/Config/GeneralConfigPage.h"

DGeneralConfigPage::DGeneralConfigPage(QWidget* parent_widget)
	: QWidget(parent_widget)
{
	m_ui = std::make_unique<Ui::DGeneralConfigPage>();
	m_ui->setupUi(this);

	m_cpu_cores = {
		{ PowerPC::CORE_INTERPRETER, tr("Interpreter (slowest)") },
		{ PowerPC::CORE_CACHEDINTERPRETER, tr("Cached Interpreter (slower)") },
#ifdef _M_X86_64
		{ PowerPC::CORE_JIT64, tr("JIT Recompiler (recommended)") },
		{ PowerPC::CORE_JITIL64, tr("JITIL Recompiler (slow, experimental)") },
#elif defined(_M_ARM_64)
		{ PowerPC::CORE_JITARM64, tr("JIT Arm64 (experimental)") },
#endif
	};

	// Populate CPU core radio buttons
	m_cpu_cores_radios.reserve(4);
	for (const CPUCore& cpu_core : m_cpu_cores)
	{
		QRadioButton* check_box = new QRadioButton(cpu_core.name, m_ui->groupCpuEmulatorEngine);
		connect(check_box, SIGNAL(toggled(bool)), this, SLOT(OnCPUEngineRadioToggled(bool)));
		m_ui->layoutCpuEmulatorEngine->addWidget(check_box);
		m_cpu_cores_radios.append(check_box);
	}

	// Populate the frame limit combo
	m_ui->comboFrameLimit->addItem(tr("Off"));
	m_ui->comboFrameLimit->addItem(tr("Auto"));
	for (int i = 5; i <= 120; i += 5) // from 5 to 120
		m_ui->comboFrameLimit->addItem(QString::number(i));

	// Connect signals
	connect(m_ui->chkEnableDualCore, SIGNAL(toggled(bool)),
		this, SLOT(OnEnableDualCoreCheckBoxToggled(bool)));
	connect(m_ui->chkEnableSkipIdle, SIGNAL(toggled(bool)),
		this, SLOT(OnEnableSkipIdleCheckBoxToggled(bool)));
	connect(m_ui->chkEnableCheats, SIGNAL(toggled(bool)),
		this, SLOT(OnEnableCheatsCheckBoxToggled(bool)));
	connect(m_ui->chkForceNTSCJ, SIGNAL(toggled(bool)),
		this, SLOT(OnForceNTSCJCheckBoxToggled(bool)));
	connect(m_ui->comboFrameLimit, SIGNAL(currentIndexChanged(int)),
		this, SLOT(OnFrameLimitIndexChanged(int)));

	LoadFromConfig();
	Refresh();
}

DGeneralConfigPage::~DGeneralConfigPage()
{
	m_cpu_cores_radios.clear();
}

void DGeneralConfigPage::LoadFromConfig()
{
	const SConfig& startup_params = SConfig::GetInstance();

	m_ui->chkEnableDualCore->setChecked(startup_params.bCPUThread);
	m_ui->chkEnableSkipIdle->setChecked(startup_params.bSkipIdle);
	m_ui->chkEnableCheats->setChecked(startup_params.bEnableCheats);
	m_ui->chkForceNTSCJ->setChecked(startup_params.bForceNTSCJ);

	m_ui->comboFrameLimit->setCurrentIndex(SConfig::GetInstance().m_Framelimit);

	for (size_t i = 0; i < m_cpu_cores.size(); ++i)
	{
		if (m_cpu_cores[i].CPUid == startup_params.iCPUCore)
			m_cpu_cores_radios.at(i)->setChecked(true);
	}
}

void DGeneralConfigPage::Refresh()
{
	if (Core::IsRunning())
	{
		m_ui->chkEnableDualCore->setEnabled(false);
		m_ui->chkEnableSkipIdle->setEnabled(false);
		m_ui->chkEnableCheats->setEnabled(false);
		m_ui->chkForceNTSCJ->setEnabled(false);
		m_ui->groupCpuEmulatorEngine->setEnabled(false);
	}
}

void DGeneralConfigPage::OnEnableDualCoreCheckBoxToggled(bool checked)
{
	if (Core::IsRunning())
		return;

	SConfig::GetInstance().bCPUThread = checked;
}

void DGeneralConfigPage::OnEnableSkipIdleCheckBoxToggled(bool checked)
{
	SConfig::GetInstance().bSkipIdle = checked;
}

void DGeneralConfigPage::OnEnableCheatsCheckBoxToggled(bool checked)
{
	SConfig::GetInstance().bEnableCheats = checked;
}

void DGeneralConfigPage::OnForceNTSCJCheckBoxToggled(bool checked)
{
	SConfig::GetInstance().bForceNTSCJ = checked;
}

void DGeneralConfigPage::OnFrameLimitIndexChanged(int index)
{
	if (index < 0)
		return;
	unsigned int u_index = index;
	SConfig::GetInstance().m_Framelimit = u_index;
}

void DGeneralConfigPage::OnCPUEngineRadioToggled(bool checked)
{
	if (!checked)
		return;

	// TODO
	/*
	if (main_frame->g_pCodeWindow)
	{
		bool using_interp = (SConfig::GetInstance().iCPUCore == PowerPC::CORE_INTERPRETER);
		main_frame->g_pCodeWindow->GetMenuBar()->Check(IDM_INTERPRETER, using_interp);
	}
	*/

	const int index = m_cpu_cores_radios.indexOf((QRadioButton*) sender());
	SConfig::GetInstance().iCPUCore = m_cpu_cores[index].CPUid;
}
