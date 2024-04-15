class azw_SoundLoop
{
    protected ref Timer m_LoopTimer;
	protected ref Timer m_TimerStop;
	protected ref Timer m_TimerDelay;
	protected ref Timer m_TimerFadeIn;

    protected ref EffectSound m_Sound_A;
	protected ref EffectSound m_Sound_B;

    protected bool m_IsInitalized;
    protected bool m_IsPlaying;
	protected bool m_IsStarting;

    protected float m_LoopLength;
    protected float m_FadeLength;
	protected float m_FadeOutLength;
	protected float m_FadeInLength;
	protected float m_StopTime;
	protected float m_DelayTime;

    protected string m_SoundSet;
    protected Object m_ParentObject;



    void azw_SoundLoop()
    {
        m_LoopTimer = new Timer();
		m_TimerStop = new Timer();
		m_TimerDelay = new Timer();
		m_TimerFadeIn = new Timer();
        
        m_IsInitalized = false;
    }

    void ~azw_SoundLoop()
    {
        delete m_LoopTimer;
		delete m_TimerStop;
		delete m_TimerDelay;
		delete m_TimerFadeIn;
    }

    void Init(string sound_set, Object parent_object, float length, float fade)
    {
        m_SoundSet = sound_set;
        m_ParentObject = parent_object;
        m_LoopLength = length;
        m_FadeLength = fade;
		m_FadeOutLength = 0;
		m_FadeInLength = fade;
		m_StopTime = 0;
		m_DelayTime = 0;
        m_IsPlaying = false;
		m_IsStarting = false;
        m_IsInitalized = true;
    }
	
	void Reset()
    {
		m_StopSounds();
		m_FadeOutLength = 0;
		m_StopTime = 0;
		m_DelayTime = 0;
        m_IsPlaying = false;
		m_IsStarting = false;
    }

    void Play()
    {
        m_IsPlaying = true;
		m_IsStarting = true;
		

		m_TimerFadeIn.Run(m_FadeInLength, this, "m_SetLoopFade", NULL, false);

        if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
		{
			if (m_DelayTime > 0)
				m_TimerDelay.Run(m_DelayTime, this, "m_PlaySound_A", NULL, false);
			else
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_PlaySound_A, 5, false);
		}	
		    
		
		if (m_StopTime > 0)
			m_TimerStop.Run( m_StopTime, this, "Stop", NULL, false );
    }

	void Stop()
    {
		m_SetFadeOuts(m_FadeOutLength);
        m_IsPlaying = false;
        if ( GetGame().IsClient()  ||  !GetGame().IsMultiplayer() )
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(m_StopSounds, 15, false);
    }

    void SetTimer(float stop_time)
	{
		m_StopTime = stop_time;
	}
	
	void SetDelay(float delay_time)
	{
		m_DelayTime = delay_time;
	}
	
	void SetFadeOut(float fade_out)
	{
		m_FadeOutLength = fade_out;
	}
	
	void SetFadeIn(float fade_in)
	{
		m_FadeInLength = fade_in;
	}

    private void m_PlaySound_A()
	{
		if (!m_Sound_A || !m_Sound_A.IsSoundPlaying())
		{
			if (!m_IsStarting)
				m_Sound_A = SEffectManager.azwPlaySoundOnObject_shuffledLoop(m_SoundSet, m_ParentObject, m_FadeLength);
			else
				m_Sound_A = SEffectManager.azwPlaySoundOnObject_shuffledLoop(m_SoundSet, m_ParentObject, m_FadeInLength, m_FadeLength);
			
			m_LoopTimer.Run( (m_LoopLength - m_FadeLength), this, "m_StopSounds", NULL, false );
		}
	}
    
    private void m_PlaySound_B()
	{
		if (!m_Sound_B || !m_Sound_B.IsSoundPlaying())
		{
			
			m_Sound_B = SEffectManager.azwPlaySoundOnObject_shuffledLoop(m_SoundSet, m_ParentObject, m_FadeLength);
			m_LoopTimer.Run((m_LoopLength - m_FadeLength), this, "m_StopSounds", NULL, false );
		}
	}

    private void m_StopSounds()
	{
		if (m_Sound_A && m_Sound_A.IsSoundPlaying())
        {
            m_Sound_A.azwFadeOut();
			if (m_IsPlaying)
			{
				m_PlaySound_B();
			}
        }

		if (m_Sound_B && m_Sound_B.IsSoundPlaying())
        {
            m_Sound_B.azwFadeOut();
			if (m_IsPlaying)
			{
				m_PlaySound_A();
			}
        }		
	}
	
	private void m_SetFadeOuts(float fade_out)
	{
		m_Sound_A.SetSoundFadeOut(fade_out);
		m_Sound_B.SetSoundFadeOut(fade_out);
	}
	
	private void m_SetFadeIns(float fade_in)
	{
		m_Sound_A.SetSoundFadeIn(fade_in);
		m_Sound_B.SetSoundFadeIn(fade_in);
	}
	
	private void m_SetLoopFade()
	{
		m_IsStarting = false;
		m_Sound_A.SetSoundFadeOut(m_FadeLength);
		m_Sound_B.SetSoundFadeOut(m_FadeLength);
		m_Sound_A.SetSoundFadeIn(m_FadeLength);
		m_Sound_B.SetSoundFadeIn(m_FadeLength);
	}
	
	Object GetParentObject()
    {
        return m_ParentObject;
    }
}