#pragma once

#include "CWeapon.h"

class CImage;

struct Item
{
    // 모든 아이템이 공통적으로 가져야 할 정보
    wstring     tag;            // 이미지 태그 정보 (아이템창에 표시할 아이콘 이미지)
    ITEM_TYPE   m_eItemType;    // WEAPON, PASSIVE
    int         m_iBasePrice;   // 기본 가격
    wstring     m_sName;        // 한국어 이름

    // 아이템이 가질 수 있는 능력치들 (weapon 또는 passive)
    union {
        tWeaponInfo m_tWeaponInfo;  // 무기 정보 (WEAPON 타입일 때 사용)

        struct {
            float   m_fDefaultSpeed;    // 기본 스피드
            int     m_iAddMaxHP;         // + 최대 체력
            float   m_fDamageCoef;      // 최종 데미지 %
            float   m_fMeleeCoef;       // 근거리 최종 데미지 %
            float   m_fRangeCoef;       // 원거리 최종 데미지 %
            float   m_fAttackSpeedCoef; // 공격 속도 %
            int     m_iCriticalAcc;     // 크리티컬 확률 %
            float   m_fSpeed;           // 속도 계수
        } PassiveStats;                 // 패시브 아이템 관련 정보 (PASSIVE 타입일 때 사용)
    };

    // 복사 생성자
    Item(const Item& other) : tag(other.tag), m_eItemType(other.m_eItemType), m_iBasePrice(other.m_iBasePrice), m_sName(other.m_sName)
    {
        if (m_eItemType == ITEM_TYPE::WEAPON) {
            m_tWeaponInfo = other.m_tWeaponInfo;  // 무기 정보 복사
        }
        else {
            PassiveStats = other.PassiveStats;  // 패시브 정보 복사
        }
    }

    // 대입 연산자
    Item& operator=(const Item& other)
    {
        if (this != &other) {
            tag = other.tag;
            m_eItemType = other.m_eItemType;
            m_iBasePrice = other.m_iBasePrice;
            m_sName = other.m_sName;

            if (m_eItemType == ITEM_TYPE::WEAPON) {
                m_tWeaponInfo = other.m_tWeaponInfo;  // 무기 정보 복사
            }
            else {
                PassiveStats = other.PassiveStats;  // 패시브 정보 복사
            }
        }
        return *this;
    }

    // 생성자 (초기화 편의를 위해 추가)
    Item(ITEM_TYPE type) : m_eItemType(type) {
        if (type == ITEM_TYPE::WEAPON) {
            memset(&m_tWeaponInfo, 0, sizeof(m_tWeaponInfo)); // 무기 정보 초기화
        }
        else {
            memset(&PassiveStats, 0, sizeof(PassiveStats));  // 패시브 정보 초기화
        }
    }


    // 기본 생성자
    Item() : tag(L""), m_eItemType(ITEM_TYPE::PASSIVE), m_iBasePrice(0), m_sName(L"") {}

    // 소멸자
    ~Item() {}
};


class ItemMgr
{
	SINGLE(ItemMgr);

private:
	unordered_map<wstring, Item*>	m_sItems;					//모든 아이템 정보 저장

	vector<Item*>					m_vWeaponsItems;			//무기류 저장
	vector<Item*>					m_vPassiveItems;			//패시브류 저장
	Item*							m_basicCharacter;

	bool							m_bBaseCharacterIsAdded;	//캐릭터가 세팅되면 한번만 추가하게

public:
	vector<Item*>& GetWeaponItems() { return m_vWeaponsItems; }
	const vector<Item*>& GetPassiveItems() { return m_vPassiveItems; }
	size_t GetPassiveItemssize() { return m_vPassiveItems.size(); }
	Item* GetBasicCharacter() { return m_basicCharacter; }

	Item* GetItem(const wstring& _tag) { return m_sItems[_tag]; }

public:
	void update();

public:
	void AddWeapons(Item* _weapon) { m_vWeaponsItems.push_back(_weapon); }
	void AddPassive(Item* _passive) { m_vPassiveItems.push_back(_passive); }
	void SetBasicCharacter(Item* _basicCharacter) { m_basicCharacter = _basicCharacter; }

public:
	void Clear();

public:
	void init();
    void init_character();
	void init_passive();
	void init_weapon();

    friend class CScene_Select_Weapon;
};

