using UnrealBuildTool;
using System.Collections.Generic;
public class EX4_1Target : TargetRules
{
    public EX4_1Target(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
        ExtraModuleNames.Add("EX4_1");
    }
}
