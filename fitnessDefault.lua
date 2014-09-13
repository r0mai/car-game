function getFitness(tracks)
	local result = 0
	for track, data in ipairs(tracks) do
		result = result + 0.5 * data.travelDistance;
		if data.crossedCheckpoints > data.checkpoints then
			result = result + 100 * data.checkpoints + 
					2 * (data.crossedCheckpoints - data.checkpoints)
		else
			result = result + 100 * data.crossedCheckpoints
		end
	end
	return result
end
