modded class EffectAreaParams
{
    float m_ParamRotation = 0;
}

modded class EffectArea
{
	float m_Rotation = 0;

	override void SetupZoneData( EffectAreaParams params ) 
	{
		// A lot of branching, allowing to use default values on specified params
		if ( params.m_ParamName != "" )
			m_Name = params.m_ParamName;
		if ( params.m_ParamTriggerType != "" )
			m_TriggerType = params.m_ParamTriggerType;
		
		if ( params.m_ParamRadius > 0 )
			m_Radius = params.m_ParamRadius;
		if ( params.m_ParamPosHeight > -1 )
			m_PositiveHeight = params.m_ParamPosHeight;
		if ( params.m_ParamNegHeight > -1 )
			m_NegativeHeight = params.m_ParamNegHeight;
		
		m_InnerRings = params.m_ParamInnerRings;
		if ( params.m_ParamInnerSpace > -1 )
			m_InnerSpacing = params.m_ParamInnerSpace;
		
		m_OuterRingToggle = params.m_ParamOuterToggle;
		if ( params.m_ParamOuterSpace > -1 )
			m_OuterSpacing = params.m_ParamOuterSpace;
		m_OuterRingOffset = params.m_ParamOuterOffset;
		
		if ( params.m_ParamVertLayers > 0 )
			m_VerticalLayers = params.m_ParamVertLayers;
		if ( params.m_ParamVerticalOffset > 0 )
			m_VerticalOffset = params.m_ParamVerticalOffset;

        if ( params.m_ParamRotation > 0 && params.m_ParamRotation < 360 )
            m_Rotation = params.m_ParamRotation;
		
		m_ParticleID = params.m_ParamPartId;
		m_AroundParticleID = params.m_ParamAroundPartId;
		m_TinyParticleID = params.m_ParamTinyPartId;
		
		if ( params.m_ParamPpeRequesterType != "" )
		{
			m_PPERequesterType = params.m_ParamPpeRequesterType;
			m_PPERequesterIdx = GetRequesterIndex(m_PPERequesterType);			
		}
		// We get the PPE index for future usage and synchronization
		
		
		// DEVELOPER NOTE :
		// If you cannot register a new requester, add your own indexation and lookup methods to get an index and synchronize it
		// EXAMPLE : m_PPERequesterIdx = MyLookupMethod()
		
		// We sync our data
		SetSynchDirty();
		
		// Now that everything is ready, we finalize setup
		InitZone();
	}
}