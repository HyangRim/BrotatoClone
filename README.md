# Brotato Clone

Brotato를 Win32 API와 Direct2D로 구현한 로그라이크 슈팅 게임 프로젝트입니다.

## 📋 프로젝트 소개

**개발 환경**: Win32 API + Direct2D  
**개발 언어**: C++  
**게임 장르**: 로그라이크 슈팅 게임  
**아키텍처**: 컴포넌트 기반 오브젝트 시스템

Brotato는 웨이브 기반 생존 슈팅 게임으로, 플레이어가 무기를 수집하고 업그레이드하면서 몬스터 웨이브를 버티는 게임입니다.

## 🎮 주요 기능

#### 전투 시스템
- **웨이브 기반 전투**: 시간 기반 몬스터 스폰 시스템
- **다양한 무기**: 수집 가능한 여러 종류의 무기
- **자동 조준**: 가장 가까운 적 자동 타겟팅
- **밀어내기 시스템**: 폭발이나 특수 공격으로 적을 밀어내는 물리 효과

#### 씬 구조
- **Scene_Start**: 전투가 직접 일어나는 메인 게임 씬
  - 일시정지 패널 (m_pPausePanel)
  - 게임 오버 패널 (m_pFailPanel)

#### UI 시스템
- **Panel UI 구조**: 일시정지, 실패 화면 등
- **동적 UI 관리**: 상황별 UI 표시 및 숨김

## 🛠️ 기술 스택

### Framework & API
![Windows](https://img.shields.io/badge/Windows-0078D6?style=flat-square&logo=windows&logoColor=white)

### Development
![C++](https://img.shields.io/badge/C++-00599C?style=flat-square&logo=cplusplus&logoColor=white)
![Win32 API](https://img.shields.io/badge/Win32%20API-0078D6?style=flat-square&logo=windows&logoColor=white)
![Direct2D](https://img.shields.io/badge/Direct2D-512BD4?style=flat-square&logo=windows&logoColor=white)

### 핵심 설계 패턴
- **Component Pattern**: 오브젝트 기반 컴포넌트 시스템
- **Scene Management**: 씬 전환 및 관리 시스템

### 핵심 시스템 구현
- **웨이브 시스템**: 시간 기반 적 생성 및 난이도 증가
- **물리 시스템**: Force 기반 밀어내기 효과
- **충돌 처리**: 플레이어, 적, 투사체 간 충돌 검출
- **자동 조준**: 거리 계산 기반 타겟팅

## 📂 프로젝트 구조
```
Brotato/
├── Engine/ # 핵심 게임 엔진
│ ├── 01. Header/ # 헤더파일
│ ├── 02. Core/ # 코어 시스템
│ ├── 03. Manager/ # 싱글톤 매니저들
│ ├── 04. Object/ # 오브젝트 (플레이어, 적, 무기)
│ ├── 05. Scene/ # 씬 관리 (CScene_Start 등)
│ ├── 06. Resource/ # 리소스 처리
│ ├── 07. Component/ # 컴포넌트 시스템
│ ├── 08. Module/ # 몬스터 AI
│ └── 09. Factory/ # 몬스터 팩토리
├── resources/ # 게임 리소스
│ ├── texture/ # 스프라이트
│ ├── sound/ # 사운드 파일
│ └── tile/ # 배경 타일
└── README.md
```


## 💡 주요 구현 특징

### 웨이브 기반 전투
- 시간에 따라 자동으로 적이 스폰
- 웨이브가 진행될수록 난이도 증가
- 웨이브 클리어 시 상점에서 업그레이드 가능

### UI 관리 시스템
- 일시정지 시 게임 오브젝트 보관 및 복원
- 패널 UI를 통한 게임 상태 관리
- 동적 UI 표시 및 숨김 처리

### Direct2D 비트맵 렌더링
- ID2D1Bitmap을 활용한 효율적인 그래픽 렌더링
- 하드웨어 가속을 통한 부드러운 화면 출력

## 🎯 게임 플레이

### 게임 흐름
1. **전투 시작**: Scene_Start에서 웨이브 시작
2. **적 처치**: 자동으로 가장 가까운 적을 공격
3. **아이템 수집**: 무기 및 업그레이드 아이템 획득
4. **웨이브 클리어**: 모든 적 처치 시 다음 웨이브로 진행
5. **업그레이드**: 상점에서 능력치 강화

### 조작 방법
- **이동**: 방향키 또는 WASD
- **공격**: 자동 공격
- **일시정지**: ESC 키


## 👤 개발자

**vfly1189**
- GitHub: [@vfly1189](https://github.com/vfly1189)
- Blog: [tobrother.tistory.com](https://tobrother.tistory.com/)



## 📄 라이선스

본 프로젝트는 교육 목적으로 제작되었으며, Brotato의 저작권은 원작자에게 있습니다.


