
# set things up here
tscale=100

# this function is called once per buffer upddate and is passed
# the current buffer in data
function oe_process!(data)

	#data[:]=reverse(data);
	data[:]=sin(data/tscale)*200
end
