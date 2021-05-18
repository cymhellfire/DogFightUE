// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DogFightEditorTarget : TargetRules
{
	public DogFightEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		BuildEnvironment = TargetBuildEnvironment.Unique;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("DogFight");

		bCompileChaos = true;
		bUseChaos = true;
	}
}
