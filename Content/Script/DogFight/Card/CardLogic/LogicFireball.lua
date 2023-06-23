local CardLogicCommand = require "Card.CardCommand.CardLogicCommand"
local ProjectileTypeDef = require "DogFight.Services.ProjectileService.ProjectileTypeDef"
local CardModifierTypeDef = require "DogFight.Services.CardModifierService.CardModifierTypeDef"
local AttributeNameDef = require "DogFight.Common.AttributeNameDef"

---@class LogicFireball : CardLogicCommand Shoot fireball toward target.
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

    -- Register all commands
    local CommandTable = {
        [CommandNameDef.AcquireTarget] = {
            Script = "DogFight.Card.CardAction.ActionCardAcquireTarget",
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

---Command finish callback
function LogicFireball:OnAcquireTargetFinished(Result, TargetInfo)
    self._TargetInfo = TargetInfo
    self:RunCommand(CommandNameDef.Fireball)
end

---Command create callback
---@param InCommand ActionLaunchProjectile
function LogicFireball:OnFireProjectileCreated(InCommand)
    local bOverrideDamage, ProjectileDamage = self._CardLogic:GetOwnerCard():GetAttributeIntegerValue(AttributeNameDef.Damage)
    local bOverrideProjectileSpeed, ProjectileSpeed = self._CardLogic:GetOwnerCard():GetAttributeIntegerValue(AttributeNameDef.ProjectileSpeed)

    local ProjectileInfo = {
        Id = ProjectileTypeDef.Fireball,
        MuzzleSpeed = bOverrideProjectileSpeed and ProjectileSpeed or 1500,
    }
    if bOverrideDamage then
        ProjectileInfo.Damage = ProjectileDamage
    end
    InCommand:SetCommandInfo(ProjectileInfo, self._TargetInfo)
end

---Finish callback
function LogicFireball:OnFireProjectileFinished(Result)
    self:FinishWithParams(Result)
end

return LogicFireball
