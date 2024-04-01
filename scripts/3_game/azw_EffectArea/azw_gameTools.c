class azwGameTools : Managed
{
    //! Static azwGameTools
	private static ref azwGameTools g_azwGameTools;

	void azwGameTools()
	{		
	}
	
	void ~azwGameTools()
	{
	}

	//! Access to the static azwGameTools
	static azwGameTools Get()
	{
		if (!g_azwGameTools && !GetGame().IsDedicatedServer())
            g_azwGameTools = new azwGameTools();
        
        return g_azwGameTools;
	}


	//! To clean it up properly before game closes
	static void CleanupInstance()
	{
		if (g_azwGameTools)
			delete g_azwGameTools;
	}

	//! Returns tilt of the ground on the given position in degrees, so you can use this value to rotate any item according to terrain.
	vector GetSurfaceOrientation_Average(float x, float z, float d = 1)
	{
		vector orientation = GetGame().GetSurfaceOrientation( x, z );
		orientation += GetGame().GetSurfaceOrientation((x + d), z );
		orientation += GetGame().GetSurfaceOrientation((x - d), z );
		orientation += GetGame().GetSurfaceOrientation( x, (z + d));
		orientation += GetGame().GetSurfaceOrientation( x, (z - d));
		orientation[0] = orientation[0]/5;
		orientation[1] = orientation[1]/5;
		orientation[2] = orientation[2]/5;
		return orientation;
	}
}