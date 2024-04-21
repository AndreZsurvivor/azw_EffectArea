class AvalancheArea_base : EffectArea
{
	protected azwGameTools  GameTools;
	protected ref array<vector> m_EmitterPoints;
	protected ref array<Object> m_DamageAreas;
	protected Object m_AvalancheSound;

	void AvalancheArea_base()
	{
		RegisterNetSyncVariableFloat("m_Rotation", 0, 0, 2);
	}
	void ~AvalancheArea_base()
	{
		GameTools.CleanupInstance();
	}
	override void OnPlayerEnterServer(PlayerBase player, EffectTrigger trigger)
	{
		super.OnPlayerEnterServer(player, trigger);
		player.IncreaseEffectAreaCount();
	}
	
	override void OnPlayerExitServer(PlayerBase player, EffectTrigger trigger)
	{
		super.OnPlayerExitServer(player, trigger);
		player.DecreaseEffectAreaCount();
	}

	void InsertEmitterPoint(vector pos)
	{
		if (m_EmitterPoints)
			m_EmitterPoints.Insert(pos);
	}

	Object SpawnObject(string type, vector position, vector orientation, float scale = 1.0)
	{
		Object obj = GetGame().CreateObjectEx( type, position, ECE_SETUP|ECE_KEEPHEIGHT );
		if (obj)
		{
			obj.SetPosition(position);
			obj.SetOrientation(orientation);
			obj.SetScale(scale);
			obj.Update();
		}
		return obj;
	}

	array<vector> CalculateEmittorPositions( vector pos, float radius, int nbRings, int innerSpacing, bool outerToggle, int outerSpacing, int outerOffset )
	{
		if (!m_EmitterPoints)
			m_EmitterPoints = new array<vector>;

		if (!m_DamageAreas)
			m_DamageAreas = new array<Object>;
		// Determine if we snap first layer to ground
		bool snapFirstLayer = true; 
		if ( m_Type == eZoneType.STATIC && pos[1] != GetGame().SurfaceRoadY( pos[0], pos[2] ) )
			snapFirstLayer = false;
		
		// BEGINNING OF SAFETY NET
		// We want to prevent divisions by 0
		if ( radius == 0 )
		{
			// In specific case of radius, we log an error and return as it makes no sense
			Error("[WARNING] :: [EffectArea PlaceParticles] :: Radius of contaminated zone is set to 0, this should not happen");
			return null;
		}
		
		if ( outerToggle && radius == outerOffset )
		{
			Error("[WARNING] :: [EffectArea PlaceParticles] :: Your outerOffset is EQUAL to your Radius, this will result in division by 0");
			return null;
		}
		
		// Inner spacing of 0 would cause infinite loops as no increment would happen
		if ( innerSpacing == 0 )
			innerSpacing = 1;

		if ( !m_EmitterPoints )
			m_EmitterPoints = new array<vector>;
		
		// END OF SAFETY NET
		
		int partCounter = 0; // Used for debugging, allows one to know how many emitters are spawned in zone
		int numberOfEmitters = 1; // We always have the central emitter
		
		float angle = m_Rotation + 100; // rotation parameter passed from cfgeffectarea.json + offset to align particledirection

		azwGameTools  GameTools = azwGameTools.Get();
		
		vector partPos = pos;
		vector partPos2 = pos;
		vector partPos3 = pos;
		vector tempInit = vector.Zero;
		vector centerSurfaceOrientation = GameTools.GetSurfaceOrientation_Average( partPos[0], partPos[2], 10 );//GetGame().GetSurfaceOrientation( partPos[0], partPos[2]);
		
		centerSurfaceOrientation[0] = m_Rotation + 100;

		// We will want to start by placing a particle at center of area
		m_EmitterPoints.Insert(partPos);
		++partCounter;
		
		// For each concentric ring, we place a particle emitter at a set offset
		for ( int i = 1; i <= nbRings + outerToggle; i++ )
		{
			// We prepare the variables to use later in calculation
			float ab; 					// Length of a side of triangle used to calculate particle positionning
			vector temp = vector.Zero; 	// Vector we rotate to position next spawn point
			
			// The particle density is not the same on the final ring which will only happen if toggled
			// Toggle uses bool parameter treated as int, thus i > nbRings test ( allows to limit branching )
			if ( i > nbRings )
			{
				ab = radius - outerOffset; // We want to leave some space to better see area demarcation
				temp[2] = temp[2] + ab;
				//Debug.Log("Radius of last circle " + i + " is : " + ab);
			}
			else
			{
				ab = ( radius / ( nbRings + 1 ) ) * i; // We add the offset from one ring to another
				temp[2] = temp[2] + ab;
				//Debug.Log("Radius of inner circle " + i + " is : " + ab);
			}
			
			if ( i == 1 )
				tempInit = temp;
		
			partPos = vector.RotateAroundZeroDeg( temp, vector.Up, angle );
			partPos += pos;
			partPos[1] = GetGame().SurfaceRoadY( partPos[0], partPos[2]);
		
			partPos2 = vector.RotateAroundZeroDeg( tempInit, vector.Up, ( angle + 90 ));
			partPos2 += partPos;
			partPos2[1] = GetGame().SurfaceRoadY( partPos2[0], partPos2[2]);

			partPos3 = vector.RotateAroundZeroDeg( tempInit, vector.Up, ( angle - 90 ));
			partPos3 += partPos;
			partPos3[1] = GetGame().SurfaceRoadY( partPos3[0], partPos3[2]);
				
			// We snap first layer to ground if specified
			if ( snapFirstLayer == true )
				partPos[1] = GetGame().SurfaceY( partPos[0], partPos[2] );
			
			else if ( snapFirstLayer == false )
				partPos[1] = partPos[1] - m_NegativeHeight;
			
			// We check the particle is indeed in the trigger to make it consistent
			if ( partPos[1] <= pos[1] + m_PositiveHeight && partPos[1] >= pos[1] - m_NegativeHeight )
			{
				vector tempCenterSurfaceOrientation = GameTools.GetSurfaceOrientation_Average( partPos[0], partPos[2], 2 );
				tempCenterSurfaceOrientation[0] = ((98*centerSurfaceOrientation[0] + 2*tempCenterSurfaceOrientation[0]) / 100);
				m_EmitterPoints.Insert(partPos);
				++partCounter;
			
				tempCenterSurfaceOrientation = GameTools.GetSurfaceOrientation_Average( partPos2[0], partPos2[2], 2 );
				tempCenterSurfaceOrientation[0] = ((98*centerSurfaceOrientation[0] + 2*tempCenterSurfaceOrientation[0]) / 100);
				m_EmitterPoints.Insert(partPos2);

				++partCounter;
			
				tempCenterSurfaceOrientation = GameTools.GetSurfaceOrientation_Average( partPos3[0], partPos3[2], 2 );
				tempCenterSurfaceOrientation[0] = ((98*centerSurfaceOrientation[0] + 2*tempCenterSurfaceOrientation[0]) / 100);
				m_EmitterPoints.Insert(partPos3);
				++partCounter;
			}
		}
		m_DamageAreas.Reserve(partCounter);

		return m_EmitterPoints;
	}

	override void PlaceParticles( vector pos, float radius, int nbRings, int innerSpacing, bool outerToggle, int outerSpacing, int outerOffset, int partId )
	{
#ifdef NO_GUI
		return; // do not place any particles if there is no GUI
#endif
		if (partId == 0)
		{
			Error("[WARNING] :: [EffectArea PlaceParticles] :: no particle defined, skipping area particle generation" );
			return;
		}
		// Determine if we snap first layer to ground
		bool snapFirstLayer = true; 
		if ( m_Type == eZoneType.STATIC && pos[1] != GetGame().SurfaceRoadY( pos[0], pos[2] ) )
			snapFirstLayer = false;
		
		// BEGINNING OF SAFETY NET
		// We want to prevent divisions by 0
		if ( radius == 0 )
		{
			// In specific case of radius, we log an error and return as it makes no sense
			Error("[WARNING] :: [EffectArea PlaceParticles] :: Radius of contaminated zone is set to 0, this should not happen");
			return;
		}
		
		if ( outerToggle && radius == outerOffset )
		{
			Error("[WARNING] :: [EffectArea PlaceParticles] :: Your outerOffset is EQUAL to your Radius, this will result in division by 0");
			return;
		}
		
		// Inner spacing of 0 would cause infinite loops as no increment would happen
		if ( innerSpacing == 0 )
			innerSpacing = 1;
		
		// END OF SAFETY NET
		
		int partCounter = 0; // Used for debugging, allows one to know how many emitters are spawned in zone
		int numberOfEmitters = 1; // We always have the central emitter
		
		
		//Debug.Log("We have : " + nbRings + " rings");
		//Debug.Log("We have : " + m_VerticalLayers + " layers");
		
		float angle = m_Rotation + 100; // rotation parameter passed from cfgeffectarea.json
		
		ParticlePropertiesArray props = new ParticlePropertiesArray();
		azwGameTools  GameTools = azwGameTools.Get();
		
		vector partPos = pos;
		vector partPos2 = pos;
		vector partPos3 = pos;
		vector tempInit = vector.Zero;
		vector centerSurfaceOrientation = GameTools.GetSurfaceOrientation_Average( partPos[0], partPos[2], 10 );//GetGame().GetSurfaceOrientation( partPos[0], partPos[2]);
		
		centerSurfaceOrientation[0] = m_Rotation;

		//Print(centerSurfaceOrientation);
		// We will want to start by placing a particle at center of area
		props.Insert(ParticleProperties(partPos, ParticlePropertiesFlags.PLAY_ON_CREATION, null, centerSurfaceOrientation, this));
		//AvalancheManager.InsertEmitterPoint(partPos);
		//m_EmitterPoints.Insert(partPos);
		//SpawnObject("azw_DamageArea", partPos, "0.000000 -0.000000 -0.000000", 1);
		++partCounter;


			
		// For each concentric ring, we place a particle emitter at a set offset
		for ( int i = 1; i <= nbRings + outerToggle; i++ )
		{
			//Debug.Log("We are on iteration I : " + i );
			
			// We prepare the variables to use later in calculation
			float ab; 					// Length of a side of triangle used to calculate particle positionning
			vector temp = vector.Zero; 	// Vector we rotate to position next spawn point
			
			// The particle density is not the same on the final ring which will only happen if toggled
			// Toggle uses bool parameter treated as int, thus i > nbRings test ( allows to limit branching )
			if ( i > nbRings )
			{
				ab = radius - outerOffset; // We want to leave some space to better see area demarcation
				temp[2] = temp[2] + ab;
				//Debug.Log("Radius of last circle " + i + " is : " + ab);
			}
			else
			{
				ab = ( radius / ( nbRings + 1 ) ) * i; // We add the offset from one ring to another
				temp[2] = temp[2] + ab;
				//Debug.Log("Radius of inner circle " + i + " is : " + ab);
			}
			
			if ( i == 1 )
				tempInit = temp;
		
			partPos = vector.RotateAroundZeroDeg( temp, vector.Up, angle );
			partPos += pos;
			partPos[1] = GetGame().SurfaceRoadY( partPos[0], partPos[2]);
		
			partPos2 = vector.RotateAroundZeroDeg( tempInit, vector.Up, ( angle + 90 ));
			partPos2 += partPos;
			partPos2[1] = GetGame().SurfaceRoadY( partPos2[0], partPos2[2]);

			partPos3 = vector.RotateAroundZeroDeg( tempInit, vector.Up, ( angle - 90 ));
			partPos3 += partPos;
			partPos3[1] = GetGame().SurfaceRoadY( partPos3[0], partPos3[2]);
				
			// We snap first layer to ground if specified
			if ( snapFirstLayer == true )
				partPos[1] = GetGame().SurfaceY( partPos[0], partPos[2] );
			
			else if ( snapFirstLayer == false )
				partPos[1] = partPos[1] - m_NegativeHeight;
			
			// We check the particle is indeed in the trigger to make it consistent
			if ( partPos[1] <= pos[1] + m_PositiveHeight && partPos[1] >= pos[1] - m_NegativeHeight )
			{
				vector tempCenterSurfaceOrientation = GameTools.GetSurfaceOrientation_Average( partPos[0], partPos[2], 2 );
				tempCenterSurfaceOrientation[0] = ((98*centerSurfaceOrientation[0] + 2*tempCenterSurfaceOrientation[0]) / 100);
				props.Insert(ParticleProperties(partPos, ParticlePropertiesFlags.PLAY_ON_CREATION, null, tempCenterSurfaceOrientation, this));
				props.Insert(ParticleProperties(partPos, ParticlePropertiesFlags.PLAY_ON_CREATION, null, tempCenterSurfaceOrientation, this));
				++partCounter;
				++partCounter;
			
				tempCenterSurfaceOrientation = GameTools.GetSurfaceOrientation_Average( partPos2[0], partPos2[2], 2 );
				tempCenterSurfaceOrientation[0] = ((98*centerSurfaceOrientation[0] + 2*tempCenterSurfaceOrientation[0]) / 100);
				props.Insert(ParticleProperties(partPos2, ParticlePropertiesFlags.PLAY_ON_CREATION, null, tempCenterSurfaceOrientation, this));
				++partCounter;
			
				tempCenterSurfaceOrientation = GameTools.GetSurfaceOrientation_Average( partPos3[0], partPos3[2], 2 );
				tempCenterSurfaceOrientation[0] = ((98*centerSurfaceOrientation[0] + 2*tempCenterSurfaceOrientation[0]) / 100);
				props.Insert(ParticleProperties(partPos3, ParticlePropertiesFlags.PLAY_ON_CREATION, null, tempCenterSurfaceOrientation, this));
				++partCounter;
			}
		}
		m_ToxicClouds.Reserve(partCounter);
		ParticleManager gPM = ParticleManager.GetInstance();
		
		array<ParticleSource> createdParticles = gPM.CreateParticlesByIdArr(partId, props, partCounter);
		if (createdParticles.Count() != partCounter)
		{
			if (gPM.IsFinishedAllocating())
			{
				ErrorEx(string.Format("Not enough particles in pool for EffectArea: %1", m_Name));
				OnParticleAllocation(gPM, createdParticles);
			}
			else
			{
				gPM.GetEvents().Event_OnAllocation.Insert(OnParticleAllocation);
			}
		}
		else
		{
			OnParticleAllocation(gPM, createdParticles);
		}
		Print("particles placed");
	}
}

class AvalancheArea_static : AvalancheArea_base
{
	override void EEInit()
	{
		// We make sure we have the particle array
		if ( !m_ToxicClouds )
			m_ToxicClouds = new array<Particle>;
		
		SetSynchDirty();
		
		#ifdef DEVELOPER
		// Debugs when placing entity by hand using internal tools
		if ( GetGame().IsServer() && !GetGame().IsMultiplayer() )
		{
			Debug.Log("YOU CAN IGNORE THE FOLLOWING DUMP");
			InitZone();
			Debug.Log("YOU CAN USE FOLLOWING DATA PROPERLY");
		}
		#endif
		
		if ( GetGame().IsClient() && GetGame().IsMultiplayer() )
			InitZone();
		
		super.EEInit();
	}
	
	override void InitZoneServer()
	{
		super.InitZoneServer();
		
		// We create the trigger on server
		if ( m_TriggerType != "" )
			CreateTrigger( m_Position, m_Radius );


	}
	
	override void InitZoneClient()
	{
		super.InitZoneClient();
		
		// We spawn VFX on client
		PlaceParticles( GetWorldPosition(), m_Radius, m_InnerRings, m_InnerSpacing, m_OuterRingToggle, m_OuterSpacing, m_OuterRingOffset, m_ParticleID );
	}


}
