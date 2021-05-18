// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class DogFightTarget : TargetRules
{
	public DogFightTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("DogFight");

		BuildEnvironment = TargetBuildEnvironment.Unique;
		bCompileChaos = true;
		bUseChaos = true;
	}
}
