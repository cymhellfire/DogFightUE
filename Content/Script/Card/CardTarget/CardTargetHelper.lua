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

---Get the total count of targets that match given type.
---@param InTable table Target information table list (native info need be converted before use).
---@param InType ECardTargetType Type to filter targets.
---@return number Total count of targets.
function CardTargetHelper.GetTargetCountByType(InTable, InType)
    if type(InTable) ~= "table" or #InTable == 0 then
        return 0
    end

    local Count = 0
    for _, v in ipairs(InTable) do
        if v.Type and v.Type == InType then
            Count = Count + 1
        end
    end

    return Count
end

---@param WorldContextObject UObject
---@param Count number
---@return table Card target
local function GenerateRandomPoint(WorldContextObject, Count)
    if not WorldContextObject or not WorldContextObject:IsValid() then
        return
    end

    local Result = {}
    for i = 1, Count do
        local Point = UE.UCommonGameplayFunctionLibrary.GetRandomPointInNavigationArea(WorldContextObject)
        Result[i] = {
            Type = UE.ECardTargetType.CTT_Point,
            Target = Point,
        }
    end

    return Result
end

---@param WorldContextObject UObject
---@param Count number
---@param bAllowDupliated boolean
---@return table Card target
local function GenerateRandomCharacter(WorldContextObject, Count, bAllowDupliated)
    if not WorldContextObject or not WorldContextObject:IsValid() then
        return
    end

    local Result = {}
    local CharacterArray = UE.UCommonGameplayFunctionLibrary.GetRandomCharacterInGame(WorldContextObject, Count, bAllowDupliated):ToTable()
    if #CharacterArray == 0 then
        return
    end

    for i = 1, #CharacterArray do
        Result[i] = {
            Type = UE.ECardTargetType.CTT_Actor,
            Target = CharacterArray[i]
        }
    end
    return Result
end

---Get random target based on given settings.
---@param TargetSettings table Settings of random targets.
---@return table Lua table contains the target information.
function CardTargetHelper.GetRandomTarget(TargetSettings)
    if type(TargetSettings) ~= "table" then
        return
    end

    local TargetType = TargetSettings.Type
    local Count = TargetSettings.Count or 1
    local Result
    if TargetType == UE.ECardTargetType.CTT_Actor then
        Result = GenerateRandomCharacter(TargetSettings.WorldContextObject, Count, TargetSettings.AllowDuplicated)
    elseif TargetType == UE.ECardTargetType.CTT_Point then
        Result = GenerateRandomPoint(TargetSettings.WorldContextObject, Count)
    end

    return Result
end

return CardTargetHelper