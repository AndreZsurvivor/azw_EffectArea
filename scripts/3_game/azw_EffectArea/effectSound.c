modded class EffectSound
{
    void azwFadeOut()
	{
		if ( IsSoundPlaying() )
		{
			if ( m_SoundFadeOutDuration > 0 && !m_SoundWaveStopping )
			{
				m_SoundWaveStopping = true;
				m_SoundFadedOut = false;
				m_SoundWaveStarting = false;
				m_SoundFadeOutStartTime = m_SoundWaveTime;
			}
		}
		else
		{
			SoundReset();
		}	
	}
}