if package.loaded["mod_mgmtmode"] then return end
if not ioncore.load_module("mod_mgmtmode") then
  return
end


-- Mark ourselves loaded.
package.loaded["mod_mgmtmode"]=true

-- Load configuration file
dopath("cfg_mgmtmode", false)
