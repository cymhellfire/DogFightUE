local CardLogicCommand = require "Card.CardCommand.CardLogicCommand"
local CardModifierTypeDef = require "DogFight.Services.CardModifierService.CardModifierTypeDef"
local AttributeNameDef = require "DogFight.Common.AttributeNameDef"

---@class LogicFireball : CardLogicCommand Shoot fireball toward target.
---@field _ProjectileInfo table Settings of spawned projectile.
local LogicFireball = UnrealClass(CardLogicCommand)

local CommandNameDef = {
    AcquireTarget = "AcquireTarget",
    Fireball = "Fireball",
}

---@param DescObject UCardDescObject
function LogicFireball:SetupDescObject(DescObject)
    CardLogicCommand.SetupDescObject(self, DescObject)

    DescObject:SetCardName("LogicFireball")

    -- Add Test modifier
    ---@type CardModifierService
    local ModifierService = GetGameService(self._CardLogic, GameServiceNameDef.CardModifierService)
    if ModifierService then
        for i = 1, 2 do
            local Modifier = ModifierService:CreateCardModifier(CardModifierTypeDef.DoubleRandomInt)
            self._CardLogic:GetOwnerCard():AddModifierObject(Modifier)
        end
    end
end

---Initialize the card workflow
function LogicFireball:OnInit(InParam)
    CardLogicCommand.OnInit(self, InParam)

    -- Record projectile info
    self._ProjectileInfo = InParam.ProjectileInfo

    -- Register all commands
    local CommandTable = {
        [CommandNameDef.AcquireTarget] = {
            Script = "DogFight.Card.CardAction.ActionCardAcquireTarget",
            OnCreate = self.OnAcquireTargetCreated,
            OnFinish = self.OnAcquireTargetFinished,
        },
        [CommandNameDef.Fireball] = {
            Script = "DogFight.Card.CardAction.ActionLaunchProjectile",
            OnCreate = self.OnFireProjectileCreated,
            OnFinish = self.OnFireProjectileFinished,
        },
    }
    self:RegisterCommandTable(CommandTable)
end

---Start executing
function LogicFireball:StartCommand()
    CardLogicCommand.StartCommand(self)

    self:RunCommand(CommandNameDef.AcquireTarget)
end

---@param InCommand ActionCardAcquireTarget
function LogicFireball:OnAcquireTargetCreated(InCommand)
    if self._TargetInfo then
        InCommand:InitAcquireSettings(self._TargetInfo)
    else
        self:FailedWithParams()
    end
end

---Command finish callback
function LogicFireball:OnAcquireTargetFinished(Result, TargetInfo)
    self._TargetInfo = TargetInfo
    self:RunCommand(CommandNameDef.Fireball)
end

---Command create callback
---@param InCommand ActionLaunchProjectile
function LogicFireball:OnFireProjectileCreated(InCommand)
    InCommand:SetCommandInfo(self._ProjectileInfo, self._TargetInfo)
end

---Finish callback
function LogicFireball:OnFireProjectileFinished(Result)
    self:FinishWithParams(Result)
end

return LogicFireball
