require "UnLua"

---@class UtilSceneController : BP_Ctrl_UtilsScene_C
local UtilSceneController = Class()

function UtilSceneController:ReceiveBeginPlay()
    
end

---Set the projectile to spawn.
---@param InId number Id of projectile.
function UtilSceneController:SetProjectileId(InId)
    self.CurProjectileId = InId
end

function UtilSceneController:OnTargetClicked(InPos)
    print("OnTargetClicked: Click at [" .. InPos.X .. "," .. InPos.Y .. "," .. InPos.Z .. "]")
    if self.CurProjectileId then
        ---@type ProjectileService
        local ProjectileService = GetGameService(GameServiceNameDef.ProjectileService)
        if ProjectileService then
            local NewProjectile = ProjectileService:SpawnProjectileAtPos(self.CurProjectileId, self:K2_GetActorLocation(), self:K2_GetActorRotation())
            -- Launch new projectile to target
            if NewProjectile then
                local FinalPos = UE.FVector(InPos.X, InPos.Y, InPos.Z + 200)
                NewProjectile:LaunchToTargetWithSpeed(FinalPos, 1000)
            end
        end
    end
end

local BindKey = UnLua.Input.BindKey

BindKey(UtilSceneController, "LeftMouseButton", "Pressed", function(self, Key)
    -- Get hit info from cursor position
    local bHit, HitResult = self:GetHitResultUnderCursor(UE.ECollisionChannel.ECC_Visibility, false)
    if bHit and HitResult.bBlockingHit then
        self:OnTargetClicked(HitResult.Location)
    end
end)

return UtilSceneController