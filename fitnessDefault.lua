function getFitness(tracks)
	local result = 0
	local multiplier = 1
	for track, data in ipairs(tracks) do
		local fitness = 0.5 * data.travelDistance;
		if data.crossedCheckpoints > data.checkpoints then
			fitness = fitness + 100 * data.checkpoints +
					2 * (data.crossedCheckpoints - data.checkpoints)
			multiplier = multiplier + 1
		else
			fitness = fitness + 100 * data.crossedCheckpoints
		end
		result = result + fitness
	end
	return result * multiplier
end
