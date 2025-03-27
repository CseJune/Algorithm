숫자 야구 채팅 게임 (Unreal Engine 5)

ㅡㅡㅡ

게임 소개

1~9 사이의 서로 다른 숫자 3개로 구성된 비밀 숫자를 맞추는 게임입니다.

채팅창을 통해 숫자를 입력하면, 서버가 결과(1S2B, OUT 등)를 판정하여 전 플레이어에게 공유합니다.

제한된 횟수(기본: 3회) 안에 정답을 맞히거나, 모두 실패하면 무승부로 처리됩니다.

ㅡㅡㅡ

주요 기능 요약

1. 채팅 입력 시스템 (UMG / 입력 위젯)

UCXChatInput 클래스 기반으로 채팅 입력 박스를 생성

EditableTextBox_ChatInput에 엔터 입력 시 OnChatInputTextCommitted() 함수 호출

입력된 텍스트를 ACXPlayerController로 전달하여 서버 RPC로 전송

2. 게임 로직 (GameMode)

ACXGameModeBase에서 정답 생성, 입력 처리, 결과 판정 등 핵심 게임 로직 처리

턴 관리 및 타이머 기반 자동 턴 전환 기능 구현 (15초마다 턴 변경)

IsGuessNumberString()로 숫자 유효성 검사

JudgeResult()로 입력과 정답 비교 후 결과 반환 (nS nB / OUT)

3. 상태 동기화 (PlayerState)

ACXPlayerState에서 플레이어 이름, 시도 횟수, 최대 시도 횟수 관리

모든 정보는 Replicated 처리되어 클라이언트 간 동기화

4. 메시지 전파 (RPC)

클라이언트 입력 → 서버 수신 → 게임 로직 처리 → 모든 클라이언트로 결과 전파

ServerRPCPrintChatMessageString() / ClientRPCPrintChatMessageString() 사용

ㅡㅡㅡ

구현 구조

├── Chat.uproject
├── Source/
│   ├── Chat/
│   │   ├── CXGameModeBase (게임 로직)
│   │   ├── CXGameStateBase (채팅 브로드캐스트)
│   │   ├── CXPlayerController (채팅 입력 전달 / 출력)
│   │   ├── CXPlayerState (이름, 시도 횟수 관리)
│   │   ├── CXChatInput (채팅 UI 위젯)

ㅡㅡㅡ

과제 의도 및 학습 포인트

언리얼의 서버-클라이언트 통신 구조 경험

UMG UI와 입력 처리, 이벤트 바인딩 이해

게임 상태 및 플레이어 상태 관리를 통한 복제 학습

실시간 멀티플레이 채팅 시스템 구현 경험

