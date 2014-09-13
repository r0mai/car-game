function getFitness(tracks)
	local result = 0
	local multiplier = 1
	for track, data in ipairs(tracks) do
		result = result + 0.5 * data.travelDistance;
		if data.crossedCheckpoints > data.checkpoints then
			result = result + 100 * data.checkpoints + 
					2 * (data.crossedCheckpoints - data.checkpoints)
			multiplier = multiplier + 1
		else
			result = result + 100 * data.crossedCheckpoints
		end
	end
	return result * multiplier
end
