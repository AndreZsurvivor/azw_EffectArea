class CfgPatches
{
    class azw_EffectArea
    {
		units[]				=	{"AvalancheArea", "azw_DamageArea"};
		weapons[]			=	{};
		requiredVersion		=	0.1;
		requiredAddons[]	=	{"DZ_Data","DZ_Scripts"};
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

class CfgVehicles
{
	class EffectArea;
	class AvalancheArea_base : EffectArea
	{
		scope=2;
	};
	class AvalancheArea_dynamic : AvalancheArea_base
	{
	};

	class Inventory_Base;
	class azw_DamageArea : Inventory_Base
	{
		scope=2;
	};
};