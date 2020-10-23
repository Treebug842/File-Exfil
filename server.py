import socket
import optparse

parser = optparse.OptionParser()
parser.add_option("-p", dest="port", help="port to listen on")
parser.add_option("-o", dest="output", help="directory to save files")
(options, arguments) = parser.parse_args()
if not options.port:
        parser.error("Must specify a port!")
if options.output:
	save_dest = options.output
else:
	save_dest = None

server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind(("0.0.0.0", int(options.port)))
start = 0
filedata = []

print("Waiting for files...")
try:
	while True:
		server.listen(1)
		conn, address = server.accept()
		while True:
			data = conn.recv(4096)
			if not data:
				break
			if start == 0:
				print(data.strip('\n') + " recieved from " + str(address[0]) + " on port " + str(address[1]))
				filename = str(data)
			else:
				filedata.append(data)
			start = 1
		conn.close()
		output = save_dest + "/" + filename if save_dest != None else filename
		with open(output, "wb") as file:
			for line in filedata:
				file.write(line)
		filedata = []
		start = 0
except KeyboardInterrupt:
	print("\nStopping...")
	exit()