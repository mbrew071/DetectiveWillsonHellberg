// All rights reserved, HelloIT Norweskie

using UnrealBuildTool;
using System.Collections.Generic;

public class GameProjectTarget : TargetRules
{
	public GameProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "GameProject" } );
	}
}
