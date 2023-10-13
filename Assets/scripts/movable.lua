function Update(L, R, offset, dt)
	if L then
		return offset - dt
	elseif R then
		return offset + dt
	end
	return offset
end