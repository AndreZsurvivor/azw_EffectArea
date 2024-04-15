class CfgPatches
{
    class azw_EffectArea
    {
		units[]				=	{};
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

	class HouseNoDestruct;
	class azw_AvalancheSound : HouseNoDestruct
	{
		scope=2;
		EQUAKERANGE = 1000;
	};
};

class CfgSoundCurves
{
	class azw_EQuakeClose_VolumeCurve
	{
		points[] =
		{
			{0,1},
			{0.1,0.65},
			{0.2,0.50},
			{0.3,0.10},
			{0.4,0.00},
			{0.5,0.00},
			{0.6,0.00},
			{0.7,0.00},
			{0.8,0.00},
			{0.9,0.00},
			{1.0,0.00}
		};
	};

	class azw_EQuakeDistant_VolumeCurve
	{
		points[] =
		{
			{0,0.10},
			{0.1,0.35},
			{0.2,0.50},
			{0.3,0.90},
			{0.4,1.00},
			{0.5,0.90},
			{0.6,0.75},
			{0.7,0.25},
			{0.8,0.15},
			{0.9,0.07},
			{1.0,0.00}
		};
	};

	class azw_SubRumble_VolumeCurve
	{
		points[] =
		{
			{ 0.0, 1.0000 },
			{ 0.1, 0.9751 },
			{ 0.2, 0.6332 },
			{ 0.3, 0.4307 },
			{ 0.4, 0.3009 },
			{ 0.5, 0.2128 },
			{ 0.6, 0.1503 },
			{ 0.7, 0.1043 },
			{ 0.8, 0.0695 },
			{ 0.9, 0.0426 },
			{ 1.0, 0.0000 }
		};
	};
};
class CfgSound3DProcessors
{
	class azw_SubRumble3DProcessingType
	{
		type="panner";
		innerRange	= 100;
		range		= 200;
	};
};
class CfgDistanceFilters
{
	class azwTestAttenuationFilter
	{
		type = "lowPassFilter";
		minCutoffFrequency = 150;
		qFactor = 1;
		innerRange = 10;
		range = 1000;
		powerFactor = 32;
	};
};

class CfgSoundShaders
{
	class azw_EQuakeMainSoundBase_SoundShader
	{
		volume	= 1;
		range	= 500;
	};
	
	class azw_TestTones_SoundShader : azw_EQuakeMainSoundBase_SoundShader
	{
		samples[] =
		{	
			{ "azw_SoundOnObject\Sounds\TestTone01", 1 },
			{ "azw_SoundOnObject\Sounds\TestTone02", 1 },
			{ "azw_SoundOnObject\Sounds\TestTone03", 1 },
			{ "azw_SoundOnObject\Sounds\TestTone04", 1 }
		};
	};

	class azw_EQuakeClose_SoundShader : azw_EQuakeMainSoundBase_SoundShader
	{
		samples[] =
		{	
			{ "azw_SoundOnObject\Sounds\EarthQuake_Loop_01", 1 },
			{ "azw_SoundOnObject\Sounds\EarthQuake_Loop_02", 1 },
			{ "azw_SoundOnObject\Sounds\EarthQuake_Loop_03", 1 },
			{ "azw_SoundOnObject\Sounds\EarthQuake_Loop_04", 1 }
		};
	};

	class azw_EQuakeDistant_SoundShader : azw_EQuakeMainSoundBase_SoundShader
	{
		samples[] =
		{	
			{ "azw_SoundOnObject\Sounds\EarthQuake_Distasnt_Loop_01", 1 },
			{ "azw_SoundOnObject\Sounds\EarthQuake_Distasnt_Loop_02", 1 },
			{ "azw_SoundOnObject\Sounds\EarthQuake_Distasnt_Loop_03", 1 },
			{ "azw_SoundOnObject\Sounds\EarthQuake_Distasnt_Loop_04", 1 }
		};
	};
	
	class azw_SubRumble_SoundShader
	{
		samples[] =
		{	
			{ "azw_SoundOnObject\Sounds\SubBassRumble_Loop_01", 1 }
		};
		volume	= 0.9;
		range	= 150;
	};
};

class CfgSoundSets
{
	class azw_EQuakeBase_SoundSet
	{
		volumeFactor = 1.5;
		doppler = 0;
		spatial = 1;
		distanceFilter = "none";
	};

	class azw_TestTones_SoundSet : azw_EQuakeBase_SoundSet
	{
		soundShaders[] = { "azw_TestTones_SoundShader" };
		spatial = 0;
	};

	class azw_EQuakeClose_SoundSet : azw_EQuakeBase_SoundSet
	{
		soundShaders[] = { "azw_EQuakeClose_SoundShader" };
		sound3DProcessingType = "azw_SubRumble3DProcessingType";
		volumeCurve	= "azw_SubRumble_VolumeCurve";
	};

	class azw_EQuakeDistant_SoundSet : azw_EQuakeBase_SoundSet
	{
		soundShaders[] = { "azw_EQuakeDistant_SoundShader" };
		volumeCurve = "azw_EQuakeDistant_VolumeCurve";
	};

	class azw_SubRumble_SoundSet : azw_EQuakeBase_SoundSet
	{
		soundShaders[] = { "azw_SubRumble_SoundShader" };
		sound3DProcessingType  ="azw_SubRumble3DProcessingType";
		volumeCurve = "azw_SubRumble_VolumeCurve";
	};
};
