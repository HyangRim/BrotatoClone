#pragma once
#include "pch.h"

const wstring upgrade_tag = L"upgrade_icon";
const vector<wstring> upgrade_tag_list = {
	L"upgrade_attack_speed",
	L"upgrade_crit_chance",
	L"upgrade_health",
	L"upgrade_melee_dmg",
	L"upgrade_percent_dmg",
	L"upgrade_ranged_dmg",
	L"upgrade_speed",
};


const vector<wstring> upgrade_name_list = {
	L"반사신경",
	L"손가락",
	L"심장",
	L"팔뚝",
	L"삼두근",
	L"어깨",
	L"다리",
};

constexpr const wchar_t upgrade_classifi[] = L"업그레이드";

const vector<wstring> upgrade_text_list = {
	L"+5 %% 공격속도",
	L"+3 %% 치명타율",
	L"+3 최대 HP",
	L"+2 근거리 데미지",
	L"+5 %% 데미지",
	L"+1 원거리 데미지",
	L"+3 % 속도",
};