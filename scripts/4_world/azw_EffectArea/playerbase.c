modded class PlayerBase
{
	override void IncreaseEffectAreaCount()
	{
		if (m_EffectAreaCount == 0)
		{
			m_InsideEffectArea = true;
			SetSynchDirty();
			OnPlayerIsNowInsideEffectAreaBeginServer();
		}
		m_EffectAreaCount++;
	}
	
	override void DecreaseEffectAreaCount()
	{
		m_EffectAreaCount--;
		if (m_EffectAreaCount <= 0)
		{
			m_EffectAreaCount = 0;
			m_InsideEffectArea = false;
			SetSynchDirty();
			OnPlayerIsNowInsideEffectAreaEndServer();
		}
	}

	override protected void OnPlayerIsNowInsideEffectAreaBeginServer()
	{
        //GetModifiersManager().ActivateModifier( azwModifiers.MDF_AVALANCHEEXPOSURE );
		//Print("ANY EFFECT AREA ENTERED <------------------------------------------------------------- FINAL");
	}
	
	override protected void OnPlayerIsNowInsideEffectAreaEndServer()
	{
        //GetModifiersManager().DeactivateModifier( azwModifiers.MDF_AVALANCHEEXPOSURE );
		//Print("ANY EFFECT AREA LEFT <------------------------------------------------------------- FINAL");
	}
	
	override protected void OnPlayerIsNowInsideEffectAreaBeginClient()
	{
		//Print("ANY EFFECT AREA ENTERED CLIENT <------------------------------------------------------------- FINAL");
	}
	
	override protected void OnPlayerIsNowInsideEffectAreaEndClient()
	{
		RemoveCurrentEffectTrigger();
		//Print("ANY EFFECT AREA LEFT CLIENT <------------------------------------------------------------- FINAL");
	}
}