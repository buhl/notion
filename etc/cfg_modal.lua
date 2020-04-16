--[[
-- TODO: 
--   [v] Movement
--   [v] Window movement
--   [v] Find obj type Up/Down
--   [v] Kill window example
--   [v] Spawn terminal example
--   [\] Tag example
--   [v] Attach example
--   [\] new WS
--   [ ] destroy WS
--   [v] P :manager
--   [v] N :current
--   [ ] resize like info box with mode->selected info
--   [ ] rubber box drawing error (sometimes the original frame doesn't get reset at movement)
--
--]]
--

--DOC
-- Find a current object with type name \var{t} under \var{obj} or one of
-- its current objects.

modal = {}

function modal.set_tagged(obj)
    fr = ioncore.find_current(obj, "WFrame") or ioncore.find_manager(obj, "WFrame")
    reg = fr:current()
    if min_tabs_tag_wrapper then
        min_tabs_tag_wrapper(fr, reg)
    else
        WRegion.set_tagged(reg, "toggle")
    end
end

function modal.focus_next(mode, dir)
    local f=mode:selected()
    --repl.inspect_object(f)
    if f then
       local nxt=ioncore.navi_next(f, dir)
       -- Does not warp. TODO.
       if nxt then
           mode:select(nxt)
           nxt:goto_focus()
       end
    end
end


function modal.move(mode, dir)
    local frame=mode:selected()
    local cwin=frame:current()
    local frame2=ioncore.navi_next(frame,dir)

    if frame2 then
        frame2:attach(cwin, { switchto=true })
        mode:select(frame2)
    end
    cwin:goto_focus()
end

function modal.kill(mode, _, _sub)
    if _sub and obj_is(_sub, "WClientWin") then
        WClientWin.kill(_sub)
    else
        WRegion.rqclose_propagate(_, _sub)
    end
    mode:select(ioncore.find_manager(ioncore.current(), "WFrame"))

end



function modal.debug(mode, _sub, _chld)
    print("-------------------------------------")
    print("mode:", mode)

    print("_sub tree:", _sub)
    print(repl.inspect_object(_sub))

    print("_chld tree:", _chld)
    print(repl.inspect_object(_chld))
    print("-------------------------------------")

end

function modal.hmm(...)
    print("Cought!")
end


--[[


modal.defbindings("Mode",

--]]

defbindings("WFrame", {
     bdoc("Testing MgmtMode"),
     kpress(META.."Escape", "mod_modalmode.begin(_)"),
 })

defbindings("WMgmtMode", {
    kpress("P", "_:select(_sub:manager() or _:selected())"),
    kpress("N", "_:select(_sub or _:selected())"),

    kpress("S", "mod_query.exec_on_merr(_sub, XTERM or 'xterm')"),
    --kpress("T", "WRegion.set_tagged(_chld, 'toggle')", "_chld:non-nil"),
    kpress("T", "modal.set_tagged(_sub)", "_sub:non-nil"),
    kpress("A", "ioncore.tagged_attach(_sub)", "_sub:non-nil"),

    kpress("M", "ioncore.create_ws(ioncore.find_manager(_sub, 'WScreen'))", "_sub:non-nil"),

    kpress("D", "modal.kill(_, _sub, _chld)"),
    kpress("X", "modal.debug(_, _sub, _chld)"),

    kpress("K", "modal.focus_next(_, 'up')"),
    kpress("J", "modal.focus_next(_, 'down')"),
    kpress("H", "modal.focus_next(_, 'left')"),
    kpress("L", "modal.focus_next(_, 'right')"),

    kpress("Shift+K", "modal.move(_, 'up')"),
    kpress("Shift+J", "modal.move(_, 'down')"),
    kpress("Shift+H", "modal.move(_, 'left')"),
    kpress("Shift+L", "modal.move(_, 'right')"),

    kpress("AnyKey", "modal.debug(_, _sub, _chld)")

})
