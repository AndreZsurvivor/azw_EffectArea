class AvalancheAreaManager
{
    private int m_PlayersInTriggerZone = 0;
    private int m_PlayersInDamage = 0;
	void AvalancheAreaManager()
	{		
	}
	
	void ~AvalancheAreaManager()
	{
	}

    void IncreasePlayerInTrigger()
    {
        if (m_PlayersInTriggerZone <= 0)
            m_PlayersInTriggerZone = 1;
        else
            m_PlayersInTriggerZone++;
    }

    void IncreasePlayerInDamage()
    {
        if (m_PlayersInDamage <= 0)
            m_PlayersInDamage = 1;
        else
            m_PlayersInDamage++;
    }

    void DecreasePlayerInTrigger()
    {
        if (m_PlayersInTriggerZone <= 0)
            m_PlayersInTriggerZone = 0;
        else
            m_PlayersInTriggerZone--;
    }

    void DecreasePlayerInDamage()
    {
        if (m_PlayersInDamage <= 0)
            m_PlayersInDamage = 0;
        else
            m_PlayersInDamage--;
    }
}