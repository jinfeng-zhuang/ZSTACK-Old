# README

## Frame

for channel=0,1,2 & enable
	sample 3072
	delta = max - min
	if delta > threshold:
		workable = 1

for channel=0,1,2 & enable & workable
	running = 1

for channel=0,1,2 & enable & workable & running
	