//------------------------------------------------------------------------------------------------
modded class SCR_PlayerController : PlayerController
{
	//------------------------------------------------------------------------------------------------
	override void OnControlledEntityChanged(IEntity from, IEntity to)
	{
		super.OnControlledEntityChanged(from, to);
		
		if (!to)
			return;
		
		if (m_bIsPossessing)
			return;

		SCR_CharacterDamageManagerComponent damageManager = SCR_CharacterDamageManagerComponent.Cast(to.FindComponent(SCR_CharacterDamageManagerComponent));
		if (!damageManager)
			return;
		
		damageManager.RSA_Initialize(to);
	}
};