class azw_DamageArea extends ItemBase
{
	protected ref AreaDamageManager 	m_AreaDamage;

    void azw_DamageArea()
    {
    }

    void ~azw_DamageArea()
    {
        DestroyAreaDamage();
    }

    override void EEInit()
	{
		super.EEInit();
	
		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( CreateAreaDamage, 100, false );
	}

	//Create and Destroy damage radius around the fireplace when starting/stopping burning process
	void CreateAreaDamage()
	{
		//destroy area damage if some already exists
		//DestroyAreaDamage();
		
		//create new area damage
		m_AreaDamage = new AreaDamageLoopedDeferred(this);
		m_AreaDamage.SetDamageComponentType(AreaDamageComponentTypes.HITZONE);
		m_AreaDamage.SetExtents("-20 0 -20", "20 20 20");
		m_AreaDamage.SetLoopInterval( 0.5 );
		m_AreaDamage.SetDeferDuration( 0.5 );
		m_AreaDamage.SetHitZones( { "Head","Torso","LeftHand","LeftLeg","LeftFoot","RightHand","RightLeg","RightFoot" } );
		m_AreaDamage.SetAmmoName( "FireDamage" );
		m_AreaDamage.Spawn();
	}
	
	void DestroyAreaDamage()
	{
		if ( m_AreaDamage ) 
			m_AreaDamage.Destroy();
	}
}