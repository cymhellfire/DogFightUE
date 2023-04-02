---@field Succeed enum Indicate command executed successfully.
---@field Failure enum Indicate that there are some issues while this command executing.
---@field Cancelled enum Indicate that this command is cancelled.
---@class CardCommandResultDef : table Defined all kind of result a card command can return.
local CardCommandResultDef = {
    Succeed = "Succeed",
    Failure = "Failure",
    Cancelled = "Cancelled",
}

return CardCommandResultDef