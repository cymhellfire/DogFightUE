---@class NumberHelper Helper class to handle numbers.
local NumberHelper = {}

-- rounds a number to the nearest decimal places
local function round(val, decimal)
    if (decimal) then
        return math.floor( (val * 10^decimal) + 0.5) / (10^decimal)
    else
        return math.floor(val+0.5)
    end
end

-- given a numeric value formats output with comma to separate thousands
-- and rounded to given decimal places
function NumberHelper.format_num(amount, decimal, prefix, neg_prefix)
    local str_amount,  formatted, famount, remain
  
    decimal = decimal or 2  -- default 2 decimal places
    neg_prefix = neg_prefix or "-" -- default negative sign
  
    famount = math.abs(round(amount,decimal))
    famount = math.floor(famount)
  
    remain = round(math.abs(amount) - famount, decimal)
  
    formatted = tostring(famount)
  
    -- attach the decimal portion
    if (decimal > 0) then
        remain = string.sub(tostring(remain),3)
        formatted = formatted .. "." .. remain ..
                  string.rep("0", decimal - string.len(remain))
    end
  
    -- attach prefix string e.g '$' 
    formatted = (prefix or "") .. formatted 
  
    -- if value is negative then format accordingly
    if (amount<0) then
        if (neg_prefix=="()") then
            formatted = "("..formatted ..")"
        else
            formatted = neg_prefix .. formatted 
        end
    end
  
    return formatted
  end

  return NumberHelper