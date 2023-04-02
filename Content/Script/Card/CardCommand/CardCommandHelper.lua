---@class CardCommandHelper Helper function library for card command classes.
local CardCommandHelper = {}

---Create a card command instance based on given path.
---@param InPath string Script path of card command
---@param ... any Initialize parameters for new command.
---@return CardCommandBase New card command instance.
function CardCommandHelper.CreateCardCommand(InPath, ...)
    ---@type CardCommandBase
    local CommandClass = require(InPath)
    if CommandClass and CommandClass.New then
        -- Instantiate loaded command
        local NewCommand = CommandClass:New()
        -- Invoke initialize function
        if NewCommand.OnInit then
            NewCommand:OnInit(...)
        end
        return NewCommand
    end

    return nil
end

---Get the player controller of the owning card of given command.
---@param InCommand CardCommandBase Command to get owner.
---@return ATopDownStylePlayerController Owner player controller.
function CardCommandHelper.GetOwnerPlayerController(InCommand)
    if not InCommand then
        return
    end

    -- Get the owner player id
    local PlayerId = -1
    if InCommand._CardLogic then
        ---@type UCard
        local OwnerCard = InCommand._CardLogic:GetOwnerCard()
        if OwnerCard then
            PlayerId = OwnerCard:GetOwnerPlayerId()
        end
    end

    if PlayerId ~= -1 then
        return UE.UCommonGameplayFunctionLibrary.GetPlayerControllerById(PlayerId)
    end
end

return CardCommandHelper