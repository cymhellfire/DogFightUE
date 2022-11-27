local ArrayHelper = {}

---Convert input table into a Unreal string array.
---@param InTable table @Table to convert
---@return TArray @New array
function ArrayHelper.StringArrayFromTable(InTable)
    if type(InTable) ~= "table" then
        return nil
    end

    local Array = UE.TArray("")
    for _, v in ipairs(InTable) do
        Array:Add(v)
    end
    return Array
end

return ArrayHelper