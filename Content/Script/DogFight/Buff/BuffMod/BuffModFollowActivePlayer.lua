---@class BuffModFollowActivePlayer : BuffModBase Make buff always switch to current active player character.
local BuffModFollowActivePlayer = UnrealClass("DogFight.Buff.BuffMod.BuffModBase")

---@param InLogic BuffLogicBase
function BuffModFollowActivePlayer:OnInit(InLogic)
    self.Super.OnInit(self, InLogic)

    ---@type LuaEventService
    local LuaEventService = GetGameService(self._Buff, GameServiceNameDef.LuaEventService)
    if LuaEventService then
       LuaEventService:RegisterListener(UE.ELuaEvent.LuaEvent_GameTimelineChanged, self, self.OnTimelineChanged) 
    end
end

function BuffModFollowActivePlayer:OnUninit()
    self.Super.OnUninit(self)

    ---@type LuaEventService
    local LuaEventService = GetGameService(self._Buff, GameServiceNameDef.LuaEventService)
    if LuaEventService then
       LuaEventService:UnregisterListener(UE.ELuaEvent.LuaEvent_GameTimelineChanged, self, self.OnTimelineChanged) 
    end
end

function BuffModFollowActivePlayer:OnTimelineChanged()
    local PlayerId = UE.UCommonGameFlowFunctionLibrary.GetTimelineFirstPlayerId(self._Buff)
    if PlayerId < 0 then
        return
    end

    ---@type ATopDownStylePlayerCharacter
    local NewCharacter = UE.UCommonGameplayFunctionLibrary.GetPlayerCharacterById(self._Buff, PlayerId)
    if NewCharacter then
        -- Transfer to new target
        self._BuffLogic._Owner:TransferTo(NewCharacter)
    end
end

function BuffModFollowActivePlayer:tostring()
    return "BuffModFollowActivePlayer"
end

return BuffModFollowActivePlayer