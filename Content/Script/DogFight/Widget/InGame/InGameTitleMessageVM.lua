require "UnLua"

local InGameTitleMessageVM = require("Common.MVVM.ViewModelBase").New()

InGameTitleMessageVM.BindingTable = {
    ["TitleMessage"] = "1",
}

return InGameTitleMessageVM