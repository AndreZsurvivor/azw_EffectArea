class azw_AvalancheSound : BuildingSuper
{
	const int SOUND_LAYERS = 4;
	protected ref array<ref azw_SoundLoop> m_Sounds;

	void azw_AvalancheSound()
	{
		m_Sounds = new array<ref azw_SoundLoop>;
		m_Sounds.Reserve(SOUND_LAYERS);

		for ( int i = 1; i <= SOUND_LAYERS; i++ )
		{
			m_Sounds.Insert(new azw_SoundLoop());
		}
		m_Sounds.Get(0).Init("azw_EQuakeClose_SoundSet",	this, 10);
		m_Sounds.Get(1).Init("azw_EQuakeClose_SoundSet",	this, 8);
		m_Sounds.Get(2).Init("azw_EQuakeDistant_SoundSet",	this, 10);
		m_Sounds.Get(3).Init("azw_SubRumble_SoundSet",		this, 5);
		
		foreach ( azw_SoundLoop sound: m_Sounds )
		{
			sound.SetFadeIn(2);
			sound.SetFadeOut(10);
			sound.SetTimer(5);
			sound.Play();
		}
	}
	
	void ~azw_AvalancheSound()
	{
		StopSound();
		m_Sounds.Clear();
		delete m_Sounds;
	}

	void StopSound()
	{
		foreach ( azw_SoundLoop sound: m_Sounds )
		{
			sound.Stop();
		}
	}
}