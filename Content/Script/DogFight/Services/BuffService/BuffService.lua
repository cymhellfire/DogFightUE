---@class BuffService : GameServiceBase, UBuffService
local BuffService = UnrealClass("DogFight.Services.GameServiceBase")

function BuffService:GetConfigPath()
    return "DogFight.Services.BuffService.BuffConfig"
end

---@param InId number Id of buff to create
function BuffService:CreateBuffById(InId)
    local Config = self.Config:GetConfig(InId)
    if not Config then
        print("[BuffService] Failed to find buff config with " .. InId)
        return
    end

    ---@type UNewBuffBase
    local NewBuff = NewObject(Config.Class or UE.UNewBuffBase.StaticClass(), self)
    if NewBuff then
        NewBuff:InitBuff(Config.Script)
    end

    return NewBuff
end

return BuffService