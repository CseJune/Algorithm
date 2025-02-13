// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SP : ModuleRules
{
	public SP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
		// 이 프로젝트에서 필수적으로 사용하는 엔진 기능들
		// Core -> 엔진의 기본 기능, CoreUObject -> 리플렉션 시스템 등, Engine -> 게임 엔진의 주요 기능, InputCore -> 입력 시스템
		// EnhancedInput -> 향상된 인풋 시스템, UMG -> UI와 관련된 모듈

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
