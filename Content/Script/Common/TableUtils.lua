---Find specified key in given table.
---@param table table Table to search with.
---@param key any Key to search.
---@return boolean Whether the key is exist.
function table.containsKey(table, key)
    local bFind = false

    if type(table) == "table" then
        for k, _ in pairs(table) do
            if k == key then
                bFind = true
                break
            end
        end
    end

    return bFind
end

---Find specified value in given table.
---@param table table Table to search with.
---@param value any Value to search.
---@return boolean Whether the value is exist.
function table.containsValue(table, value)
    local bFind = false

    if type(table) == "table" then
        for _, v in pairs(table) do
            if v == value then
                bFind = true
                break
            end
        end
    end

    return bFind
end

---Remove single given value from table.
function table.removeValue(table, value)
    for i, v in ipairs(table) do
        if v == value then
            table.remove(table, i)
            break
        end
    end
end