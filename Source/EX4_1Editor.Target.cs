using UnrealBuildTool;
public class EX4_1EditorTarget : TargetRules
{
    public EX4_1EditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
        ExtraModuleNames.Add("EX4_1");
    }
}

