---@field ViewModel table ViewModel table of this model.
---@class ModelBase Base class of all widget want to use MVVM partten.
local ModelBase = UnrealClass()

---Bind a new ViewModel to this model.
---@param InVM table ViewModel to bind.
---@param InTable ModelBase Model which the ViewModel bind to.
function ModelBase:BindViewModel(InVM, InTable)
    if not InVM then
        return
    end

    self.ViewModel = InVM
    -- Add all binding to view model
    self.ViewModel:SetupBinding(self, InTable)
end

return ModelBase