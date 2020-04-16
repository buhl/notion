if package.loaded["mod_modal"] then return end
if not ioncore.load_module("mod_modal") then
  return
end


-- Mark ourselves loaded.
package.loaded["mod_modal"]=true

-- Load configuration file
dopath("cfg_modal", false)
