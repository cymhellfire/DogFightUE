---@class CardTargetHelper Helper class to process card target information.
local CardTargetHelper = {}

---Convert a target information struct to lua table.
---@param TargetInfo FAcquiredTargetInfo Target information struct.
---@return table Lua table contains the target information.
function CardTargetHelper.TargetInfoToTable(TargetInfo)
    local NewTable = {
        Type = TargetInfo.Type,
    }

    if TargetInfo.Type == UE.ECardTargetType.CTT_Actor then
        NewTable["Target"] = TargetInfo.ActorPtr
    else
        NewTable["Target"] = TargetInfo.Vector
    end

    return NewTable
end

---Get a vector represent the target position.
---@param InTable table Target information table (native info need be converted before use).
---@return FVector Position of target (nullable).
function CardTargetHelper.TargetInfoToPosition(InTable)
    -- Direction cannot be converted to position
    if not InTable or InTable.Type == UE.ECardTargetType.CTT_Direction then
        return
    end

    -- Use target actor location as target
    if InTable.Type == UE.ECardTargetType.CTT_Actor then
        return InTable.Target and InTable.Target:K2_GetActorLocation() or nil
    end
    
    if InTable.Type == UE.ECardTargetType.CTT_Point then
        return InTable.Target
    end
end

return CardTargetHelper