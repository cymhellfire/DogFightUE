
local function UnrealClass(super_name, className)
	local new_class = nil
	-- Use the default behavior for class
	if not super_name or type(super_name) == "string" then
		new_class = UnLua.Class(super_name)
	-- Use the given class directly
	elseif type(super_name) == "table" then
		new_class = UnLua.Class()
		new_class.Super = super_name
	end

	-- Instantiate function
	new_class.New = function(self, object)
		object = object or {}
		setmetatable(object, self)
		if object.ctor then
			object:ctor()
		end
		return object
	end

	return new_class
end

_G.UnrealClass = UnrealClass