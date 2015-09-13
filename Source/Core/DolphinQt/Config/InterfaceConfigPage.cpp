// Copyright 2014 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#include <QtCore/QLocale>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QMessageBox>

#include "ui_InterfaceConfigPage.h"

#include "Common/CommonPaths.h"
#include "Common/FileSearch.h"
#include "Common/FileUtil.h"
#include "Core/ConfigManager.h"

#include "DolphinQt/Config/InterfaceConfigPage.h"

#if defined(HAVE_XRANDR) && HAVE_XRANDR
#include "DolphinWX/X11Utils.h"
#endif

static const QLocale::Language language_ids[] =
{
	QLocale::Language::AnyLanguage,

	QLocale::Language::Catalan,
	QLocale::Language::Czech,
	QLocale::Language::German,
	QLocale::Language::English,
	QLocale::Language::Spanish,
	QLocale::Language::French,
	QLocale::Language::Italian,
	QLocale::Language::Hungarian,
	QLocale::Language::Dutch,
	QLocale::Language::NorwegianBokmal,
	QLocale::Language::Polish,
	QLocale::Language::Portuguese,
	QLocale::Language::Portuguese, // no Portuguese Brazilian in Qt
	QLocale::Language::Serbian,
	QLocale::Language::Swedish,
	QLocale::Language::Turkish,

	QLocale::Language::Greek,
	QLocale::Language::Russian,
	QLocale::Language::Hebrew,
	QLocale::Language::Arabic,
	QLocale::Language::Persian, // ie. Farsi
	QLocale::Language::Korean,
	QLocale::Language::Japanese,
	QLocale::Language::Chinese, // no simplified/traditional chinese in Qt
	QLocale::Language::Chinese, // no simplified/traditional chinese in Qt
};

DInterfaceConfigPage::DInterfaceConfigPage(QWidget* parent_widget)
	: QWidget(parent_widget)
{
	m_ui = std::make_unique<Ui::DInterfaceConfigPage>();
	m_ui->setupUi(this);

	// Populate the language combo
	// Keep these in sync with language_ids array above
	m_languages
		<< tr("<System Language> (%1)").arg(QLocale::system().nativeLanguageName())
		<< tr("Catal\u00e0")                                      // Catalan
		<< tr("\u010ce\u0161tina")                                // Czech
		<< tr("Deutsch")                                          // German
		<< tr("English")                                          // English
		<< tr("Espa\u00f1ol")                                     // Spanish
		<< tr("Fran\u00e7ais")                                    // French
		<< tr("Italiano")                                         // Italian
		<< tr("Magyar")                                           // Hungarian
		<< tr("Nederlands")                                       // Dutch
		<< tr("Norsk bokm\u00e5l")                                // Norwegian
		<< tr("Polski")                                           // Polish
		<< tr("Portugu\u00eas")                                   // Portuguese
		<< tr("Portugu\u00eas (Brasil)")                          // Portuguese (Brazil)
		<< tr("Srpski")                                           // Serbian
		<< tr("Svenska")                                          // Swedish
		<< tr("T\u00fcrk\u00e7e")                                 // Turkish
		<< tr("\u0395\u03bb\u03bb\u03b7\u03bd\u03b9\u03ba\u03ac") // Greek
		<< tr("\u0420\u0443\u0441\u0441\u043a\u0438\u0439")       // Russian
		<< tr("\u05e2\u05d1\u05e8\u05d9\u05ea")                   // Hebrew
		<< tr("\u0627\u0644\u0639\u0631\u0628\u064a\u0629")       // Arabic
		<< tr("\u0641\u0627\u0631\u0633\u06cc")                   // Farsi
		<< tr("\ud55c\uad6d\uc5b4")                               // Korean
		<< tr("\u65e5\u672c\u8a9e")                               // Japanese
		<< tr("\u7b80\u4f53\u4e2d\u6587")                         // Simplified Chinese
		<< tr("\u7e41\u9ad4\u4e2d\u6587")                         // Traditional Chinese
	;

	m_ui->comboLanguage->addItems(m_languages);

	LoadFromConfig();

	// Connect signals
	connect(m_ui->chkConfirmStop, &QCheckBox::toggled, [](bool checked) {
		SConfig::GetInstance().bConfirmStop = checked;
	});
	connect(m_ui->chkUsePanicHandlers, &QCheckBox::toggled, [](bool checked) {
		SConfig::GetInstance().bUsePanicHandlers = checked;
		SetEnableAlert(checked);
	});
	connect(m_ui->chkOnScreenDisplayMessages, &QCheckBox::toggled, [](bool checked) {
		SConfig::GetInstance().bOnScreenDisplayMessages = checked;
	});
	connect(m_ui->chkPauseOnFocusLost, &QCheckBox::toggled, [](bool checked) {
		SConfig::GetInstance().m_PauseOnFocusLost = checked;
	});
	connect(m_ui->comboTheme, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index) {
		SConfig::GetInstance().theme_name = m_ui->comboTheme->currentText().toStdString();
		// TODO: update theme icons
		// TODO: update game list
	});
	connect(m_ui->comboLanguage, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [this](int index) {
		if (SConfig::GetInstance().m_QtInterfaceLanguage != language_ids[index])
			QMessageBox::information(this, tr("Language changed"),
				tr("You must restart Dolphin in order for the change to take effect."));
		SConfig::GetInstance().m_QtInterfaceLanguage = language_ids[index];
	});
}

DInterfaceConfigPage::~DInterfaceConfigPage()
{
	m_languages.clear();
}

void DInterfaceConfigPage::LoadFromConfig()
{
	const SConfig& startup_params = SConfig::GetInstance();

	m_ui->chkConfirmStop->setChecked(startup_params.bConfirmStop);
	m_ui->chkUsePanicHandlers->setChecked(startup_params.bUsePanicHandlers);
	m_ui->chkOnScreenDisplayMessages->setChecked(startup_params.bOnScreenDisplayMessages);
	m_ui->chkPauseOnFocusLost->setChecked(SConfig::GetInstance().m_PauseOnFocusLost);

	for (size_t i = 0; i < m_languages.size(); ++i)
	{
		if (language_ids[i] == SConfig::GetInstance().m_QtInterfaceLanguage)
		{
			m_ui->comboLanguage->setCurrentIndex(i);
			break;
		}
	}

	LoadThemes();
}

void DInterfaceConfigPage::Refresh()
{
}

void DInterfaceConfigPage::LoadThemes()
{
	auto sv = DoFileSearch({"*"}, {
		File::GetUserPath(D_THEMES_IDX),
		File::GetSysDirectory() + THEMES_DIR
	}, /* recursive */ false);
	for (const std::string& filename : sv)
	{
		std::string name, ext;
		SplitPath(filename, nullptr, &name, &ext);

		name += ext;
		const QString qt_name = QString::fromStdString(name);
		if (-1 == m_ui->comboTheme->findText(qt_name))
			m_ui->comboTheme->addItem(qt_name);
	}
	m_ui->comboTheme->setCurrentText(QString::fromStdString(SConfig::GetInstance().theme_name));
}
