import os
import socket
import optparse
import tempfile

tmpDir = str(tempfile.mkdtemp())

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
error_code = 0

print("Waiting for files...")
try:
	while True:
		server.listen(1)
		conn, address = server.accept()
		while True:
			data = conn.recv(4096)
			if not data:
				break
			filedata.append(data)
			if data.decode() == "ERROR: 477224891":
				error_code = 1
		conn.close()
		if error_code == 1:
			print("ERROR: Client side error sending file!")
		else:
			filename = filedata.pop(0)
			output = save_dest + "/" + filename.decode() if save_dest != None else filename.decode()
			tmpFile = tmpDir + "/" + filename.decode()
			with open(tmpFile, "wb") as file:
				for line in filedata:
					file.write(line)
			filedata = []
			with open(tmpFile, 'r') as fin:
    				data = fin.read().splitlines(True)
			with open(tmpFile, 'w') as fout:
    				fout.writelines(data[1:])

			with open(tmpFile, 'r') as fin:
    				data = fin.read().splitlines(True)
			with open(tmpFile, 'w') as fout:
    				fout.writelines(data[:-1])

			os.system(f"base64 -d {tmpFile} > {output}")

			print(f"{filename.decode()} recieved from {address[0]} on port {address[1]}")
except KeyboardInterrupt:
	print("\nStopping...")
	exit()
