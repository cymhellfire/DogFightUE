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

return CardCommandHelper