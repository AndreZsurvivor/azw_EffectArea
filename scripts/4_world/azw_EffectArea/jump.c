modded class PlayerBase
{
	override void OnJumpStart()
    {
        super.OnJumpStart(); 
		SpawnObject("azw_EffectArea", azwGetPlayerPos(), "0.000000 -0.000000 -0.000000", 1);
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
	
	vector azwGetPlayerPos()
	{
		Man player = GetHierarchyRootPlayer();
		vector playerPos = player.GetPosition();
		return playerPos;
	}
};





	