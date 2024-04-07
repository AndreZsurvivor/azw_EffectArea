modded class PlayerBase
{

	override void OnJumpStart()
    {
        super.OnJumpStart();
		

		
		//azw_playTestSoundLater();
		//Print("current_height:");
		//auto AZW_pos = AZW_getPlayerPos()[1];
		//Print(AZW_pos);
		//SpawnObject("azw_EffectTest", AZW_getPlayerPos(), "0.000000 -0.000000 -0.000000", 1);

		//PPERequesterBank.GetRequester(PPERequester_anzwStateMachine).Start();
    }

	static void SpawnObject(string type, vector position, vector orientation, float scale = 1.0)
	{
    	Object obj = GetGame().CreateObjectEx(type, position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS);

	    obj.SetPosition(position);
    	obj.SetOrientation(orientation);
    	obj.SetOrientation(obj.GetOrientation());
    	obj.SetScale(scale);
    	obj.Update();
		obj.SetAffectPathgraph(true, false);
		if (obj.CanAffectPathgraph()) {
        	GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, obj);
	    } 
	}
	
	vector AZW_getPlayerPos()
	{
		Man player = GetHierarchyRootPlayer();
		vector playerPos = player.GetPosition();
		return playerPos;
	}
};





	