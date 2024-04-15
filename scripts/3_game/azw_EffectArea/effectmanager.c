/**
\brief Manager class for managing Effect (EffectParticle, EffectSound)
	\warning Keeps a ref to any Effect registered (Created/Played), make sure to perform the necessary cleanup
*/
modded class SEffectManager
{
	static EffectSound azwPlaySoundOnObject_shuffledLoop(string sound_set, Object parent_object, float fade)
	{
		EffectSound sound_A;
        bool loop = true;
		sound_A = CreateSound(sound_set, parent_object.GetPosition(), fade, fade, loop);		
		azwPrepareSound(sound_A, parent_object, false);
		sound_A.SoundPlay();
		return sound_A;
	}
	
	static EffectSound azwPlaySoundOnObject_shuffledLoop(string sound_set, Object parent_object, float fade_in, float fade_out)
	{
		EffectSound sound_A;
        bool loop = true;
		sound_A = CreateSound(sound_set, parent_object.GetPosition(), fade_in, fade_out, loop);		
		azwPrepareSound(sound_A, parent_object, false);
		sound_A.SoundPlay();
		return sound_A;
	}
	
	static EffectSound azwPrepareSound(EffectSound sound, Object parent_object, bool autodestroy = true)
	{
		sound.SetParent( parent_object );
		sound.SetLocalPosition( vector.Zero );
		sound.SetAutodestroy(autodestroy);
		return sound;
	}
	
}