class AvalancheAreaManager
{
    protected ref array<vector> m_EmitterPoints;
    private int m_PlayersInTriggerZone = 0;
    private int m_PlayersInDamage = 0;
	void AvalancheAreaManager()
	{
        m_EmitterPoints = new ref array<vector>;
	}
	
	void ~AvalancheAreaManager()
	{
        delete m_EmitterPoints;
	}

    array<vector> GetEmitterPoints()
    {
        return m_EmitterPoints;
    }

    void InsertEmitterPoint(vector emitterPos)
    {
        if (m_EmitterPoints)
            m_EmitterPoints.Insert(emitterPos);
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