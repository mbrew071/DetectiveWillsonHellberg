// All rights reserved, HelloIT Norweskie

using UnrealBuildTool;
using System.Collections.Generic;

public class GameProjectEditorTarget : TargetRules
{
	public GameProjectEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V4;

		ExtraModuleNames.AddRange( new string[] { "GameProject" } );
	}
}
