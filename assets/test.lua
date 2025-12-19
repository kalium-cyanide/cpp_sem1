pieces = query('.piece')
cells = query('.cell')
root = query('#root'):get(0)
ghost = nil

state = {
    dragged = nil,
    start_parent = nil
}

pieces:on('mousedown', function(e)
    print('Start drag')
    state.dragged = e.target
    state.start_parent = e.target:parent()

    root:appendChild(e.target)
end)

pieces:on('mousemove', function(e)


    print('dragging')
    local w = state.dragged:w()
    local h = state.dragged:h()
    state.dragged:set_style('position', 'absolute')
    state.dragged:set_style('left', (e.client_x - w / 2) .. 'px')
    state.dragged:set_style('top', (e.client_y - h / 2) .. 'px')

end)

pieces:on('mouseup', function(e)

    local cell = e.currentTarget

    if cell ~= state.dragged then
        cell:appendChild(state.dragged)
    end

    state.dragged:set_style('position', 'flex')

    state.dragged = nil

end)