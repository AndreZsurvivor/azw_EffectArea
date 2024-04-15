enum AvalancheAreaState
{
	INIT 			= 1, // The dynamic area is initializing
	READY			= 2,
	START 			= 3, // The dynamic area is starting
	LIVE 			= 4, // The dynamic area is live
	DECAY_START 	= 5, // The dynamic area decay has started
	DECAY_END 		= 6, // The dynamic area will soon be deleted
	STOP			= 7, // The Event has finished
	COOLDOWN		= 8
}

// The dynamic AvalancheArea, using it's own default settings
class AvalancheArea_dynamic : AvalancheArea_base
{
	const float				TICK_RATE			= 1;
	protected ref Timer		m_TimerTick;
	protected float			m_Lifetime			= LIFETIME_DEFAULT;

	protected ref Timer 	m_StartupTimer;
	protected int 			m_AvalancheState 	= AvalancheAreaState.INIT;
	
	// Constants used for startup events
	const float				LIFETIME_DEFAULT			= 12;
	const int 				AREA_SETUP_DELAY 			= 2;
	
	// Constants used for dissapearing events
	const float 			START_DECAY_LIFETIME		= 1;
	const float 			FINISH_DECAY_LIFETIME		= 0.5;
	const float				COOLDOWN_TIME				= 1500;
	
	void AvalancheArea_dynamic()
	{
		RegisterNetSyncVariableInt("m_AvalancheState");
	}
	
	override void EEOnCECreate()
	{
		// We get the PPE index for future usage and synchronization ( we must do it here for dynamic as it is not read through file )
		if ( GetGame().IsServer() )
			m_PPERequesterIdx = GetRequesterIndex(m_PPERequesterType);
		
		SetSynchDirty();
		
		// If this is the first initialization, we delay it in order to play cool effects
		if ( m_AvalancheState == AvalancheAreaState.INIT )
		{
			m_StartupTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
			//m_StartupTimer.Run( AREA_SETUP_DELAY, this, "InitZone" );
		}
	}
	
	float GetRemainingTime()
	{
		return m_Lifetime;
	}
	
	float GetStartDecayLifetime()
	{
		return START_DECAY_LIFETIME;
	}
	
	float GetFinishDecayLifetime()
	{
		return FINISH_DECAY_LIFETIME;
	}
	
	override void Tick()
	{
		if ( m_AvalancheState >= AvalancheAreaState.START )
			m_Lifetime -= TICK_RATE;

		
		//Print(m_AvalancheState);
		//Print(m_Lifetime);
		int insiders =  m_Trigger.GetInsiders().Count();
		//Print("insiders =" + insiders);
		
		if ( insiders > 0 && m_AvalancheState == AvalancheAreaState.READY )
		{
			int random100 = Math.RandomInt(0, 100);
			int chance = 95 - insiders;
			if (random100 > chance)
			{
				
				CalculateEmittorPositions(GetWorldPosition(), m_Radius, m_InnerRings, m_InnerSpacing, m_OuterRingToggle, m_OuterSpacing, m_OuterRingOffset);
				if ( m_EmitterPoints && m_EmitterPoints.Count() > 0)
				{
					int iterator = 0;
					foreach ( vector position: m_EmitterPoints)
					{
						vector orientation = vector.Zero;
						if (!iterator && !m_AvalancheSound)
						{
							m_AvalancheSound = SpawnObject("azw_AvalancheSound", position, orientation, 1);
						}

						if ( iterator )
						{
							orientation[0] = m_Rotation;
							vector positionCorrected = position;
							positionCorrected[1] = position[1] - 10;
							Object obj = SpawnObject("azw_DamageArea", positionCorrected, orientation, 1);
							m_DamageAreas.Insert(obj);
						}
						iterator++;			
					}
				}
				Print("random passed");
				SetDecayState( AvalancheAreaState.START );
				return;
			}
		}

		if (  insiders > 0 && m_AvalancheState == AvalancheAreaState.LIVE )
		{
		}
		
		if ( GetRemainingTime() < FINISH_DECAY_LIFETIME)
		{
			// The second state of decay, further reduction of particle density and size
			foreach (Object damage: m_DamageAreas)
			{
				GetGame().ObjectDelete( damage );
			}
			SetDecayState( AvalancheAreaState.DECAY_END );
		}
		else if ( GetRemainingTime() < START_DECAY_LIFETIME + START_DECAY_LIFETIME )
		{
			// The first state of decay, slight reduction in particle density and size
			iterator = 0;
			foreach (Object damageB: m_DamageAreas)
			{
				if (iterator < 6)
					GetGame().ObjectDelete( damageB );
				iterator++;
			}
			SetDecayState( AvalancheAreaState.DECAY_START );
		}
		if (m_Lifetime <= 0)
		{
			SetDecayState( AvalancheAreaState.STOP );
		}
	}
	
	// Set the new state of the Area
	void SetDecayState( int newState )
	{
		if (m_AvalancheState != newState)
		{
			m_AvalancheState = newState;
		
			// We update the trigger state values as we also want to update player bound effects
			if ( m_Trigger )
				AvalancheTrigger_dynamic.Cast( m_Trigger ).SetAreaState( m_AvalancheState );
			
			SetSynchDirty();
		}
	}
	
	override void EEInit()
	{
		// We make sure we have the particle array
		if ( !m_ToxicClouds )
			m_ToxicClouds = new array<Particle>;
		
		m_Name = "Default Dynamic";
		m_Type = eZoneType.DYNAMIC;
		m_TriggerType = "AvalancheTrigger_dynamic";
		
		SetSynchDirty();

		if ( m_AvalancheState == AvalancheAreaState.LIVE )
			InitZone(); // If it has already been created, we simply do the normal setup, no cool effects, force the LIVE state
		else if ( GetGame().IsClient() && m_AvalancheState > AvalancheAreaState.LIVE )
			InitZoneClient(); // Same as before but without state forcing
		
		super.EEInit();
	}
	
	// We spawn particles and setup trigger
	override void InitZone()
	{
		Print("InitZone");
		m_TimerTick = new Timer;
		m_TimerTick.Run(TICK_RATE, this, "Tick", NULL, true);
		m_AvalancheState = AvalancheAreaState.READY;
		SetSynchDirty();
		
		super.InitZone();
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
		
		if ( !m_ToxicClouds )
			m_ToxicClouds = new array<Particle>;

		// We spawn VFX on client
		PlaceParticles( GetWorldPosition(), m_Radius, m_InnerRings, m_InnerSpacing, m_OuterRingToggle, m_OuterSpacing, m_OuterRingOffset, m_ParticleID );		
	}

	override void CreateTrigger( vector pos, int radius )
	{
		vector temp = { 0, 0, 0 };
		foreach (vector point: m_EmitterPoints)
		{
			temp += point;
		}
		temp[0] = temp[0]/m_EmitterPoints.Count();
		temp[1] = temp[1]/m_EmitterPoints.Count();
		temp[2] = temp[2]/m_EmitterPoints.Count();

		//Print(m_EmitterPoints.Count());

		super.CreateTrigger( pos, radius );
		
		// This handles the specific case of dynamic triggers as some additionnal parameters are present
		AvalancheTrigger_dynamic dynaTrigger = AvalancheTrigger_dynamic.Cast( m_Trigger );
		if ( dynaTrigger )
		{
			dynaTrigger.SetLocalEffects( m_AroundParticleID, m_TinyParticleID, m_PPERequesterIdx );
			dynaTrigger.SetAreaState( m_AvalancheState );
		}
	}
	
	override void EEDelete( EntityAI parent )
	{
		super.EEDelete( parent );
	}
	
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		Print("OnVariablesSynchronized");
		if ( !m_ToxicClouds )
			m_ToxicClouds = new array<Particle>;
		
		switch ( m_AvalancheState )
		{
			case AvalancheAreaState.READY:
				Print("Avalanche READY");
				break;

			case AvalancheAreaState.START:
				Print("Avalanche START");
				m_Lifetime = LIFETIME_DEFAULT;
				InitZone();
				SetDecayState( AvalancheAreaState.LIVE );
				break;

			case AvalancheAreaState.LIVE:
				Print("Avlanache LIVE");
				break;

			case AvalancheAreaState.DECAY_START:
				int loopIteration = 0;
				Print("Avlanache DECAY_START");
				// We go through all the particles bound to this area and update relevant parameters
				foreach ( Particle particleA : m_ToxicClouds )
				{
					loopIteration++;

					if (loopIteration > 4)
						break;

					if (loopIteration <= 4)
						particleA.Stop();
				}
				break;

			case AvalancheAreaState.DECAY_END:
				Print("Avlanache DECAY_END");
				// We go through all the particles bound to this area and update relevant parameters
				foreach ( Particle particleB : m_ToxicClouds )
				{
					loopIteration++;

					if (loopIteration > 10)
						break;

					if (loopIteration <= 10)
						particleB.Stop();
				}
				break;

			case AvalancheAreaState.STOP:
				Print("Avlanache STOP");
				foreach ( Particle particleC : m_ToxicClouds )
				{
					particleC.Stop();
				}
				break;

			default:
				break;
		}
	}
}