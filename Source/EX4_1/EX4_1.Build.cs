using UnrealBuildTool;
public class EX4_1 : ModuleRules
{
    public EX4_1(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicIncludePaths.Add(ModuleDirectory);
        PublicDependencyModuleNames.AddRange(new[] {"Core", "CoreUObject", "Engine", "InputCore", "UMG", "Slate", "SlateCore"});
    }
}
