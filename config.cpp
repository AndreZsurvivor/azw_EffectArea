class CfgPatches
{
    class azw_EffectArea
    {
		units[]				=	{"AvalancheArea"};
		weapons[]			=	{};
		requiredVersion		=	0.1;
		requiredAddons[]	=	{"DZ_Data","DZ_Scripts"};
    };
};

class CfgVehicles
{
	class EffectArea;
	class AvalancheArea : EffectArea
	{
		scope=2;
	};
};

class CfgMods
{
	class azw_EffectArea
	{
		dir = "azw_EffectArea";
		picture = "";
		action = "";
		hideName = 1;
		hidePicture = 1;
		name = "azw_EffectArea";
		credits = "AndreZ";
		author = "AndreZ";
		authorID = "0"; 
		version = "0.1"; 
		extra = 0;
		type = "mod";
		
		dependencies[] = { "Game", "World" };
		
		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[]		=	{"azw_EffectArea/scripts/3_game"};
			};
			class worldScriptModule
			{
				value = "";
				files[] = {"azw_EffectArea/scripts/4_world"};
			};
		};
	};
};