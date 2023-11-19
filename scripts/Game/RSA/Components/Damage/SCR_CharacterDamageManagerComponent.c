//#define RSA_STAY_ALIVE_FOREVER

//------------------------------------------------------------------------------------------------
modded class SCR_CharacterDamageManagerComponent : ScriptedDamageManagerComponent
{
	protected bool m_bRSA_Initialized = false;
	protected HitZone m_pRSA_HealthHitZone;
	protected float m_fRSA_CriticalHealth;
#ifndef RSA_STAY_ALIVE_FOREVER
	protected const float RSA_UNCONSCIOUS_PROTECTION_TIMEOUT = 1000;
#endif

	//-----------------------------------------------------------------------------------------------------------
	//! Add handlers for protecting players that fall unconscious
	void RSA_Initialize(IEntity owner)
	{
		if (m_bRSA_Initialized)
			return;
		
		SCR_CharacterControllerComponent charCtrl = SCR_CharacterControllerComponent.Cast(owner.FindComponent(SCR_CharacterControllerComponent));
		if (!charCtrl)
			return;
		
	#ifndef RSA_STAY_ALIVE_FOREVER
		charCtrl.m_OnLifeStateChanged.Insert(RSA_OnLifeStateChanged);
	#else
		GetOnDamage().Insert(RSA_OnDamage_BloodHitZoneProtection);
	#endif
		GetOnDamage().Insert(RSA_OnDamage_HealthHitZoneProtection);
		m_pRSA_HealthHitZone = GetHitZoneByName("Health");		
		m_fRSA_CriticalHealth = m_pRSA_HealthHitZone.GetDamageStateThreshold(ECharacterHealthState.CRITICAL);
		m_bRSA_Initialized = true;
	}
	
#ifndef RSA_STAY_ALIVE_FOREVER
	//-----------------------------------------------------------------------------------------------------------
	//! Add/Remove protection handler when consciousness changes
	protected void RSA_OnLifeStateChanged(ECharacterLifeState lifeState)
	{
		switch (lifeState)
		{
			// Add protection handler when revived
			case ECharacterLifeState.ALIVE:
			{
				GetGame().GetCallqueue().Remove(RSA_EnableHealthHitZoneProtection);
				RSA_EnableHealthHitZoneProtection(true);
				break;
			};
			
			// Schedule removal of protection handler when falling unconscious
			case ECharacterLifeState.INCAPACITATED:
			{
				GetGame().GetCallqueue().CallLater(RSA_EnableHealthHitZoneProtection, RSA_UNCONSCIOUS_PROTECTION_TIMEOUT, false, false);
				break;
			};
			
			// Remove protection handler when dead
			case ECharacterLifeState.DEAD:
			{
				GetGame().GetCallqueue().Remove(RSA_EnableHealthHitZoneProtection);
				RSA_EnableHealthHitZoneProtection(false);
				break;
			};
		};
	}
#else
	//-----------------------------------------------------------------------------------------------------------
	// Prevent the blood hit zone from getting destroyed
	protected void RSA_OnDamage_BloodHitZoneProtection(EDamageType type, float damage, HitZone pHitZone, notnull Instigator instigator, inout vector hitTransform[3], float speed, int colliderID, int nodeID)
	{
		if (m_pBloodHitZone.GetHealth() < 1)
			m_pBloodHitZone.SetHealth(1);
	}
#endif
	
	//-----------------------------------------------------------------------------------------------------------
	//! Add/remove protection handler
	void RSA_EnableHealthHitZoneProtection(bool enabled)
	{
		GetOnDamage().Remove(RSA_OnDamage_HealthHitZoneProtection);
		
		if (enabled)
			GetOnDamage().Insert(RSA_OnDamage_HealthHitZoneProtection);
	}
	
	//-----------------------------------------------------------------------------------------------------------
	// Prevent the health hit zone from getting destroyed
	protected void RSA_OnDamage_HealthHitZoneProtection(EDamageType type, float damage, HitZone pHitZone, notnull Instigator instigator, inout vector hitTransform[3], float speed, int colliderID, int nodeID)
	{
		if (m_pRSA_HealthHitZone.GetHealth() < 1)
		{
			m_pRSA_HealthHitZone.SetHealth(1);
			m_pResilienceHitZone.SetHealth(1);
		};
	}
	
	//-----------------------------------------------------------------------------------------------------------
	//! Check whether character health state meets requirements for consciousness
	override protected bool ShouldBeUnconscious()
	{
		if (m_bRSA_Initialized && m_pRSA_HealthHitZone.GetHealthScaled() <= m_fRSA_CriticalHealth)
			return true;
		
		return super.ShouldBeUnconscious();
	}
}