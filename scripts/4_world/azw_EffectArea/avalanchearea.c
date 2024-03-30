class AvalancheArea : EffectArea
{
	void AvalancheArea()
	{
		RegisterNetSyncVariableFloat("m_Rotation", 0, 0, 2);

		Print(m_Rotation);
	}

	// ----------------------------------------------
	// 				INITIAL SETUP
	// ----------------------------------------------
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
		
		float angle = m_Rotation; // Used in for loop to know where we are in terms of angle spacing ( RADIANS )
		
		ParticlePropertiesArray props = new ParticlePropertiesArray();
		
		// We also populate vertically, layer 0 will be snapped to ground, subsequent layers will see particles floating and relevant m_VerticalOffset
		//for ( int k = 0; k <= m_VerticalLayers; k++ )
		//{
            int k = 0;
			vector partPos = pos;
			// We prevent division by 0
			// We don't want to tamper with ground layer vertical positioning
			if ( k != 0 )
			{
				partPos[1] = partPos[1] + ( m_VerticalOffset * k );
			}
			
            vector partPos02 = { 9756, 0, 10350 };
            partPos02[1] = GetGame().SurfaceY( partPos02[0], partPos02[2] );

            vector partPos03 = { 9760, 0, 10350 };
            partPos03[1] = GetGame().SurfaceY( partPos03[0], partPos03[2] );

            vector partPos04 = { 9765, 0, 10350 };
            partPos04[1] = GetGame().SurfaceY( partPos04[0], partPos04[2] );

			// We will want to start by placing a particle at center of area
			props.Insert(ParticleProperties(partPos, ParticlePropertiesFlags.PLAY_ON_CREATION, null, GetGame().GetSurfaceOrientation( partPos[0], partPos[2]), this));
            //props.Insert(ParticleProperties(partPos02, ParticlePropertiesFlags.PLAY_ON_CREATION, null, GetGame().GetSurfaceOrientation( partPos02[0], partPos02[2]), this));
            //props.Insert(ParticleProperties(partPos03, ParticlePropertiesFlags.PLAY_ON_CREATION, null, GetGame().GetSurfaceOrientation(  partPos03[0], partPos03[2]), this));
            //props.Insert(ParticleProperties(partPos04, ParticlePropertiesFlags.PLAY_ON_CREATION, null, GetGame().GetSurfaceOrientation(  partPos04[0], partPos04[2]), this));
			++partCounter;
			
			// For each concentric ring, we place a particle emitter at a set offset
			for ( int i = 1; i <= nbRings + outerToggle; i++ )
			{
				//Debug.Log("We are on iteration I : " + i );
				
				// We prepare the variables to use later in calculation
				float angleIncrement; 		// The value added to the offset angle to place following particle
				float ab; 					// Length of a side of triangle used to calculate particle positionning
				vector temp = vector.Zero; 	// Vector we rotate to position next spawn point
				
				// The particle density is not the same on the final ring which will only happen if toggled
				// Toggle uses bool parameter treated as int, thus i > nbRings test ( allows to limit branching )
				if ( i > nbRings )
				{
					ab = radius - outerOffset; // We want to leave some space to better see area demarcation
					
					// We calculate the rotation angle depending on particle spacing and distance from center
					angleIncrement = Math.Acos( 1 - ( ( outerSpacing * outerSpacing ) / ( 2 * Math.SqrInt(ab) ) ) );
					temp[2] = temp[2] + ab;
					
					//Debug.Log("Radius of last circle " + i + " is : " + ab);
				}
				else
				{
					ab = ( radius / ( nbRings + 1 ) ) * i; // We add the offset from one ring to another
					
					// We calculate the rotation angle depending on particle spacing and distance from center
					angleIncrement = Math.Acos( 1 - ( ( innerSpacing * innerSpacing ) / ( 2 * Math.SqrInt(ab) ) ) );
					temp[2] = temp[2] + ab;
					
					//Debug.Log("Radius of inner circle " + i + " is : " + ab);
				}
				
				//for ( int j = 0; j <= ( Math.PI2 / angleIncrement ); j++ )
				//{
					// Determine position of particle emitter
					// Use offset of current ring for vector length
					// Use accumulated angle for vector direction
					
					//float sinAngle = Math.Sin( angle );
					//float cosAngle = Math.Cos( angle );
					Print(angle);
				
					partPos = vector.RotateAroundZeroDeg( temp, vector.Up, angle);
					partPos += pos;

					Print(partPos);
					
					// We snap first layer to ground if specified
					if ( k == 0 && snapFirstLayer == true )
						partPos[1] = GetGame().SurfaceY( partPos[0], partPos[2] );
					else if ( k == 0 && snapFirstLayer == false )
						partPos[1] = partPos[1] - m_NegativeHeight;
					
					// We check the particle is indeed in the trigger to make it consistent
					if ( partPos[1] <= pos[1] + m_PositiveHeight && partPos[1] >= pos[1] - m_NegativeHeight )
					{
						// Place emitter at vector end ( coord )
						props.Insert(ParticleProperties(partPos, ParticlePropertiesFlags.PLAY_ON_CREATION, null, GetGame().GetSurfaceOrientation( partPos[0], partPos[2] ), this));
						Print("particle placed");
						
						++partCounter;
					}

					// Increase accumulated angle
					//angle += angleIncrement;
				//}
                
				
				//angle = 0; // We reset our accumulated angle for the next ring
			//}
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
		
		//Debug.Log("Emitter count : " + partCounter );
	}
}