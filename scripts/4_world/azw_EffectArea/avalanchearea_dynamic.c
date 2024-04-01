enum AvalancheAreaState
{
	INIT 			= 1, // The dynamic area is initializing
	START 			= 2, // The dynamic area is starting
	LIVE 			= 3, // The dynamic area is live
	DECAY_START 	= 4, // The dynamic area decay has started
	DECAY_END 		= 5, // The dynamic area will soon be deleted
	STOP			= 6, // The Event has finished
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
	const float				LIFETIME_DEFAULT			= 80;
	const int 				AREA_SETUP_DELAY 			= 20;
	
	// Constants used for dissapearing events
	const float 			START_DECAY_LIFETIME		= 20;
	const float 			FINISH_DECAY_LIFETIME		= 10;
	
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
			m_StartupTimer.Run( AREA_SETUP_DELAY, this, "InitZone" );
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
		m_Lifetime -= TICK_RATE;
		Print("tick");
		Print(m_Lifetime);
		if ( GetRemainingTime() < FINISH_DECAY_LIFETIME)
		{
			// The second state of decay, further reduction of particle density and size
			SetDecayState( AvalancheAreaState.DECAY_END );
		}
		else if ( GetRemainingTime() < START_DECAY_LIFETIME + START_DECAY_LIFETIME )
		{
			// The first state of decay, slight reduction in particle density and size
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
		
		#ifdef DEVELOPER
		// Debugs when placing entity by hand using internal tools
		/*if ( GetGame().IsServer() && !GetGame().IsMultiplayer() )
		{
			Debug.Log("YOU CAN IGNORE THE FOLLOWING DUMP");
			InitZone();
			Debug.Log("YOU CAN USE FOLLOWING DATA PROPERLY");
		}*/
		#endif
		
		if ( m_AvalancheState == AvalancheAreaState.LIVE )
			InitZone(); // If it has already been created, we simply do the normal setup, no cool effects, force the LIVE state
		else if ( GetGame().IsClient() && m_AvalancheState > AvalancheAreaState.LIVE )
			InitZoneClient(); // Same as before but without state forcing
		
		super.EEInit();
	}
	
	// We spawn particles and setup trigger
	override void InitZone()
	{
		m_TimerTick = new Timer;
		m_TimerTick.Run(TICK_RATE, this, "Tick", NULL, true);
		m_AvalancheState = AvalancheAreaState.LIVE;
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
		Print("onvariablesync");
		if ( !m_ToxicClouds )
			m_ToxicClouds = new array<Particle>;
		
		switch ( m_AvalancheState )
		{
			case AvalancheAreaState.START:
				Print("Avalanche started");
				break;

			case AvalancheAreaState.LIVE:
			{
				Print("Avlanache LIVE");
				break;
			}
			case AvalancheAreaState.DECAY_START:
			{
				int loopIteration = 0;
				Print("Avlanache DECAY_START");
				// We go through all the particles bound to this area and update relevant parameters
				//Debug.Log("We start decay");
				foreach ( Particle particleA : m_ToxicClouds )
				{
					loopIteration++

					if (loopIteration > 4)
						break;

					if (loopIteration <= 4)
						particleA.Stop();

	
				}
				break;
			}
			case AvalancheAreaState.DECAY_END:
			{
				Print("Avlanache DECAY_END");
				// We go through all the particles bound to this area and update relevant parameters
				//Debug.Log("We finish decay");
				foreach ( Particle particleB : m_ToxicClouds )
				{
					loopIteration++

					if (loopIteration > 10)
						break;

					if (loopIteration <= 10)
						particleB.Stop();


				}
				break;
			}
			case AvalancheAreaState.STOP:
			{
				Print("Avlanache STOP");
				foreach ( Particle particleC : m_ToxicClouds )
				{
					particleC.Stop();
				}
				break;
			}
			default:
				break;
		}
	}
}