//-----------------------------------------------------------------------------------------------------------
class RSA_CharacterHealthHitZone : ScriptedHitZone
{
	//-----------------------------------------------------------------------------------------------------------
	override void OnDamageStateChanged()
	{
		super.OnDamageStateChanged();
		
		UpdateConsciousness();
	}
	
	//-----------------------------------------------------------------------------------------------------------
	void UpdateConsciousness()
	{
		SCR_CharacterDamageManagerComponent damageManager = SCR_CharacterDamageManagerComponent.Cast(GetHitZoneContainer());
		if (damageManager)
			damageManager.UpdateConsciousness();
	}
}