require "UnLua"

local ModelBase = Class()

function ModelBase:BindViewModel(InVM, InTable)
    if not InVM then
        return
    end
    self.ViewModel = InVM
    -- Add all binding to view model
    self.ViewModel:SetupBinding(self, InTable)
end

return ModelBase