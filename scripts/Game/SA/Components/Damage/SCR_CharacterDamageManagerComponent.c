//------------------------------------------------------------------------------------------------
modded class SCR_CharacterDamageManagerComponent : ScriptedDamageManagerComponent
{
	protected PlayerManager m_pPlayerManager;
	protected SCR_PossessingManagerComponent m_pPossessingManager;
	protected HitZone m_pHealthHitZone;
	
	//-----------------------------------------------------------------------------------------------------------
	override protected void OnPostInit(IEntity owner)
	{
		super.OnPostInit(owner);
		m_pPossessingManager = SCR_PossessingManagerComponent.Cast(GetGame().GetGameMode().FindComponent(SCR_PossessingManagerComponent));
		m_pHealthHitZone = GetHitZoneByName("Health");
	}
	
	//-----------------------------------------------------------------------------------------------------------
	//! Ensure that players cannot die
	protected override void OnDamage(EDamageType type, float damage, HitZone pHitZone, notnull Instigator instigator, inout vector hitTransform[3], float speed, int colliderID, int nodeID)
	{		
		super.OnDamage(type, damage, pHitZone, instigator, hitTransform, speed, colliderID, nodeID);
	
		int playerId = GetGame().GetPlayerManager().GetPlayerIdFromControlledEntity(GetOwner());
		
		// Skip handling of unpossessed units that are not a player
		if (m_pPossessingManager.GetIdFromMainEntity(GetOwner()) == 0)
			return;
		
		// Skip handling for possessed units
		if (m_pPossessingManager.IsPossessing(playerId))
			return;
		
		if (m_pHealthHitZone.GetHealth() < 1)
		{
			m_pHealthHitZone.SetHealth(1);
			// Set resiliance to ensure the player falls unconscious
			m_pResilienceHitZone.SetHealth(1);
		}
		
		if (m_pBloodHitZone.GetHealth() < 1)
			m_pBloodHitZone.SetHealth(1);
	}	
}