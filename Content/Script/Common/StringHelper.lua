---Check if the string starts with given sub string.
---@param String string @ String to check with
---@param Pattern string @ Sub string to match
function string.startWith(String, Pattern)
    return string.sub(String, 1, string.len(Pattern)) == Pattern
end
