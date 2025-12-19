local original_positions = {}

local app = {
    state = "MENU",

    ui = {
        menu = query("#main-menu"):get(0),
        game_layer = query("#game-layer"):get(0),
        game_over = query("#game-over-screen"):get(0),
        board = query("#board"):get(0),
        winner_text = query("#winner-text"):get(0)
    },

    game = {
        widget_pool = {},
        active_pieces = {},
        drag = { active = false, widget = nil, start_mx = 0, start_my = 0, orig_x = 0, orig_y = 0, from_col = 0, from_row = 0 }
    }
}

function set_visible(widget, is_visible)
    if not widget then
        return
    end

    local id = widget:id()
    local position = widget:get_style('position')

    if is_visible then
        local x = original_positions[id] and original_positions[id].x or "0px"
        local y = original_positions[id] and original_positions[id].y or "0px"
        widget:set_style('position', original_positions[id].type)

        widget:set_style("left", x)
        widget:set_style("top", y)
    else
        if not original_positions[id] then
            original_positions[id] = {
                x = widget:x() .. "px",
                y = widget:y() .. "px",
                type = position
            }
        end
        widget:set_style('position', 'absolute')
        widget:set_style("left", "10000px")
        widget:set_style("top", "10000px")
    end
end

local function init_pool()
    local q = query(".piece")
    local i = 0
    while true do
        local w = q:get(i)
        if not w then
            break
        end

        local id = w:id()
        if not app.game.widget_pool[id] then
            app.game.widget_pool[id] = {}
        end

        w:addEventListener("mousedown", function(e)
            handle_input(w, e)
        end)
        w:addEventListener("mousemove", function(e)
            handle_input(w, e)
        end)
        w:addEventListener("mouseup", function(e)
            handle_input(w, e)
        end)

        table.insert(app.game.widget_pool[id], w)
        set_visible(w, false)
        i = i + 1
    end
end

function sync_board_state()
    if app.state ~= "GAME" then
        return
    end

    app.game.active_pieces = {}
    local used_counters = {}

    for row = 1, 8 do
        for col = 1, 8 do
            local fig_type = get_figure(col, row)
            if fig_type ~= "" then
                if not used_counters[fig_type] then
                    used_counters[fig_type] = 1
                end
                local idx = used_counters[fig_type]

                if app.game.widget_pool[fig_type] and app.game.widget_pool[fig_type][idx] then
                    local w = app.game.widget_pool[fig_type][idx]
                    table.insert(app.game.active_pieces, {
                        widget = w,
                        col = col,
                        row = row
                    })

                    if app.game.drag.active and app.game.drag.widget == w then
                        app.game.drag.from_col = col
                        app.game.drag.from_row = row
                    end
                end
                used_counters[fig_type] = idx + 1
            end
        end
    end

    for type, widgets in pairs(app.game.widget_pool) do
        local start_idx = used_counters[type] or 1
        for i = start_idx, #widgets do
            set_visible(widgets[i], false)
        end
    end
end

function handle_input(w, e)
    print("Event type: " .. e.type .. ", client_x: " .. e.client_x .. ", client_y: " .. e.client_y)

    if app.state ~= "GAME" then
        return
    end

    local d = app.game.drag
    local b = app.ui.board

    if e.type == "mousedown" then
        d.active = true
        d.widget = w
        d.start_mx = e.client_x
        d.start_my = e.client_y
        d.orig_x = w:x()
        d.orig_y = w:y()

        for _, item in ipairs(app.game.active_pieces) do
            if item.widget == w then
                d.from_col = item.col
                d.from_row = item.row
                break
            end
        end
        e:stopPropagation()

    elseif e.type == "mousemove" and d.active and d.widget == w then
        local dx = e.client_x - d.start_mx
        local dy = e.client_y - d.start_my
        w:set_style("left", (d.orig_x + dx) .. "px")
        w:set_style("top", (d.orig_y + dy) .. "px")

    elseif e.type == "mouseup" then
        if d.active and d.widget == w and b then

            w:set_style("left", d.orig_x .. "px")
            w:set_style("top", d.orig_y .. "px")

            local cell_w = b:w() / 8
            local cell_h = b:h() / 8

            local release_x = e.client_x
            local release_y = e.client_y

            local rel_x = release_x - b:x()
            local rel_y = release_y - b:y()

            local to_col = math.floor(rel_x / cell_w) + 1
            local to_row = math.floor(rel_y / cell_h) + 1

            if to_col >= 1 and to_col <= 8 and to_row >= 1 and to_row <= 8 then

                local from_type = get_figure(d.from_col, d.from_row)
                local is_castling = false

                if (d.from_row == 1 or d.from_row == 8) and d.from_col == 5 then
                    if to_col == 7 or to_col == 8 then
                        make_move(-1, 0, 0, 0)
                        is_castling = true
                    elseif to_col == 3 or to_col == 1 then
                        make_move(-2, 0, 0, 0)
                        is_castling = true
                    end
                end

                if not is_castling then
                    make_move(d.from_col, d.from_row, to_col, to_row)
                end
                check_game_over()
            end
        end

        d.active = false
        d.widget = nil
        sync_board_state()
        update_view()
    end
end

function update_view()
    if app.state ~= "GAME" or not app.ui.board then
        return
    end

    local bx = app.ui.board:x()
    local by = app.ui.board:y()
    local cell_w = app.ui.board:w() / 8
    local cell_h = app.ui.board:h() / 8

    for _, item in ipairs(app.game.active_pieces) do
        local w = item.widget
        w:set_style("width", cell_w .. "px")
        w:set_style("height", cell_h .. "px")

        if not (app.game.drag.active and app.game.drag.widget == w) then
            local abs_x = bx + (item.col - 1) * cell_w
            local abs_y = by + (item.row - 1) * cell_h
            w:set_style("left", abs_x .. "px")
            w:set_style("top", abs_y .. "px")
        end
    end
end

function check_game_over()
    local status = cpp_get_status()
    if status ~= -1 then
        switch_scene("GAMEOVER")
    end
end

function switch_scene(new_state)
    app.state = new_state

    set_visible(app.ui.menu, false)
    set_visible(app.ui.game_over, false)
    set_visible(app.ui.game_layer, false)

    if app.ui.board then
        set_visible(app.ui.board, false)
    end

    for _, list in pairs(app.game.widget_pool) do
        for _, w in ipairs(list) do
            set_visible(w, false)
        end
    end

    if new_state == "MENU" then
        set_visible(app.ui.menu, true)
    elseif new_state == "GAME" then
        set_visible(app.ui.game_layer, true)
        if app.ui.board then
            set_visible(app.ui.board, true)
        end
        sync_board_state()
        update_view()
    elseif new_state == "GAMEOVER" then
        set_visible(app.ui.game_layer, false)
        if app.ui.board then
            set_visible(app.ui.board, false)
        end

        set_visible(app.ui.game_over, true)
        update_view()
    end
end

local function setup_button(selector, action)
    local btn_list = query(selector)
    local btn = btn_list:get(0)

    if btn then

        btn:addEventListener("click", function(e)
            print("Click on " .. selector)
            action()
            e:stopPropagation()
        end)
    else
        print("ERROR: Button not found: " .. selector)
    end
end

function bind_buttons()
    setup_button("#btn-start", function()
        cpp_restart_game()
        switch_scene("GAME")
    end)

    setup_button("#btn-exit", function()
        cpp_exit_app()
    end)

    setup_button("#btn-restart", function()
        cpp_restart_game()
        switch_scene("GAME")
    end)

    setup_button("#btn-menu", function()
        switch_scene("MENU")
    end)
end

init_pool()
bind_buttons()
switch_scene("MENU")