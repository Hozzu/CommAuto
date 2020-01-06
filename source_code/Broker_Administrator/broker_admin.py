from flask import Flask, jsonify, request
from ast import literal_eval
import json

app = Flask(__name__)

broker_set = set([])

@app.route('/get_broker_list', methods = ['GET'])
def get_broker_list():
	return jsonify({'data': list(broker_set)})
	
@app.route('/register', methods = ['POST'])
def register():
	data = request.data.decode('utf-8')
	data = data.replace('{', '{"')
	data = data.replace('}', '"}')
	data = data.replace(':', '":"')
	data = data.replace(',', '","')
	#print(data)

	data = json.loads(data)
	broker_set.add(data['host'] + ':' + data['port'])
	#print(broker_set)
	return 'success'

host_addr = "0.0.0.0"
port_num = "8080"

if __name__ == "__main__":
	app.run(host=host_addr, port=port_num)
