#pragma once

#include "CWeapon.h"

class CImage;

struct Item
{
    // ��� �������� ���������� ������ �� ����
    wstring     tag;            // �̹��� �±� ���� (������â�� ǥ���� ������ �̹���)
    ITEM_TYPE   m_eItemType;    // WEAPON, PASSIVE
    int         m_iBasePrice;   // �⺻ ����
    wstring     m_sName;        // �ѱ��� �̸�

    // �������� ���� �� �ִ� �ɷ�ġ�� (weapon �Ǵ� passive)
    union {
        tWeaponInfo m_tWeaponInfo;  // ���� ���� (WEAPON Ÿ���� �� ���)

        struct {
            float   m_fDefaultSpeed;    // �⺻ ���ǵ�
            int     m_iAddMaxHP;         // + �ִ� ü��
            float   m_fDamageCoef;      // ���� ������ %
            float   m_fMeleeCoef;       // �ٰŸ� ���� ������ %
            float   m_fRangeCoef;       // ���Ÿ� ���� ������ %
            float   m_fAttackSpeedCoef; // ���� �ӵ� %
            int     m_iCriticalAcc;     // ũ��Ƽ�� Ȯ�� %
            float   m_fSpeed;           // �ӵ� ���
        } PassiveStats;                 // �нú� ������ ���� ���� (PASSIVE Ÿ���� �� ���)
    };

    // ���� ������
    Item(const Item& other) : tag(other.tag), m_eItemType(other.m_eItemType), m_iBasePrice(other.m_iBasePrice), m_sName(other.m_sName)
    {
        if (m_eItemType == ITEM_TYPE::WEAPON) {
            m_tWeaponInfo = other.m_tWeaponInfo;  // ���� ���� ����
        }
        else {
            PassiveStats = other.PassiveStats;  // �нú� ���� ����
        }
    }

    // ���� ������
    Item& operator=(const Item& other)
    {
        if (this != &other) {
            tag = other.tag;
            m_eItemType = other.m_eItemType;
            m_iBasePrice = other.m_iBasePrice;
            m_sName = other.m_sName;

            if (m_eItemType == ITEM_TYPE::WEAPON) {
                m_tWeaponInfo = other.m_tWeaponInfo;  // ���� ���� ����
            }
            else {
                PassiveStats = other.PassiveStats;  // �нú� ���� ����
            }
        }
        return *this;
    }

    // ������ (�ʱ�ȭ ���Ǹ� ���� �߰�)
    Item(ITEM_TYPE type) : m_eItemType(type) {
        if (type == ITEM_TYPE::WEAPON) {
            memset(&m_tWeaponInfo, 0, sizeof(m_tWeaponInfo)); // ���� ���� �ʱ�ȭ
        }
        else {
            memset(&PassiveStats, 0, sizeof(PassiveStats));  // �нú� ���� �ʱ�ȭ
        }
    }


    // �⺻ ������
    Item() : tag(L""), m_eItemType(ITEM_TYPE::PASSIVE), m_iBasePrice(0), m_sName(L"") {}

    // �Ҹ���
    ~Item() {}
};


class ItemMgr
{
	SINGLE(ItemMgr);

private:
	unordered_map<wstring, Item*>	m_sItems;					//��� ������ ���� ����

	vector<Item*>					m_vWeaponsItems;			//����� ����
	vector<Item*>					m_vPassiveItems;			//�нú�� ����
	Item*							m_basicCharacter;

	bool							m_bBaseCharacterIsAdded;	//ĳ���Ͱ� ���õǸ� �ѹ��� �߰��ϰ�

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

