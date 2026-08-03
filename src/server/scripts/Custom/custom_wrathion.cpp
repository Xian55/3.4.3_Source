#include "ScriptMgr.h"

// alpha testing
#include "CollectionMgr.h"
#include "DB2Stores.h"


namespace
{
    enum class LoginSpells : uint32
    {
        JOYOUS_JOURNEYS             = 377749,
    };

    static constexpr std::array<LoginSpells, 1> g_LoginSpells = { LoginSpells::JOYOUS_JOURNEYS };

    void ApplyLoginSpells(Player* player, bool firstLogin)
    {
        for (LoginSpells spell : g_LoginSpells)
        {
            if (!player->HasAura(uint32(spell)))
            {
                if (Aura* aura = player->AddAura(uint32(spell), player))
                {
                    for (uint8 i = EFFECT_0; i <= EFFECT_1; ++i)
                    {
                        AuraEffect* xp = aura->GetEffect(i);
                        xp->SetAmount(xp->GetBaseAmount());
                    }
                }
            }
        }
    }

    void ProcessAlphaItems(Player* player)
    {
        if (!player->HasItemCount(1100, 1))
        {
            player->AddItem(1100, 4);    // Monster bag
            player->ModifyMoney(100000); // 10g

            // Add all available heirlooms
            for (HeirloomEntry const* heirloom : sHeirloomStore)
                player->GetSession()->GetCollectionMgr()->AddHeirloom(heirloom->ItemID, 0);
        }
    }

    void ProcessUnclaimedBpayItems(const WorldSession* session)
    {
        LoginDatabasePreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_BATTLE_PAY_PURCHASES);
        stmt->setUInt32(0, session->GetAccountId());

        if (PreparedQueryResult result = LoginDatabase.Query(stmt))
        {
            do
            {
                const auto fields = result->Fetch();

                const uint32 id        = fields[0].GetUInt32();
                const uint32 productId = fields[1].GetUInt32();

                Battlepay::Purchase purchase;
                purchase.ProductID  = productId;
                purchase.PurchaseID = id;

                session->GetBattlePayMgr()->ProcessDelivery(purchase, true);

            } while (result->NextRow());
        }
    }
}

class CustomWrathionPlayer : public PlayerScript
{
public:
    CustomWrathionPlayer() 
        : PlayerScript("CustomWrathionPlayer") { }

    void OnLogin(Player* player, bool firstLogin) override
    {
        const WorldSession* session = player->GetSession();

        // Apply login spells
        ApplyLoginSpells(player, firstLogin);

        // Process any unclaimed bpay purchases
        ProcessUnclaimedBpayItems(session);

        // Process Alpha Testing items
        //ProcessAlphaItems(player);
    }
};

void AddSC_CustomWrathion()
{
    new CustomWrathionPlayer();
}
