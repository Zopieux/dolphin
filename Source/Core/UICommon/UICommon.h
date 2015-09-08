// Copyright 2014 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

namespace UICommon
{
extern const char* URL_DOWNLOAD,* URL_DOWNLOAD_TEXT,* URL_LICENSE,* URL_AUTHORS,* URL_SUPPORT;

void Init();
void Shutdown();

void CreateDirectories();
void SetUserDirectory(const std::string& custom_path);

} // namespace UICommon
