// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SP : ModuleRules
{
	public SP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG" });
		// �� ������Ʈ���� �ʼ������� ����ϴ� ���� ��ɵ�
		// Core -> ������ �⺻ ���, CoreUObject -> ���÷��� �ý��� ��, Engine -> ���� ������ �ֿ� ���, InputCore -> �Է� �ý���
		// EnhancedInput -> ���� ��ǲ �ý���, UMG -> UI�� ���õ� ���

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
