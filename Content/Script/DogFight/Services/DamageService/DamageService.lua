require "UnLua"

---@class DamageService : GameServiceBase, UDamageService Service that handle damage event and config.
local DamageService = Class("DogFight.Services.GameServiceBase")
local DamageNameDef = require("DogFight.Services.DamageService.DamageNameDef")

function DamageService:StartupScript(ServiceName)
    self.Super.StartupScript(self, ServiceName)

    self.DamageWidgetPath = "/Game/DogFightGame/UI/InGame/DamageDisplay/BP_Widget_DamageDisplayUnit"

    -- Instaniate damage instance
    self:InstantiateDamageType()
end

function DamageService:GetConfigPath()
    return "DogFight.Services.DamageService.DamageConfig"
end

---Initialize damage instance with configuration.
---@param DamageInstance UExtendedDamageInstance Damage instance.
---@param Config table Damage instance configuration.
local function InitializeDamageInstance(DamageInstance, Config)
    if Config.DamageType then
        DamageInstance.Id = Config.Id
        DamageInstance:SetDamageType(Config.DamageType)
    end
end

---Instantiate all damage type based on configurations.
function DamageService:InstantiateDamageType()
    ---@type UClass
    local DamageClass = UE.UExtendedDamageInstance.StaticClass()
    if not DamageClass then
        print("[DamageService] Cannot find damage class")
        return
    end

    for k, v in pairs(self.Config.Data) do
        local DamageId = v.Id
        local NewDamage = NewObject(DamageClass, self, nil, v.Script)
        if NewDamage then
            InitializeDamageInstance(NewDamage, v)
            self:RegisterDamageType(DamageId, NewDamage)
        end
    end
end

function DamageService:CreateDamageCalculator(InClass, InPath, Instigator)
    if not InClass:IsValid() then
        return nil
    end

    local NewInstance = NewObject(InClass, Instigator, nil, InPath)
    return NewInstance
end

---Apply damage to target actor.
---@param DamageId number Id of damage config.
---@param Target AActor Target actor.
---@param BaseDamage number Initial value of damage.
---@param Causer AActor Causer of this damage.
---@param InCalculator UDamageCalculatorBase Damage calculator instance to use.
function DamageService:ApplyDamageToActor(DamageId, Target, InValue, Causer, InCalculator)
    local DamageInstance = self:GetDamageInstanceById(DamageId)
    if not DamageInstance then
        return
    end

    if not InCalculator:IsValid() then
        return
    end

    InCalculator:ApplyDamage(Target, InValue, DamageInstance, Causer, nil)
end

function DamageService:CreateDamageWidget()
    -- Load the resource
    local LoadedClass = UE.ULuaIntegrationFunctionLibrary.LoadClassByPath(self.DamageWidgetPath)
    if not LoadedClass then
        print("[DamageService] Failed to load damage display widget: " .. self.DamageWidgetPath)
        return
    end

    ---Create new widget
    return UE.UWidgetBlueprintLibrary.Create(GetCurrentWorld(self), LoadedClass, GetLocalPlayerController(self))
end

return DamageService